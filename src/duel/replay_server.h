#ifndef DUEL_REPLAY_SERVER_H_
#define DUEL_REPLAY_SERVER_H_

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "core/game_result.h"
#include "core/server/game_state.h"

class ConnectorManager;
class GameStateObserver;
struct FrameResponse;

class ReplayServer {
public:
  ReplayServer(ConnectorManager*, const std::vector<GameState>& gameStates);
  ~ReplayServer();

  // Doesn't take ownership.
  void addObserver(GameStateObserver*);

  bool start();
  void stop();
  void join();

  // This callback should be alive during duel server is alive.
  void setCallbackDuelServerWillExit(std::function<void ()> callback)
  {
    callbackReplayServerWillExit_ = callback;
  }

private:
  struct DuelState;

  void runReplayLoop();

  GameResult runGame(ConnectorManager* manager);

private:
  std::thread th_;
  volatile bool shouldStop_;

  ConnectorManager* manager_;
  std::vector<GameStateObserver*> observers_;
  std::vector<GameState> gameStates_;
  std::function<void ()> callbackReplayServerWillExit_;
};

#endif
