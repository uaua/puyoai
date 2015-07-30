#include "AI.h"

string genUrl(const CoreField& f)
{
  string url = "http://1st.geocities.jp/mattulwan/puyo_simulator/?";
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
      url += ch + (c>1 ? to_string(c) : "");
      x = x+c-1;
    }
  }
  return url;
}

int main(int argc, char* argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  UauaAI ua(argc, argv);
  
  CoreField f("");
  map<int, vector<CoreField>> rensaCount;
  const KumipuyoSeq seq = generateRandomSequence();

  // fprintf(stderr, "%" PRId64 "\n", ua.nobasiEval(f));

  ua.nobasi(f, seq.subsequence(50), seq.size(), rensaCount);

  for (auto&& r : rensaCount) {
    cout << r.first << ": " << r.second.size() << endl;
    // cout << r.second.front().toDebugString() << endl;
    for (auto&& f : r.second) {
      cout << genUrl(f) << endl;
      cout << f.toDebugString() << endl;
    }
  }

  return 0;
}
