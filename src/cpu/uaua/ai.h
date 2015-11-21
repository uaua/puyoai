#ifndef CPU_UAUA_AI_H_
#define CPU_UAUA_AI_H_

#include <inttypes.h>
#include <cstdio>
#include <cmath>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
<<<<<<< HEAD
=======
#include <memory>
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
#include <queue>
#include <random>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "base/base.h"
#include "base/strings.h"
<<<<<<< HEAD
#include "core/algorithm/plan.h"
#include "core/algorithm/rensa_detector.h"
#include "core/algorithm/rensa_detector_strategy.h"
#include "core/algorithm/puyo_possibility.h"
#include "core/client/ai/ai.h"
#include "core/core_field.h"
#include "core/frame_request.h"
#include "core/kumipuyo_seq_generator.h"

class UauaAI : public AI {
private:
public:
=======
#include "core/plan/plan.h"
#include "core/rensa/rensa_detector.h"
#include "core/rensa/rensa_detector_strategy.h"
#include "core/client/ai/ai.h"
#include "core/core_field.h"
#include "core/field_pretty_printer.h"
#include "core/frame_request.h"
#include "core/kumipuyo_seq_generator.h"
// #include "solver/puyop.h"

namespace {
std::string makePuyopURL(const KumipuyoSeq& seq, const std::vector<Decision>& decisions) {
  static const char ENCODER[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]";
  std::stringstream ss;

  ss << "http://www.puyop.com/s/_";

  for (int i = 0; i < seq.size(); ++i) {
    const Kumipuyo& kp = seq.get(i);
    int d = 0;
    switch (kp.axis) {
      case PuyoColor::RED:
        d += 0;
        break;
      case PuyoColor::GREEN:
        d += 1 * 5;
        break;
      case PuyoColor::BLUE:
        d += 2 * 5;
        break;
      case PuyoColor::YELLOW:
        d += 3 * 5;
        break;
      default:
        CHECK(false);
    }

    switch (kp.child) {
      case PuyoColor::RED:
        d += 0;
        break;
      case PuyoColor::GREEN:
        d += 1;
        break;
      case PuyoColor::BLUE:
        d += 2;
        break;
      case PuyoColor::YELLOW:
        d += 3;
        break;
      default:
        CHECK(false);
    }

    if (i < int(decisions.size())) {
      int h = (decisions[i].x << 2) + decisions[i].r;
      d |= h << 7;
    }

    ss << ENCODER[d & 0x3F] << ENCODER[(d >> 6) & 0x3F];
  }

  return ss.str();
}
}

class UauaAI : public AI {
 private:
 public:
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
  UauaAI(int argc, char* argv[])
      : AI(argc, argv, "uaua") {
  }
  virtual ~UauaAI() {}

  typedef int64_t Score;

<<<<<<< HEAD
=======
  class list {
   public:
    std::unique_ptr<list> l;
    Decision val;
    list() {
      l = nullptr;
    }
    list(list* n, Decision v): l(n) {
      val = v;
    }
  };

>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
  struct State {
    int n;
    CoreField f;
    Score score;
    Decision d;
<<<<<<< HEAD
    State() : State(0, CoreField()) {
    }
    State(int n, const CoreField& f) :
    State(n, f, std::numeric_limits<Score>::min()) {
=======
    int max_chains;
    list *l;
    State() : State(0, CoreField()) {
    }
    State(int n, const CoreField& f) :
        State(n, f, std::numeric_limits<Score>::min()) {
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
    }
    State(int n, const CoreField& f, const Score& score) :
        State(n, f, score, Decision()){
    }
    State(int n, const CoreField& f, const Score& score, const Decision& d) :
<<<<<<< HEAD
        n(n), f(f), score(score), d(d) {
=======
        n(n), f(f), score(score), d(d), max_chains(0), l(nullptr) {
    }
    State(int n, const CoreField& f, const Score& score, const Decision& d, int max_chains, list* l) :
        n(n), f(f), score(score), d(d), max_chains(max_chains), l(l) {
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
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

<<<<<<< HEAD
=======
  int calcFirePos(const CoreField& f) const {
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

  std::pair<int, int> getChainsWithFirePos(const CoreField& f) const {
    bool prohibits[FieldConstant::MAP_WIDTH]{};
    int maxChains = 0;
    int firePos = 0;
    const auto callback = [this, &maxChains, &firePos](CoreField&& f, const ColumnPuyoList&) {
      int pos = calcFirePos(f);
      int chains = f.simulateFast();
      // maxChains = std::max(maxChains, chains);
      if (maxChains == chains && firePos < pos) {
        firePos = pos;
      }
      if (maxChains < chains) {
        maxChains = chains;
        firePos = pos;
      }
    };
    RensaDetector::detectByDropStrategy(f, prohibits,
                                        PurposeForFindingRensa::FOR_FIRE,
                                        2, 13, callback);
    return std::make_pair(maxChains, firePos);
  }

>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
  Score nokosi(const CoreField& /*f*/) const {
    return 0;
  }

  Score nobasi(const CoreField& f) const {
    const int SEQUENCE_LENGTH = 5;
<<<<<<< HEAD
    std::vector<int> chains(SEQUENCE_LENGTH);
    for (int ii = 0; ii < 5; ii++) {
=======
    const int TURN = 5;
    std::vector<int> chains(TURN);
    for (int ii = 0; ii < TURN; ii++) {
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
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

<<<<<<< HEAD
      /*
      for (const State& p : curStates) {
=======
      int sz = std::min<int>(BEAM_WIDTH, curStates.size());
      // for (const State& p : curStates) {
      for (int i = 0; i < sz; i++) {
        const State& p = curStates[i];
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
        if (maxChains < p.score) {
          maxChains = p.score;
        }
      }
<<<<<<< HEAD
      */
      maxChains = curStates.front().score;
=======
      /*
        for (const State& p : curStates) {
        if (maxChains < p.score) {
        maxChains = p.score;
        }
        }
      */
      // maxChains = curStates.front().score;
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
      
      chains[ii] = maxChains;
    }
    int sum = std::accumulate(chains.begin(), chains.end(), 0);
    return sum;
  }

<<<<<<< HEAD
=======
  std::string genUrl(const CoreField& f) const {
    std::string url = "http://1st.geocities.jp/mattulwan/puyo_simulator/?";
    for (int y = CoreField::HEIGHT+1; y >= 1; y--) {
      for (int x = 1; x <= CoreField::WIDTH; x++) {
        int c = 0;
        char ch;
        while (x+c <= CoreField::WIDTH && f.color(x, y) == f.color(x+c, y)) {
          c += 1;
        }
        switch (f.color(x, y)) {
          case PuyoColor::RED:
            ch = 'b';
            break;
          case PuyoColor::GREEN:
            ch = 'e';
            break;
          case PuyoColor::YELLOW:
            ch = 'd';
            break;
          case PuyoColor::BLUE:
            ch = 'c';
            break;
          case PuyoColor::EMPTY:
            ch = 'a';
            break;
          default:
            ch = '?';
            break;
        }
        url += ch + (c>1 ? std::to_string(c) : "");
        x = x+c-1;
      }
    }
    return url;
  }

  Score nobasiEval(const CoreField& f) const {
    std::pair<int, int> sc = getChainsWithFirePos(f);
    int diff = 0;
    for (int x = 1; x <= FieldConstant::WIDTH-1; x++) {
      diff += std::abs(f.height(x) - f.height(x+1));
    }
    return sc.first*100 + sc.second - diff;
  }

  int diffField(const CoreField& f, const CoreField& g) const {
    int cnt = 0;
    for (int x = 1; x <= FieldConstant::WIDTH; x++) {
      for (int y = 1; y <= 13; y++) {
        if (f.isNormalColor(x, y) && f.color(x, y) == g.color(x,y)) {
          cnt += 1;
        }
      }
    }
    return cnt;
  }

  DropDecision search(const CoreField& f, const KumipuyoSeq& seq) {
    std::vector<State> curStates;
    Decision d(3, 0);
    Score score = std::numeric_limits<Score>::min();
    static const int BEAM_WIDTH = 10000;
    std::unordered_set<int64_t> used;

    std::cerr << "seq size: " << seq.size() << std::endl;
    
    curStates.emplace_back(0, f);
    used.insert(f.hash());
    
    for (int ii = 0; ii < seq.size(); ii++) {
      std::vector<State> nextStates(curStates.begin(), curStates.begin()+std::min<int>(curStates.size(), BEAM_WIDTH));
      int sz = std::min<int>(BEAM_WIDTH, curStates.size());
      // for (const State& p : curStates) {
      for (int i = 0; i < sz; i++) {
        // bool dame = false;
        const State& p = curStates[i];
        /*
        for (int j = 0; j < i && !dame; j++) {
          int cnt = diffField(p.f, curStates[j]2);
          if (std::max(p.f.countPuyos(), curStates[j].f.countPuyos())*1 < cnt*3) {
            dame = true;
          }
        }
        if (dame) {
          if (sz < int(curStates.size())) {
            sz += 1;
          }
          continue;
        }
        */
        if (p.n != ii) continue;
        Plan::iterateAvailablePlans(
            p.f,
            seq.subsequence(p.n, 1),
            1,
            [this, &p, &nextStates, &used](const RefPlan& plan) {
              if (plan.isRensaPlan()) {
                // return;
              }
              const CoreField f(plan.field());
              const int64_t h = f.hash();
              if (used.find(h) == used.end()) {
                used.insert(h);
                int chains = getChains(f);
                int max_chains = std::max(chains, p.max_chains);
                Decision d = plan.decisions().front();
                if (p.n == 0) {
                  // nextStates.emplace_back(p.n+1, f, nobasi(f), plan.decisions().front());
                  nextStates.emplace_back(p.n+1,
                                          f,
                                          nobasiEval(f),
                                          d,
                                          max_chains,
                                          new list(p.l, d));
                } else {
                  // nextStates.emplace_back(p.n+1, f, nobasi(f), p.d);
                  nextStates.emplace_back(p.n+1,
                                          f,
                                          nobasiEval(f),
                                          p.d,
                                          p.max_chains,
                                          new list(p.l, d));
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
    for (int i = 0; i < std::min(1000, sz); i++) {
      const State& p = curStates[i];
      // std::cerr << p.score << std::endl;
      /*
      int stateScore = nobasi(p.f);
      if (score < stateScore) {
        score = stateScore;
        d = p.d;
      }
      */
      // std::cout << p.f.toDebugoString() << std::endl;
      // KumipuyoSeq seq;
      // FieldPrettyPrinter::print(p.f.toPlainField(), seq);
      std::cout << genUrl(p.f) << std::endl;
      std::vector<Decision> decisions;
      list* l = p.l;
      while (l != nullptr) {
        decisions.emplace_back(l->val);
        l = l->l.get();
      }
      // std::cout << decisions.size() << std::endl;
      std::reverse(decisions.begin(), decisions.end());
      std::cout << makePuyopURL(seq, decisions) << std::endl;
    }
    // */

    // score = curStates.front().score;
    // d = curStates.front().d;

    return DropDecision(d, "score: " + std::to_string(score));
  }

>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
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
<<<<<<< HEAD
=======

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
    
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
    std::vector<State> curStates;
    Decision d(3, 0);
    const KumipuyoSeq seq = seq_.subsequence(0, std::min(5, seq_.size()));
    Score score = std::numeric_limits<Score>::min();
    static const int BEAM_WIDTH = 10;
    std::unordered_set<int64_t> used;

<<<<<<< HEAD
    std::cerr << "ita" << std::endl;
=======
    std::cerr << "seq size: " << seq.size() << std::endl;
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
    
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
