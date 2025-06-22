#pragma once

#include "chatroom.h"
#include "constants.h"
#include "login.h"
#include "menu.h"
#include "signup.h"
#include "start.h"
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Gui {
  Gui();
  void run();
  Font font;
  State state;
  std::string room_name;
  void run_start();
};
