
#include "start.h"

// helper: draw centred heading -----------------------
static inline void drawHeading(Font f, const char *txt, int y, int size,
                               Color c) {
  float w = MeasureTextEx(f, txt, (float)size, 0).x;
  DrawTextEx(f, txt, {(WIDTH - w) * 0.5f, (float)y}, (float)size, 0, c);
}

/*----------------------------------------------------*/

Start::Start(State &s, Font &f) : state_{s}, font_{f} {
  float cx = WIDTH * 0.5f;
  float firstY = 220.0f;

  loginCard_ = Theme::card(cx, firstY);
  signupCard_ = Theme::card(cx, firstY + Theme::CardH + Theme::Gap);
}

void Start::draw() {
  // background has already been cleared in Gui::run() loop

  drawHeading(font_, "Welcome to P-Chat", 80, Theme::HeadingSize,
              Theme::Accent);

  bool goLogin = GuiButton(loginCard_, "Log in");
  bool goSignup = GuiButton(signupCard_, "Sign up") || IsKeyPressed(KEY_ENTER);

  if (goLogin)
    state_ = LOGIN;
  if (goSignup)
    state_ = SIGNUP;
}
