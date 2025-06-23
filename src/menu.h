#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include "theme.h"
#include <raylib.h>
#include <vector>

class Menu {
public:
  Menu(State &state, Font &font, Db &db);

  // returns true when a room was chosen and writes its name into outRoom
  bool draw(std::string &outRoom);

private:
  void refreshRooms(); // reload DB + recompute geometry
  void rebuildLayout();

  // refs to shared objects
  State &state_;
  Font &font;
  Db &db_;

  // data / layout
  std::vector<std::string> rooms_;
  std::vector<Rectangle> cards_;
  int listScroll_ = 0; // for GuiListView scrolling

  std::vector<const char *> roomLabels_; // <- c-strings for raygui
  float textW;
  Rectangle bounds;
  Rectangle backBtn_;
};
