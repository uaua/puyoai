#include "ai.h"

#include <iostream>

<<<<<<< HEAD
=======
#include <gflags/gflags.h>
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
#include <gtest/gtest.h>

using namespace std;

<<<<<<< HEAD
=======
DEFINE_int32(kumipuyo_seed, 1919, "kumipuyo seed");

>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
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
<<<<<<< HEAD
=======
  void search(const CoreField& f, const KumipuyoSeq& seq) {
    UauaAI uaua(0, nullptr);
    uaua.search(f, seq);
  }
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
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
<<<<<<< HEAD
  (void)d;
  cout << d.decision() << "\n" << d.message() << endl;
}
=======
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
>>>>>>> dc9dba4dfde742da83192447ff1315c6f7b485bd
