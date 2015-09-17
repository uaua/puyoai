#include <gflags/gflags.h>
#include <glog/logging.h>

#include "ai.h"

using namespace std;


int main(int argc, char* argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  
  UauaAI(argc, argv).runLoop();

  return 0;
}
