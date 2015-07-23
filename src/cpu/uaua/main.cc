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
    Pack() : score(0), chains(0) {}
    Pack(int64_t score, int chains)
        : score(score),
          chains(chains) {
    }
  };

  Pack nobasiEval(const CoreField& pf) {
    RensaResult res;
    int hakkaY;
    int64_t score = minScore;
    const auto callback = [&](CoreField&& f, const ColumnPuyoList&) {
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
    RensaDetector::detectByDropStrategy(pf, prohibits, PurposeForFindingRensa::FOR_FIRE, 2, 13, callback);
    // RensaDetector::detectSingle(pf, RensaDetectorStrategy::defaultDropStrategy(), callback);
    
    int sa = 0;
    for (int x = 1; x <= FieldConstant::WIDTH-1; x++) {
      sa += abs(pf.height(x)-pf.height(x+1));
    }
    auto con = calcCon(pf);
    return Pack(res.chains*100 + hakkaY*3 - sa, res.chains);
  }

  struct State {
    CoreField f;
    int turn;
    Decision d;
    Pack p;
    bool dame;
    State() : dame(false) {}
    State(const CoreField& f, int turn, const Pack& pack)
        : State(f, turn, pack, false) {
    }
    State(const CoreField& f, int turn, const Pack& pack, bool dame)
        : f(f), turn(turn), p(pack), dame(dame) {
    }
    bool operator<(const State& e) const {
      return p.score < e.p.score;
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
  
  DropDecision nobasi(const CoreField& f, const KumipuyoSeq& nexts,int depth) const {
    priority_queue<State> q;
    depth = nexts.size();
    const int BEAM_WIDTH = 2000;
    int lim[11]{};
    int64_t best = minScore+1;
    Decision d(3, 0);
    State p;
    q.push(State(f, 0, Pack(minScore, 0)));

    //*
    const auto& nobasiCallback = [&](const RefPlan& plan) {
      if (plan.field().height(3) >= 12) return;
      const Pack pack = nobasiEval(plan.field());
      CoreField ff(f);
      ff.simulate();
      State s(ff, p.turn+1, pack);
      if (p.turn == 0) {
        s.d = plan.decisions().front();
      } else {
        s.d = p.d;
      }
      q.push(s);
    };
    // */

    /*
    const auto& nobasiCallback = [&](const CoreField& f,
                                     const std::vector<Decision>& d,
                                     int numChigiri,
                                     int framesToIgnite,
                                     int lastDropFrames,
                                     bool shouldFire) {
      UNUSED_VARIABLE(numChigiri);
      UNUSED_VARIABLE(framesToIgnite);
      UNUSED_VARIABLE(lastDropFrames);
      UNUSED_VARIABLE(shouldFire);
      const int64_t score = nobasiEval(f);
      CoreField ff(f);
      ff.simulate();
      State s(ff, p.turn+1, score);
      if (p.turn == 0) {
        s.d = d.front();
      } else {
        s.d = p.d;
      }
      q.push(s);
    };
    */
    
    while (!q.empty()) {
      // fprintf(stderr, "size: %zd\n", q.size());
      p = q.top();
      q.pop();
      if (p.turn >= depth) {
        // fprintf(stderr, "score: %" PRId64 "\n", p.score);
        if (best < p.p.score) {
          best = p.p.score;
          d = p.d;
        }
        continue;
      }
      if (p.p.chains >= 9 || p.dame) {
        State s(p.f, p.turn+1, p.p, true);
        s.d = p.d;
        q.push(s);
        continue;
      }
      if (lim[p.turn]++ >= BEAM_WIDTH) {
        continue;
      }
      Plan::iterateAvailablePlans(p.f, nexts.subsequence(p.turn, 1), 1, nobasiCallback);
      // Plan::iterateAvailablePlansWithoutFiring(f, nexts.subsequence(p.turn, 1), 2, nobasiCallback);
    }
    return DropDecision(d, "score: " + to_string(best));
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
