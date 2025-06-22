#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct Login {
  Login(State &state, Db &db);
  void draw_login();
  State &state;
  Db &db;
  Rectangle usrBox;
  Rectangle passBox;
  Rectangle sendBtn;
  Rectangle usrLabel;
  Rectangle passLabel;
  unsigned int txtLim = 100;
  char usrBuff[100] = {0};
  char passBuff[100] = {0};
  const char *title = "LOGIN";
  float titleFontSize = 40;
  float titleX;
  float titleY = 50;
  float titleWidth;
  std::string login_failed = "";
  bool w_user = true;
  bool w_pass = false;
  bool usrBoxActive = false; // Track which box is active
  bool passBoxActive = false;
};
