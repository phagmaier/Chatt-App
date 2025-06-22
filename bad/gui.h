
#pragma once
#include "constants.h"
#include "theme.h"

#include "chatroom.h"
#include "db.h"
#include "login.h"
#include "menu.h"
#include "signup.h"
#include "start.h"

struct Gui {
  Gui();
  ~Gui();

  void run(); // main loop

private:
  // shared objects live as members so they out-live run()
  Font font_;
  State state_ = START;
  Db db_;
  Start start_;
  Login login_;
  Signup signup_;
  Menu menu_;
  ChatRoom room_;

  std::string currentRoom_;
};
