#include "signup.h"
#include "theme.h"

// -------------- helpers ------------------------------------------------
static inline void drawHeading(Font f, const char *txt, int y, int size,
                               Color c) {
  float w = MeasureTextEx(f, txt, (float)size, 0).x;
  DrawTextEx(f, txt, {(WIDTH - w) * 0.5f, (float)y}, (float)size, 0, c);
}

/*----------------------------------------------------------------------*/

Signup::Signup(State &s, Font &f, Db &db) : state_{s}, font_{f}, db_{db} {
  const float cx = WIDTH * 0.5f;
  const float topY = 220.0f;

  constexpr float boxW = 400, boxH = 48;
  userBox_ = {cx - boxW / 2, topY, boxW, boxH};
  passBox_ = {cx - boxW / 2, topY + 80, boxW, boxH};
  sendBtn_ = {cx + boxW / 2 - 120, topY + 80 + 60, 120, 46};
}

void Signup::draw() {
  // ---------------- heading -----------------------------------------
  drawHeading(font_, "Sign Up", 80, Theme::HeadingSize, Theme::Accent);

  // ---------------- labels ------------------------------------------
  // const int labelSize = 20;
  DrawTextEx(font_, "Username", {userBox_.x, userBox_.y - Theme::body - 8},
             (float)Theme::body, 0, Theme::Fg);

  DrawTextEx(font_, "Password", {passBox_.x, passBox_.y - Theme::body - 8},
             (float)Theme::body, 0, Theme::Fg);

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

  bool send = GuiButton(sendBtn_, "Send") || IsKeyPressed(KEY_ENTER);

  if (send && user_[0] && pass_[0]) {
    if (db_.isUnique(user_)) {
      state_ = MENU;
      db_.createUser(user_, pass_);
      std::memset(pass_, 0, sizeof(pass_));
    } else {
      error_ = "Sign Up failed";
    }
  }

  if (!error_.empty()) {
    DrawTextEx(font_, error_.c_str(), {passBox_.x, passBox_.y + 70},
               Theme::body, 0, RED);
  }
}
