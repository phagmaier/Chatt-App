#pragma once
#include "constants.h"
#include "db.h"
#include "raygui.h"
#include "theme.h"
#include <cstring>
#include <raylib.h>
#include <vector>

class ChatRoom {
public:
  ChatRoom(State &state, Font &font, Db &db);

  // called after MENU chooses a room; reloads history
  void open(const std::string &roomName, const char *user);

  // call every frame
  void draw();
  // inline const char *user() { return user_; }

private:
  // helpers
  void drawHistory() const;
  void pushWrapped(const std::string &line);
  float textWidth(const char *txt) const;

  // refs
  State &state_;
  Font &font_;
  Db &db_;

  // geometry
  Rectangle historyBox_;
  Rectangle inputBox_;
  Rectangle sendBtn_;
  Rectangle backBtn_;

  // state
  std::vector<std::string> history_;
  std::string room_;
  const char *user_ = nullptr;
  int roomId_ = -1;

  // typing buffer
  static constexpr int InLim = 256;
  char input_[InLim + 1]{};

  // wrapping
  int colLim_ = 0; // chars per line inside historyBox_
  int rowLim_ = 0; // visible rows
};
