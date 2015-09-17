#include "ai.h"

#include <iostream>

#include <gtest/gtest.h>

using namespace std;

class AITest : public testing::Test {
 protected:
  int nobasiEval(const CoreField& f) {
    UauaAI uaua(0, nullptr);
    return uaua.nobasi(f);
  }
  DropDecision think(const CoreField& f) {
    UauaAI uaua(0, nullptr);
    const KumipuyoSeq& seq = KumipuyoSeqGenerator::generateRandomSequenceWithSeed(2, 1919);
    return uaua.think(0, f, seq, {}, {}, false);
  }
};

TEST_F(AITest, nobasiCheck)
{
    CoreField f;
    int res = nobasiEval(f);
    cout << res << endl;
}

TEST_F(AITest, thinkCheck)
{
  CoreField f;
  DropDecision d = think(f);
  (void)d;
  cout << d.decision() << "\n" << d.message() << endl;
}
