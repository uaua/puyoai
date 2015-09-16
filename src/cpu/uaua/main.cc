#include <inttypes.h>
#include <cstdio>
#include <cmath>

#include <algorithm>
#include <fstream>
#include <functional>
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
#include "core/kumipuyo_seq_generator.h"

using namespace std;

class UauaAI : public AI {
private:
public:
  UauaAI(int argc, char* argv[])
      : AI(argc, argv, "uaua") {
  }
  virtual ~UauaAI() {}

  typedef int64_t Score;

  struct State {
    int n;
    CoreField f;
    Score score;
    Decision d;
    State() : State(0, CoreField()) {
    }
    State(int n, const CoreField& f) :
        State(n, f, numeric_limits<Score>::min()) {
    }
    State(int n, const CoreField& f, const Score& score) :
        State(n, f, score, Decision()){
    }
    State(int n, const CoreField& f, const Score& score, const Decision& d) :
        n(n), f(f), score(score), d(d) {
    }
    bool operator<(const State& e) const {
      if (n == e.n) {
        return score < e.score;
      }
      return n > e.n;
    }
  };

  int getChains(const CoreField& f) const {
    bool prohibits[FieldConstant::MAP_WIDTH]{};
    int maxChains = 0;
    const auto callback = [&maxChains](CoreField&& f, const ColumnPuyoList&) {
      int chains = f.simulateFast();
      maxChains = max(maxChains, chains);
    };
    RensaDetector::detectByDropStrategy(f, prohibits,
                                        PurposeForFindingRensa::FOR_FIRE,
                                        2, 13, callback);
    return maxChains;
  }

  Score nokosi(const CoreField& f) const {
    return 0;
  }

  Score nobasi(const CoreField& f) const {
    const int SEQUENCE_LENGTH = 5;
    vector<int> chains(SEQUENCE_LENGTH);
    for (int ii = 0; ii < 5; ii++) {
      const KumipuyoSeq& seq = KumipuyoSeqGenerator::generateRandomSequence(SEQUENCE_LENGTH);
      priority_queue<State> q;
      int maxChains = 0;
      int beamWidth[SEQUENCE_LENGTH+1]{};
      const int MAX_BEAM_WIDTH = 10;

      q.emplace(0, f, getChains(f));

      while (!q.empty()) {
        State p = q.top(); q.pop();
        if (beamWidth[p.n] >= MAX_BEAM_WIDTH) {
          continue;
        }
        beamWidth[p.n] += 1;

        if (p.n >= seq.size()) {
          maxChains = p.score;
          continue;
        }
      
        Plan::iterateAvailablePlans(
            p.f,
            seq.subsequence(p.n, 1),
            1,
            [this, &p, &q](const RefPlan& plan) {
              if (plan.isRensaPlan()) {
                return;
              }
              const CoreField f(plan.field());
              q.emplace(p.n+1, f, getChains(f));
            });
      }

      chains[ii] = maxChains;
    }
    int sum = accumulate(chains.begin(), chains.end(), 0);
    return sum;
  }

  virtual DropDecision think(int frameId,
                             const CoreField& f,
                             const KumipuyoSeq& seq_,
                             const PlayerState& me,
                             const PlayerState& enemy,
                             bool fast) const override {
    priority_queue<State> q;
    Decision d(3, 0);
    const KumipuyoSeq seq = seq_.subsequence(0, min(5, seq_.size()));
    Score score = numeric_limits<Score>::min();
    int beamWidth[111]{};
    static const int MAX_BEAM_WIDTH = 10;

    cerr << "ita" << endl;
    
    q.emplace(0, f);
    
    while (!q.empty()) {
      State p = q.top(); q.pop();

      cerr << q.size() << endl;

      if (beamWidth[p.n] >= MAX_BEAM_WIDTH) {
        continue;
      }
      beamWidth[p.n] += 1;
      
      if (p.n >= seq.size()) {
        if (score < p.score) {
          score = p.score;
          d = p.d;
        }
        continue;
      }
      
      Plan::iterateAvailablePlans(
          p.f,
          seq.subsequence(p.n, 1),
          1,
          [this, &p, &q](const RefPlan& plan) {
            if (plan.isRensaPlan()) {
              return;
            }
            const CoreField f(plan.field());
            if (p.n == 0) {
              q.emplace(p.n+1, f, nobasi(f), plan.decisions().front());
            } else {
              q.emplace(p.n+1, f, nobasi(f), p.d);
            }
          });
    }

    cerr << "done: " << score << endl;
    return DropDecision(d, "score: " + to_string(score));
  }
};

int main(int argc, char* argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  
  UauaAI(argc, argv).runLoop();

  return 0;
}
