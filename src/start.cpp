
#include "start.h"
#include "theme.h"

Start::Start(State &s, Font &f) : state_{s}, font_{f} {
  float cx = WIDTH * 0.5f;
  float firstY = 220.0f;

  loginCard_ = Theme::card(cx, firstY);
  signupCard_ = Theme::card(cx, firstY + Theme::CardH + Theme::Gap);
  textW = MeasureTextEx(f, "WELCOME TO P-CHAT", Theme::HeadingSize, 2).x;
}

void Start::draw() {
  // background has already been cleared in Gui::run() loop

  // drawHeading(font_, "Welcome to P-Chat", 80, Theme::HeadingSize,
  //             Theme::Accent);

  DrawTextEx(font_, "WELCOME TO P-CHAT", {WIDTH / 2.0f - textW / 2.0f, 80.f},
             Theme::HeadingSize, 2, Theme::Accent);
  bool goLogin = GuiButton(loginCard_, "Log in");
  bool goSignup = GuiButton(signupCard_, "Sign up") || IsKeyPressed(KEY_ENTER);

  if (goLogin)
    state_ = LOGIN;
  if (goSignup)
    state_ = SIGNUP;
}
