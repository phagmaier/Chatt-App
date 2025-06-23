#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include "theme.h"
#include <cstring>
#include <raylib.h>

class Login {
public:
  Login(State &state, Font &font_header, Font &font_text, Db &db);
  void draw(std::string &usrName);

private:
  State &state_;
  Font &font_header;
  Font &font_text;
  Db &db_;

  Rectangle userBox_;
  Rectangle passBox_;
  Rectangle sendBtn_;
  Rectangle backBtn_;
  float textW;

  static constexpr int Lim = 100;
  char user_[Lim + 1]{};
  char pass_[Lim + 1]{};
  bool focusUser_ = true;
  bool focusPass_ = false;
  std::string error_;
};
