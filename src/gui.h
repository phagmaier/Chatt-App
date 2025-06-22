#pragma once

#include "chatroom.h"
#include "constants.h"
#include "login.h"
#include "menu.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Gui {
  Gui();
  Font font;
  State state;
  std::string room_name;
  void run();
};
