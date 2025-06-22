#pragma once
#include "constants.h"
#include "raygui.h"
// #include "raylib.h"
#include "db.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ChatRoom {
  ChatRoom(State &state, Font &font, Db &db);
  ~ChatRoom();
  int titleSize = 35;
  int titleWidth = 0;
  int inMax = 10000;
  std::string name;
  State &state;
  Font &font;
  std::vector<std::string> chats;
  char *inBuff;
  Rectangle chatBox;
  Rectangle inputBox;
  Rectangle sendBtn;
  Rectangle backBtn;
  float charWidth;
  float textHeight;
  int row_lim;
  int col_lim;
  int room_id;
  Db &db;
  std::string usr;

  void update(std::string &new_name, const char *new_usr);
  void load_text();
  void draw_input();
  void draw_message_box();
  void append_msg(std::string &str);
  void draw_room();
  bool save_text(std::string &msg);
  void resize_chats();
};
