#pragma once
#include "constants.h"
#include "raygui.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Menu {
  Menu(State &state, Font &font);
  void draw_menu(std::string &room_name);
  const char *title = "Main Menu";
  State &state;
  Font font;
  std::vector<std::string> rooms;
  std::vector<Rectangle> recs;
  int titleFontSize = 40;
  int titleWidth;
  float titleX;
  float titleY = 50;
};
