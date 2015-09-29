#include "ai.h"

#include <iostream>

#include <gflags/gflags.h>
#include <gtest/gtest.h>

using namespace std;

DEFINE_int32(kumipuyo_seed, 1919, "kumipuyo seed");

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
  void search(const CoreField& f, const KumipuyoSeq& seq) {
    UauaAI uaua(0, nullptr);
    uaua.search(f, seq);
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
  cout << d.decision() << "\n" << d.message() << endl;
}

TEST_F(AITest, searchCheck)
{
  CoreField f;
  const KumipuyoSeq& seq = KumipuyoSeqGenerator::generateRandomSequenceWithSeed(50, FLAGS_kumipuyo_seed);
  cerr << "seed: " << FLAGS_kumipuyo_seed << endl;
  search(f, seq);
}

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
