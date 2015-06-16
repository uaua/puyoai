#include <gflags/gflags.h>
#include <glog/logging.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <tuple>

#include "base/base.h"
#include "core/algorithm/plan.h"
#include "core/algorithm/rensa_detector.h"
#include "core/algorithm/rensa_detector_strategy.h"
#include "core/algorithm/puyo_possibility.h"
#include "core/client/ai/ai.h"
#include "core/core_field.h"
#include "core/frame_request.h"
#include <iostream>
#include <random>
#include <fstream>

#include "base/strings.h"

using namespace std;

typedef long long ll;

class SampleRensaAI : public AI {
private:
  ll kScore;
  ll kChain;
  ll kCover;
  ll k3Height;
  ll kCount;
  ll kDansa;
  ll kTate3;
  ll kLji;
  ll kYoko3;
  int argc;
  string a[3];
  vector<string> v;
  std::random_device rd;
  const double K;
  struct Con {
    int tate3,yoko3,lji;
    Con():tate3(0),yoko3(0),lji(0){}
    Con(int t,int y,int l):tate3(t),yoko3(y),lji(l){}
  };
public:
  SampleRensaAI(int argc, char* argv[]) : AI(argc, argv, "sample_rensa"), argc(argc),v(20),K(0.4) {
    if( argc < 7 ) {
      a[0] = argv[1];
      std::ifstream ifs( string(a[0])+".txt" );
      ifs >> v[0];
      kScore = atoll(v[0].c_str());
      ifs >> v[1];
      kChain = atoll(v[1].c_str());
      ifs >> v[2];
      kCover = atoll(v[2].c_str());
      ifs >> v[3];
      k3Height = atoll(v[3].c_str());
      ifs >> v[4];
      kCount = atoll(v[4].c_str());
      ifs >> v[5];
      kDansa = atoll(v[5].c_str());
      ifs >> v[6];
      kTate3 = atoll(v[6].c_str());
      ifs >> v[7];
      kLji = atoll(v[7].c_str());
      ifs >> v[8];
      kYoko3 = atoll(v[8].c_str());
    } else {
    }
  }
  virtual ~SampleRensaAI() {}

  virtual DropDecision think(int frameId, const CoreField& f, const KumipuyoSeq& seq,
                             const PlayerState& me, const PlayerState& enemy, bool fast) const override
  {
    //UNUSED_VARIABLE(frameId);
    //UNUSED_VARIABLE(me);
    //UNUSED_VARIABLE(enemy);
    return eval(frameId,f, me, enemy, seq, fast ? 2 : 3);
  }

private:
  DropDecision eval(int frameId,const CoreField& f,const PlayerState& me,const PlayerState& enemy, const KumipuyoSeq& nexts, int depth) const
  {
    LOG(INFO) << f.toDebugString() << nexts.toString();

    Decision best = Decision(3, 0);
    ll score = numeric_limits<long>::min();
    std::string msg = "";
    bool tsumi = true;
    if( enemy.isRensaOngoing() || me.totalOjama(enemy)>0 ) {
      vector<RensaResult> es;
      es.reserve(11111);
      std::mt19937 mt;
      std::uniform_int_distribution<int> ojama_range(2,4);
      bool prohibits[FieldConstant::MAP_WIDTH]={};
      RensaDetector::detectByDropStrategy(enemy.field,
                                          prohibits,
                                          PurposeForFindingRensa::FOR_FIRE,
                                          10,
                                          13,
                                          [&](CoreField&& f,
                                              const ColumnPuyoList& fire_puyos) {
                                            RensaResult r = f.simulate();
                                            if( r.score >= 70*ojama_range(mt) ) {
                                              es.push_back(r);
                                            }
                                          });
      sort(es.begin(),es.end(),[](const RensaResult& a,const RensaResult& b){
          return a.score > b.score;
        });
      int ch = enemy.currentChain;
      int sc = enemy.currentRensaResult.score + enemy.unusedScore;
      ll maxSc=numeric_limits<long>::min();
      typedef std::tuple<Decision,RensaResult,CoreField> pack;
      vector<pack> v,w;
      msg = "taiou: " +to_string(enemy.rensaFinishingFrameId()-frameId) + ":"+to_string(me.totalOjama(enemy));
      //enemy.field.simulate();
      Plan::iterateAvailablePlans(f, nexts, depth, [&](const RefPlan& plan) {
          if( !plan.isRensaPlan() ) {
            RensaDetector::detectByDropStrategy(plan.field(),
                                                prohibits,
                                                PurposeForFindingRensa::FOR_FIRE,
                                                max(min(10,(enemy.rensaFinishingFrameId()-frameId)/30*2),4),
                                                13,
                                                [&](CoreField&& f,
                                                    const ColumnPuyoList& fire_puyos) {
                                                  RensaResult r = f.simulate();
                                                  const PuyoSet ps(fire_puyos);
                                                  if(TsumoPossibility::possibility(ps,fire_puyos.size()) >= K) {
                                                    if( me.totalOjama(enemy)*70 < r.score ) {
                                                      w.emplace_back(plan.decisions().front(), r, f);
                                                    }
                                                  }
                                                });
          } else {
            if( plan.framesToIgnite() < enemy.rensaFinishingFrameId()-frameId-3 ) {
              if( me.totalOjama(enemy)*70 <= plan.rensaResult().score ) {
                //msg += to_string(plan.framesToIgnite())+" ";
                //best = plan.decisions().front();
                v.emplace_back(plan.decisions().front(), plan.rensaResult(), plan.field());
              } else {
                if( maxSc < plan.rensaResult().score && plan.numChigiri() <= 1 ) {
                  maxSc = plan.rensaResult().score;
                  best = plan.decisions().front();
                }
              }
            }
          }
        });
      if( v.size() == 0 ) {
        if( w.size() > 0 ) {
          msg += ": kibou";
          sort(w.begin(),w.end(),[](const pack& a,const pack& b){
              return get<1>(a).score > get<1>(b).score;
            });
          best = get<0>(w.front());
          tsumi = false;
        }else {
          msg += ": damepo\n";
          if( numeric_limits<long>::min() != maxSc ) { 
            tsumi = false;
          }
        }
      } else {
        msg += ": daijoubu";
        sort(v.begin(),v.end(),[&](const pack& a,const pack& b){
            //return get<1>(a).score > get<1>(b).score;
            return calc(es,get<2>(a),get<1>(a)) > calc(es,get<2>(b),get<1>(b));
          });
        tsumi = false;
        best = get<0>(v.back());
      }
    }
    if(tsumi) {
      vector<RensaResult> es;
      es.reserve(11111);
      std::mt19937 mt;
      std::uniform_int_distribution<int> ojama_range(2,4);
      bool prohibits[FieldConstant::MAP_WIDTH]={};
      RensaDetector::detectByDropStrategy(enemy.field,
                                          prohibits,
                                          PurposeForFindingRensa::FOR_FIRE,
                                          10,
                                          13,
                                          [&](CoreField&& f,
                                              const ColumnPuyoList& fire_puyos) {
                                            RensaResult r = f.simulate();
                                            const PuyoSet ps(fire_puyos);
                                            if(TsumoPossibility::possibility(ps,fire_puyos.size()) >= K) {
                                              if( r.score >= 70*ojama_range(mt) ) {
                                                es.push_back(r);
                                              }
                                            }
                                          });
      sort(es.begin(),es.end(),[](const RensaResult& a,const RensaResult& b){
          return a.score > b.score;
        });
      Plan::iterateAvailablePlans(f, nexts, depth, [&](const RefPlan& plan) {
          //if( !plan.isRensaPlan() ) return;
          int cover = 0;
          ll sc = calc(es,plan);
          if( score < sc ) {
            if( plan.field().height(3) <= 11 ) {
              score = sc;
              best = plan.decisions().front();
            }
          }
        });
    }
    Con con = calcCon(me.field);
    msg += "tate:" + to_string(con.tate3) + "/yoko:" + to_string(con.yoko3) + "/lji:" + to_string(con.lji) + "\n";
    msg += to_string(score) + "\n";
    msg += "now_rensa : " + to_string(enemy.isRensaOngoing()) + "\n";
    msg += "ojama : " + to_string(me.totalOjama(enemy)) + "\n";
    return DropDecision(best,msg+":::"+strings::join(v," : "));
  }
  ll calc(const vector<RensaResult>& es,const RefPlan& plan) const {
    return calc(es,plan.field(),plan.rensaResult());
  }
  Con calcCon(const CoreField& f) const {
    int lji,tate3,yoko3; lji=tate3=yoko3=0;
    for( int x = 1; x < FieldConstant::WIDTH; x++ ) {
      for( int y = 1; y <= FieldConstant::HEIGHT; y++ ) {
        if( f.isNormalColor(x,y) ){
          PuyoColor u,l,d,r,m;
          m = f.color(x,y);
          u = f.color(x,y-1);
          d = f.color(x,y+1);
          r = f.color(x+1,y);
          l = f.color(x-1,y);
          if( m == u && m == d ) tate3 += 1;
          if( m == l && m == r ) yoko3 += 1;
          if( (m == u && m == l) ||
              (m == l && m == d) ||
              (m == d && m == r) ||
              (m == r && m == u) ) lji += 1;
        }
      }
    }
    return Con(tate3,yoko3,lji);
  }
  ll calc(const vector<RensaResult>& es,const CoreField& f, const RensaResult& rr) const {
    bool prohibits[FieldConstant::MAP_WIDTH]={};
    int cover = 0;
    RensaDetector::detectByDropStrategy(f,
                                        prohibits,
                                        PurposeForFindingRensa::FOR_FIRE,
                                        4,
                                        13,
                                        [&](CoreField&& f,
                                            const ColumnPuyoList& fire_puyos) {
                                          RensaResult r = f.simulate();
                                          const PuyoSet ps(fire_puyos);
                                          if(TsumoPossibility::possibility(ps,fire_puyos.size()) >= K) {
                                            cover += upper_bound(es.begin()+cover,es.end(),r,
                                                                 [](const RensaResult& left,const RensaResult& right){
                                                                   return left.score > right.score;
                                                                 })-(es.begin()+cover);
                                          }
                                        });
    int sa = 0;
    for( int i = 1; i < 6; i++ ) {
      sa += abs(f.height(i)-f.height(i+1));
    }
    Con con = calcCon(f);
    ll sc = rr.score*kScore +
      rr.chains*kChain +
      (es.size()-cover+1)*kCover +
      f.height(3)*k3Height +
      f.countColorPuyos()*kCount +
      sa*kDansa +
      con.tate3*kTate3 +
      con.yoko3*kYoko3 +
      con.lji*kLji;
    return sc;
  }
};

int main(int argc, char* argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  SampleRensaAI(argc, argv).runLoop();

  return 0;
}
