#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include "theme.h"
#include <cstring>
#include <raylib.h>

class Signup {
public:
  Signup(State &state, Font &font, Db &db);

  void draw();

private:
  State &state_;
  Font &font_;
  Db &db_;

  Rectangle userBox_;
  Rectangle passBox_;
  Rectangle sendBtn_;

  static constexpr int Lim = 100;
  char user_[Lim + 1]{};
  char pass_[Lim + 1]{};
  bool focusUser_ = true;
  bool focusPass_ = false;
  std::string error_;
};
