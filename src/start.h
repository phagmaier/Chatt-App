#pragma once
#include "constants.h"
#include "raygui.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct Start {
  Start(State &state, Font &font);
  void draw_start();
  State &state;
  Font &font;
  Rectangle loginBox;
  Rectangle signupBox;
  const char *title = "Welcome to P-Chat";
  float titleFontSize = 40;
  float titleX;
  float titleY = 50;
  float titleWidth;
};
