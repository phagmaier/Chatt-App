#include "login.h"

// -------------- helpers ------------------------------------------------
static inline void drawHeading(Font f, const char *txt, int y, int size,
                               Color c) {
  float w = MeasureTextEx(f, txt, (float)size, 0).x;
  DrawTextEx(f, txt, {(WIDTH - w) * 0.5f, (float)y}, (float)size, 0, c);
}

/*----------------------------------------------------------------------*/

Login::Login(State &s, Font &f, Db &db) : state_{s}, font_{f}, db_{db} {
  const float cx = WIDTH * 0.5f;
  const float topY = 220.0f;

  constexpr float boxW = 400, boxH = 48;
  userBox_ = {cx - boxW / 2, topY, boxW, boxH};
  passBox_ = {cx - boxW / 2, topY + 80, boxW, boxH};
  sendBtn_ = {cx + boxW / 2 - 120, topY + 80 + 60, 120, 46};
}

void Login::draw() {
  // ---------------- heading -----------------------------------------
  drawHeading(font_, "Login", 80, Theme::HeadingSize, Theme::Accent);

  // ---------------- labels ------------------------------------------
  const int labelSize = 20;
  DrawTextEx(font_, "Username", {userBox_.x, userBox_.y - labelSize - 8},
             (float)labelSize, 0, Theme::Fg);

  DrawTextEx(font_, "Password", {passBox_.x, passBox_.y - labelSize - 8},
             (float)labelSize, 0, Theme::Fg);

  // ---------------- text boxes --------------------------------------
  if (GuiTextBox(userBox_, user_, Lim, focusUser_)) {
    focusUser_ = true;
    focusPass_ = false;
  }

  if (GuiTextBox(passBox_, pass_, Lim, focusPass_)) {
    focusUser_ = false;
    focusPass_ = true;
  }

  if (IsKeyPressed(KEY_TAB)) { // swap focus
    focusUser_ = !focusUser_;
    focusPass_ = !focusPass_;
  }

  // ---------------- submit ------------------------------------------
  bool send = GuiButton(sendBtn_, "Send") || IsKeyPressed(KEY_ENTER);

  if (send && user_[0] && pass_[0]) {
    if (db_.verifyLogin(user_, pass_)) {
      state_ = MENU;
      std::memset(pass_, 0, sizeof(pass_)); // hygiene
    } else {
      error_ = "Login failed";
    }
  }

  if (!error_.empty()) {
    DrawTextEx(font_, error_.c_str(), {passBox_.x, passBox_.y + 70}, 20, 0,
               RED);
  }
}
