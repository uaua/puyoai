#ifndef GUI_PLAYER_UI_H_
#define GUI_PLAYER_UI_H_

#include <memory>
#include <utility>

#include "base/base.h"
#include "core/frame_response.h"
#include "gui/drawer.h"
#include "gui/main_window.h"
#include "gui/screen.h"

class ReplayServer;

class PlayerUi : public Drawer, public MainWindow::EventListener {
 public:
  PlayerUi(ReplayServer*);
  virtual ~PlayerUi() override;

  virtual void onInit() override;
  virtual void draw(Screen*) override;

  virtual void handleEvent(const SDL_Event&) override;
  virtual void handleAfterPollEvent() override;
 private:
  ReplayServer* server_;

 private:
  class Label {
   public:
    Label() {}
    Label(int x, int y, int w, int h) :
        x(x), y(y), w(w), h(h),
        hovered_(false), pressed_(false), clicked_(false),
        mx(0), my(0) {
    }

    SDL_Rect rect() const {
      SDL_Rect r;
      r.x = x;
      r.y = y;
      r.w = w;
      r.h = h;
      return r;
    }

    bool hovered() const { return hovered_; }
    bool pressed() const { return pressed_; }
    bool clicked() const { return clicked_; }

    void onUpdate() {
      int state = SDL_GetMouseState(&mx, &my);
      if (x <= mx && mx <= x+w && y <= my && my <= y+h) {
        hovered_ = true;
      } else {
        hovered_ = false;
      }
      if ((state&SDL_BUTTON(SDL_BUTTON_LEFT)) && hovered_) {
        pressed_ = true;
        clicked_ = false;
      } else {
        if (pressed_ && hovered_) {
          clicked_ = true;
        } else {
          clicked_ = false;
        }
        pressed_ = false;
      }
    }

    std::pair<int, int> getMousePos() const {
      return std::make_pair(mx-x, my-y);
    }

    int x, y, w, h;

   private:
    bool hovered_;
    bool pressed_;
    bool clicked_;

    int mx, my;
  };

  std::unique_ptr<Label> seekbar, stateButton, thinkButton;
  std::vector<FrameResponse> data[2];
};

#endif
