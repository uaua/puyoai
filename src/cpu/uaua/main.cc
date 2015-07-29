#include <inttypes.h>
#include <cstdio>
#include <cmath>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <tuple>
#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "base/base.h"
#include "base/strings.h"
#include "core/algorithm/plan.h"
#include "core/algorithm/rensa_detector.h"
#include "core/algorithm/rensa_detector_strategy.h"
#include "core/algorithm/puyo_possibility.h"
#include "core/client/ai/ai.h"
#include "core/core_field.h"
#include "core/frame_request.h"
#include "core/sequence_generator.h"

using namespace std;

typedef long long ll;

class UauaAI : public AI {
private:
public:
  UauaAI(int argc, char* argv[])
      : AI(argc, argv, "uaua") {
  }
  virtual ~UauaAI() {}

  virtual DropDecision think(int frameId,
                             const CoreField& f,
                             const KumipuyoSeq& seq,
                             const PlayerState& me,
                             const PlayerState& enemy,
                             bool fast) const override {
    UNUSED_VARIABLE(me);
    UNUSED_VARIABLE(enemy);
    UNUSED_VARIABLE(fast);
    return calc(frameId, f, me, enemy, seq, 2);
  }

  const int64_t minScore = -1145141919;

  int calcHakkaY(const CoreField& f) {
    // f.countConnectedPuyosMax4()
    for (int y = FieldConstant::HEIGHT; y >= 1; y--) {
      for (int x = 1; x <= FieldConstant::WIDTH; x++) {
        if (!f.isEmpty(x, y) && f.countConnectedPuyosMax4(x, y) >= 4) {
          return y;
        }
      }
    }
    return 0;
  }

  struct Pack {
    int64_t score;
    int chains;
    int hakkaY;
    Pack() : score(0), chains(0), hakkaY(0) {}
    Pack(int64_t score, int chains, int hakkaY)
        : score(score),
          chains(chains),
          hakkaY(hakkaY) {
    }
  };

  struct State {
    CoreField f;
    int turn;
    Decision d;
    Pack p;
    bool dame;
    bool hakka;
    State() : State({}, 0, {}) {}
    State(const CoreField& f, int turn, const Pack& pack)
        : State(f, turn, pack, false) {
    }
    State(const CoreField& f, int turn, const Pack& pack, bool dame)
        : State(f, turn, pack, {}, dame) {
    }
    State(const CoreField& f, int turn, const Pack& pack, const Decision& d, bool dame)
        : f(f), turn(turn), d(d), p(pack), dame(dame), hakka(false) {
    }

    State next(const CoreField& f, const Pack& pack) {
      State s(f, turn+1, pack, d, dame);
      s.hakka = hakka;
      return s;
    }

    bool operator<(const State& e) const {
      if (turn == e.turn) {
        return p.score < e.p.score;
      }
      return turn > e.turn;
    }
  };

  tuple<int, int, int> calcCon(const CoreField& f) const {
    int lji, tate3, yoko3;
    lji = tate3 = yoko3 = 0;
    for (int x = 1; x < FieldConstant::WIDTH; x++) {
      for (int y = 1; y <= FieldConstant::HEIGHT; y++) {
        if (f.isNormalColor(x,y)){
          PuyoColor u,l,d,r,m;
          m = f.color(x,y);
          u = f.color(x,y-1);
          d = f.color(x,y+1);
          r = f.color(x+1,y);
          l = f.color(x-1,y);
          if (m == u && m == d) tate3 += 1;
          if (m == l && m == r) yoko3 += 1;
          if ((m == u && m == l) ||
              (m == l && m == d) ||
              (m == d && m == r) ||
              (m == r && m == u)) lji += 1;
        }
      }
    }
    return make_tuple(tate3, yoko3, lji);
  }

  typedef function<Pack (const CoreField&, const State& s)> Eval;

  KumipuyoSeq genKumipuyoSequence(int k) const {
    auto&& seq = generateRandomSequenceWithSeed(114514);
    return seq.subsequence(3, k);
  }

  struct SearchResult {
    Decision d;
    int64_t score;
    State state;
    string msg;
    SearchResult() {}
    SearchResult(Decision d, int64_t score, const State& state, const string& msg)
        : d(d), score(score), state(state), msg(msg) {}
  };

  SearchResult nobasiBeamSearch(const CoreField& f, const KumipuyoSeq& nexts, const Eval& eval) const {
    priority_queue<State> q;
    const int BEAM_WIDTH = 10;
    int lim[111]{};
    int64_t best = minScore+1;
    int64_t dameBest = 0;
    Decision d(3, 0);
    State p, bp;
    int depth = nexts.size();
    string msg;

    q.push(State(f, 0, Pack(minScore, 0, 0)));

    const auto& nobasiCallback = [&p, &q, &eval](const RefPlan& plan) {
      if (plan.field().height(3) >= 12) return;
      const CoreField ff(plan.field());
      const RensaResult& res = plan.rensaResult();
      if (res.chains >= 10) {
        // State s(ff, p.turn+1, p.p, p.d, true);
        State s = p.next(ff, p.p);
        if (p.turn == 0) {
          s.hakka = plan.isRensaPlan();
        }
        q.push(s);
        return;
      }
      const Pack pack = eval(ff, p);
      State s = p.next(ff, pack);
      if (p.turn == 0) {
        s.d = plan.decisions().front();
        s.hakka = plan.isRensaPlan();
      }
      q.push(s);
    };
    
    while (!q.empty()) {
      // fprintf(stderr, "size: %zd\n", q.size());
      p = q.top();
      q.pop();
      if (lim[p.turn]++ >= BEAM_WIDTH) {
        continue;
      }
      
      if (p.turn >= depth) {
        // fprintf(stderr, "score: %" PRId64 "\n", p.score);
        // const Pack pack = eval(p.f, p);
        if (best < p.p.score/*pack.score*/) {
          bp = p;
          msg = p.hakka ? "hakka!" : "nanimonashi";
          best = p.p.score;
          d = p.d;
        }
        if (p.dame) {
          if (dameBest < p.p.score) {
            dameBest = p.p.score;
          }
        }
        continue;
      }

      if (p.dame) {
        State s = p.next(p.f, p.p);
        q.push(s);
        continue;
      }
      
      KumipuyoSeq seq;
      if (p.turn < nexts.size()) {
        seq = nexts.subsequence(p.turn, 1);
      }
      
      Plan::iterateAvailablePlans(p.f, seq, 1, nobasiCallback);
    }
    
    return SearchResult(d, best * 100000 + dameBest, bp, msg);
  }
  
  DropDecision nobasi(const CoreField& f, const KumipuyoSeq& nexts,int depth) const {
    UNUSED_VARIABLE(depth);
    KumipuyoSeq s(nexts);
    SearchResult res = nobasiBeamSearch(f, s, [this, &s](const CoreField& pf, const State& p) {
        if (p.turn < s.size()) {
          RensaResult res;
          int hakkaY;
          int64_t score = minScore;
          const auto callback = [&score, &hakkaY, &res, this](CoreField&& f, const ColumnPuyoList&) {
            int tHakkaY = calcHakkaY(f);
            RensaResult tRes = f.simulate();
            // fprintf(stderr, "%d %d\n", tRes.chains, tHakkaY);
            if (score < tRes.chains*100 + tHakkaY) {
              score = tRes.chains*100 + tHakkaY;
              res = tRes;
              hakkaY = tHakkaY;
            }
          };
          bool prohibits[FieldConstant::WIDTH]{};
          // RensaDetector::detectByDropStrategy(pf, prohibits, PurposeForFindingRensa::FOR_FIRE, 2, 13, callback);
          RensaDetector::detectByFloatStrategy(pf, prohibits, 2, 13, callback);
          
          int sa = 0;
          int ma = 0, mi = 114514;
          for (int x = 1; x <= FieldConstant::WIDTH-1; x++) {
            // sa += abs(pf.height(x)-pf.height(x+1));
            ma = max(ma, pf.height(x));
            mi = min(mi, pf.height(x));
          }
          sa = abs(ma-mi);
          auto con = calcCon(pf);
          // UNUSED_VARIABLE(con);
          return Pack(res.chains*100 + hakkaY*2 - sa, res.chains, hakkaY);
        } else {
          // SearchResult nobasiBeamSearch(const CoreField& f, const KumipuyoSeq& nexts, const Eval& eval) const {
        }
      });
    
    return DropDecision(res.d,
                        " score: " + to_string(res.score) +
                        " chains: " + to_string(res.state.p.chains) +
                        " hakka: " + to_string(res.state.p.hakkaY) +
                        "\n" + res.msg);
  }
  
  DropDecision calc(int frameId,
                    const CoreField& f,
                    const PlayerState& me,
                    const PlayerState& enemy,
                    const KumipuyoSeq& nexts,
                    int depth) const {
    UNUSED_VARIABLE(frameId);
    UNUSED_VARIABLE(me);
    UNUSED_VARIABLE(enemy);
    UNUSED_VARIABLE(depth);
    return nobasi(f, nexts, depth);
  }
};

int main(int argc, char* argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  /*
  UauaAI ua(argc, argv);
  
  CoreField f("   R  "
              "   BY "
              "   BY "
              "R  RB "
              "Y  GYB"
              "YRYBRG"
              "YRRYYY"
              "BBBRRR");

  fprintf(stderr, "%" PRId64 "\n", ua.nobasiEval(f));
  */
  
  UauaAI(argc, argv).runLoop();

  return 0;
}
