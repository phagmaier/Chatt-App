#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include "theme.h"
#include <cstring>
#include <raylib.h>

class Login {
public:
  Login(State &state, Font &font, Db &db);

  void draw(); // call every frame
  inline const char *user() { return user_; }

private:
  // references to shared objects
  State &state_;
  Font &font_;
  Db &db_;

  // geometry
  Rectangle userBox_;
  Rectangle passBox_;
  Rectangle sendBtn_;

  // data
  static constexpr int Lim = 100;
  char user_[Lim + 1]{};
  char pass_[Lim + 1]{};
  bool focusUser_ = true; // which box has focus?
  bool focusPass_ = false;
  std::string error_;
};
