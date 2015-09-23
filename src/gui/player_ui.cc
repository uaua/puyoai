#include <iostream>

#include "gui/player_ui.h"
#include "duel/replay_server.h"

using namespace std;

PlayerUi::PlayerUi(ReplayServer* server) {
  server_ = server;
}

PlayerUi::~PlayerUi() {
}

void PlayerUi::onInit() {
}

void PlayerUi::draw(Screen* screen) {
  SDL_Surface* surface = screen->surface();
  const int SEEKBAR_WIDTH =screen->mainBox().w();
  const int SEEKBAR_HEIGHT = 15;

  if (!seekbar) {
    seekbar.reset(new Label(screen->mainBox().sx,
                            screen->mainBox().dy+30,
                            SEEKBAR_WIDTH,
                            SEEKBAR_HEIGHT));
  }
  if (!stateButton) {
    stateButton.reset(new Label(screen->mainBox().sx,
                                screen->mainBox().dy+30+SEEKBAR_HEIGHT+5,
                                40, 40));
  }
  /*
  if (!thinkButton) {
    thinkButton.reset(new Label(screen->mainBox().sx+40+5,
                                screen->mainBox().dy+30+SEEKBAR_HEIGHT+5,
                                40, 40));
  }
  */

  SDL_Rect r;

  if (seekbar) {
    r = seekbar->rect();
    SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 64, 64, 64, 0));
    
    r.w = SEEKBAR_WIDTH * (double(server_->frameId())/server_->totalFrames());
    SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
  }

  if (stateButton) {
    r = stateButton->rect();
    if (server_->pause()) {
      SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 64, 64, 64, 0));
    } else {
      SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    }
  }

  if (thinkButton) {
    r = thinkButton->rect();
    if (server_->thinking()) {
      SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 64, 64, 64, 0));
    } else {
      SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    }
  }
}

void PlayerUi::handleEvent(const SDL_Event& /*event*/) {
}

void PlayerUi::handleAfterPollEvent() {
  if (seekbar) {
    seekbar->onUpdate();

    if (seekbar->pressed()) {
      const pair<int, int> pos = seekbar->getMousePos();
      int frameId = server_->totalFrames() * (double(pos.first)/seekbar->w);
      server_->setFrameId(frameId);
    }
  }

  if (stateButton) {
    stateButton->onUpdate();

    if (stateButton->clicked()) {
      server_->togglePlayState();
    }
  }

  if (thinkButton) {
    thinkButton->onUpdate();

    if (thinkButton->clicked()) {
      server_->think(data);
      if (data[0].size() > 0) {
        GameState gs = server_->nowGameState();
        PlayerGameState pgs = gs.playerGameState(0);
        CoreField f(pgs.field);
        CoreField g(f);
        cout << "decision: " << data[0].front().decision.toString() << endl;
        cout << "drop: " << g.dropKumipuyo(data[0].front().decision, pgs.kumipuyoSeq.front()) << endl;
        cout << "eq: " << (f == g) << endl;
        cout << g.toDebugString() << endl;
      }
    }
  }
}
