#ifndef CPU_UAUA_AI_H_
#define CPU_UAUA_AI_H_

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
#include <unordered_set>
#include <vector>

#include "base/base.h"
#include "base/strings.h"
#include "core/plan/plan.h"
#include "core/rensa/rensa_detector.h"
#include "core/rensa/rensa_detector_strategy.h"
#include "core/client/ai/ai.h"
#include "core/core_field.h"
#include "core/frame_request.h"
#include "core/kumipuyo_seq_generator.h"

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
    State(n, f, std::numeric_limits<Score>::min()) {
    }
    State(int n, const CoreField& f, const Score& score) :
        State(n, f, score, Decision()){
    }
    State(int n, const CoreField& f, const Score& score, const Decision& d) :
        n(n), f(f), score(score), d(d) {
    }
    bool operator<(const State& e) const {
      return score < e.score;
    }
  };

  int getChains(const CoreField& f) const {
    bool prohibits[FieldConstant::MAP_WIDTH]{};
    int maxChains = 0;
    const auto callback = [&maxChains](CoreField&& f, const ColumnPuyoList&) {
      int chains = f.simulateFast();
      maxChains = std::max(maxChains, chains);
    };
    RensaDetector::detectByDropStrategy(f, prohibits,
                                        PurposeForFindingRensa::FOR_FIRE,
                                        2, 13, callback);
    return maxChains;
  }

  Score nokosi(const CoreField& /*f*/) const {
    return 0;
  }

  Score nobasi(const CoreField& f) const {
    const int SEQUENCE_LENGTH = 5;
    const int TURN = 5;
    std::vector<int> chains(TURN);
    for (int ii = 0; ii < TURN; ii++) {
      const KumipuyoSeq& seq = KumipuyoSeqGenerator::generateRandomSequence(SEQUENCE_LENGTH);
      std::vector<State> curStates;
      int maxChains = 0;
      const int BEAM_WIDTH = 10;
      std::unordered_set<int64_t> used;

      curStates.emplace_back(0, f, getChains(f));
      used.insert(f.hash());

      for (int ii = 0; ii < seq.size(); ii++) {
        std::vector<State> nextStates;
        int sz = std::min<int>(BEAM_WIDTH, curStates.size());
        // for (const State& p : curStates) {
        for (int i = 0; i < sz; i++) {
          const State& p = curStates[i];
          Plan::iterateAvailablePlans(
              p.f,
              seq.subsequence(p.n, 1),
              1,
              [this, &p, &nextStates, &used](const RefPlan& plan) {
                if (plan.isRensaPlan()) {
                  return;
                }
                const CoreField f(plan.field());
                const int64_t h = f.hash();
                if (used.find(h) == used.end()) {
                  nextStates.emplace_back(p.n+1, f, getChains(f));
                  used.insert(h);
                }
              });
        }
        // std::cerr << "nobasi states: " << nextStates.size() << std::endl;
        std::sort(nextStates.rbegin(), nextStates.rend());
        // nextStates.resize(BEAM_WIDTH);
        curStates = std::move(nextStates);
      }

      int sz = std::min<int>(BEAM_WIDTH, curStates.size());
      // for (const State& p : curStates) {
      for (int i = 0; i < sz; i++) {
        const State& p = curStates[i];
        if (maxChains < p.score) {
          maxChains = p.score;
        }
      }
      /*
      for (const State& p : curStates) {
        if (maxChains < p.score) {
          maxChains = p.score;
        }
      }
      */
      // maxChains = curStates.front().score;
      
      chains[ii] = maxChains;
    }
    int sum = std::accumulate(chains.begin(), chains.end(), 0);
    return sum;
  }

  virtual DropDecision think(int frameId,
                             const CoreField& f,
                             const KumipuyoSeq& seq_,
                             const PlayerState& me,
                             const PlayerState& enemy,
                             bool fast) const override {
    UNUSED_VARIABLE(frameId);
    UNUSED_VARIABLE(me);
    UNUSED_VARIABLE(enemy);
    UNUSED_VARIABLE(fast);

    if (f.countPuyos() >= 55) {
      const KumipuyoSeq seq = seq_.subsequence(0, std::min(2, seq_.size()));
      Score score = std::numeric_limits<Score>::min();
      Decision d(3,0);
      Plan::iterateAvailablePlans(
            f,
            seq,
            seq.size(),
            [&d, &score](const RefPlan& plan) {
              if (!plan.isRensaPlan()) {
                return;
              }
              RensaResult rr = plan.rensaResult();
              if (score < rr.score) {
                score = rr.score;
                d = plan.decisions().front();
              }
            });
      return DropDecision(d);
    }
    
    std::vector<State> curStates;
    Decision d(3, 0);
    const KumipuyoSeq seq = seq_.subsequence(0, std::min(5, seq_.size()));
    Score score = std::numeric_limits<Score>::min();
    static const int BEAM_WIDTH = 10;
    std::unordered_set<int64_t> used;

    std::cerr << "seq size: " << seq.size() << std::endl;
    
    curStates.emplace_back(0, f);
    used.insert(f.hash());
    
    for (int ii = 0; ii < seq.size(); ii++) {
      std::vector<State> nextStates;
      int sz = std::min<int>(BEAM_WIDTH, curStates.size());
      // for (const State& p : curStates) {
      for (int i = 0; i < sz; i++) {
        const State& p = curStates[i];
        Plan::iterateAvailablePlans(
            p.f,
            seq.subsequence(p.n, 1),
            1,
            [this, &p, &nextStates, &used](const RefPlan& plan) {
              if (plan.isRensaPlan()) {
                return;
              }
              const CoreField f(plan.field());
              const int64_t h = f.hash();
              if (used.find(h) == used.end()) {
                used.insert(h);
                if (p.n == 0) {
                  // nextStates.emplace_back(p.n+1, f, nobasi(f), plan.decisions().front());
                  nextStates.emplace_back(p.n+1, f, getChains(f), plan.decisions().front());
                } else {
                  // nextStates.emplace_back(p.n+1, f, nobasi(f), p.d);
                  nextStates.emplace_back(p.n+1, f, getChains(f), p.d);
                }
              }
            });
      }
      std::sort(nextStates.rbegin(), nextStates.rend());
      std::cerr << "states: " << nextStates.size() << std::endl;
      // nextStates.resize(BEAM_WIDTH);
      curStates = std::move(nextStates);
    }

    //*
    int sz = std::min<int>(BEAM_WIDTH, curStates.size());
    // for (const State& p : curStates) {
    for (int i = 0; i < sz; i++) {
      const State& p = curStates[i];
      // std::cerr << p.score << std::endl;
      int stateScore = nobasi(p.f);
      if (score < stateScore) {
        score = stateScore;
        d = p.d;
      }
    }
    // */

    // score = curStates.front().score;
    // d = curStates.front().d;

    return DropDecision(d, "score: " + std::to_string(score));
  }
};

#endif
