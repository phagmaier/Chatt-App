#include "login.h"

Login::Login(State &state, Db &db) : state(state), db(db) {
  titleWidth = MeasureText(title, titleFontSize);
  titleX = (float)WIDTH / 2.0f - (float)titleWidth / 2.0f;

  float boxW = 400;
  float boxH = 40;
  float x = WIDTH / 2.0f - boxW / 2.0f;
  usrBox = {x, 200, boxW, boxH};
  passBox = {x, 300, boxW, boxH};
  sendBtn = {x + boxW - 90, 360, 90, 40};
}

void Login::draw_login() {
  DrawText(title, titleX, titleY, titleFontSize, RED);
  DrawText("Username:", usrBox.x, usrBox.y - 30, 20, DARKGRAY);
  DrawText("Password:", passBox.x, passBox.y - 30, 20, DARKGRAY);

  if (GuiTextBox(usrBox, usrBuff, txtLim, w_user)) {
    w_user = true;
    w_pass = false;
  }
  if (GuiTextBox(passBox, passBuff, txtLim, w_pass)) {
    w_user = false;
    w_pass = true;
  }

  if (IsKeyDown(KEY_TAB)) {
    w_user = !w_user;
    w_pass = !w_pass;
  }

  bool shouldSend = GuiButton(sendBtn, "Send") || IsKeyPressed(KEY_ENTER);
  if (shouldSend && strlen(usrBuff) > 0 && strlen(passBuff) > 0) {
    if (db.verifyLogin(usrBuff, passBuff)) {
      state = MENU;
    } else {
      login_failed = "Login Failed";
    }
  }

  if (!login_failed.empty()) {
    DrawText(login_failed.c_str(), passBox.x, passBox.y + 60, 20, RED);
  }
}
