#include "start.h"

Start::Start(State &state, Font &font) : state(state), font{font} {
  titleWidth = MeasureText(title, titleFontSize);
  titleX = (float)WIDTH / 2.0f - (float)titleWidth / 2.0f;

  float boxW = 100;
  float boxH = 200;
  float spacing = 100;
  float x = WIDTH / 2.0f - boxW / 2.0f;
  loginBox = {x, 200, boxW, boxH};
  signupBox = {x, loginBox.y + spacing + loginBox.height, boxW, boxH};
}

/*
void Start::draw_start() {
  DrawText(title, titleX, titleY, titleFontSize, RED);
  DrawText("LOGIN:", loginBox.x, loginBox.y - 30, 20, DARKGRAY);
  DrawText("Sign Up:", signupBox.x, signupBox.y - 30, 20, DARKGRAY);

  if (GuiButton(loginBox, "Login")) {
    state = LOGIN;
  }

  else if (GuiButton(signupBox, "sign Up") || IsKeyPressed(KEY_ENTER)) {
    state = SIGNUP;
  }
}
*/

// start.cpp

void Start::draw_start() {
  // one-time style init
  // ClearBackground({24, 26, 27, 255});

  DrawTextEx(font, title, {titleX, 80}, titleFontSize, 0, RED);

  const float cardW = 280, cardH = 80;
  const float cardX = WIDTH / 2.0f - cardW / 2.0f;
  const float cardY = 220, gapY = cardH + 36;

  Rectangle loginCard{cardX, cardY, cardW, cardH};
  Rectangle signupCard{cardX, cardY + gapY, cardW, cardH};

  bool goLogin = GuiButton(loginCard, "Log in");
  bool goSignup = GuiButton(signupCard, "Sign up") || IsKeyPressed(KEY_ENTER);

  if (goLogin)
    state = LOGIN;
  if (goSignup)
    state = SIGNUP;
}
