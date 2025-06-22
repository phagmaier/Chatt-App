#include "chatroom.h"
#include <raylib.h>

ChatRoom::ChatRoom(State &state, Font &font, Db &db)
    : font{font}, state{state}, chatBox({50, 70, 900, 500}),
      inputBox({50, 600, 800, 40}), sendBtn({860, 600, 90, 40}),
      backBtn({50, 660, 100, 30}), inBuff{(char *)calloc(inMax + 1, 1)},
      db{db} {

  charWidth = MeasureTextEx(font, "a", FONTSIZE, TEXTSPACING).x;
  textHeight = MeasureTextEx(font, "a", FONTSIZE, TEXTSPACING).y;
  row_lim = (int)((chatBox.height - LINE_THICK) / (textHeight + LINE_SPACING));
  col_lim = (int)((chatBox.width - LINE_THICK) / charWidth) - 1;
}

ChatRoom::~ChatRoom() {
  if (inBuff) {
    free(inBuff);
  }
}

void ChatRoom::append_msg(std::string &msg) {
  if (msg.size() <= col_lim) {
    if (save_text(msg)) {
      chats.push_back(msg);
    }
    return;
  }

  int idx = 0;
  while (idx + col_lim < msg.size()) { // Fixed condition
    std::string substr = msg.substr(idx, col_lim);
    if (save_text(substr)) {
      chats.push_back(substr);
    } else {
      // should probably give some error message
      // or do something
      return;
    }
    idx += col_lim;
  }
  if (idx < msg.size()) {
    std::string substr = msg.substr(idx);
    if (save_text(substr)) {
      chats.push_back(substr);
    }
  }
}

void ChatRoom::resize_chats() {
  if (chats.size() >= row_lim * 2) {
    const unsigned int new_size = chats.size() - row_lim;
    std::vector<std::string> new_chats;
    new_chats.reserve(new_size);
    for (unsigned int i = 0; i < new_size; ++i) {
      new_chats.push_back(chats[i]);
    }
    chats = new_chats;
  }
}

bool ChatRoom::save_text(std::string &msg) {
  if (!db.insertMessage(name, usr, msg)) {
    return false;
  }
  resize_chats();
  return true;
}

void ChatRoom::load_text() { chats = db.get_logs(row_lim, room_id); }

void ChatRoom::draw_message_box() {
  DrawRectangleLinesEx(chatBox, LINE_THICK, RAYWHITE);
  const unsigned int size = chats.size();
  unsigned int idx = size <= row_lim ? 0 : size - row_lim;
  Vector2 textPos = {chatBox.x + LINE_THICK, chatBox.y + LINE_THICK};

  for (; idx < size; ++idx) {
    DrawTextEx(font, chats[idx].c_str(), textPos, FONTSIZE, TEXTSPACING,
               RAYWHITE);
    textPos.y += textHeight + LINE_SPACING;
  }
}

void ChatRoom::draw_room() {
  DrawText(name.c_str(), WIDTH / 2 - titleWidth / 2, 20, titleSize, RED);

  // Handle input
  GuiTextBox(inputBox, inBuff, inMax, true);

  // Check for send button or Enter key
  bool shouldSend = GuiButton(sendBtn, "Send") ||
                    IsKeyPressed(KEY_ENTER); // Changed to IsKeyPressed

  if (shouldSend && strlen(inBuff) > 0) {
    std::string msg = "[you]: ";
    msg += inBuff;
    append_msg(msg);
    memset(inBuff, 0, inMax + 1);
  }

  draw_message_box();

  if (GuiButton(backBtn, "Back")) {
    // save_text(); // Save before going back
    state = MENU;
  }
}

void ChatRoom::update(std::string &new_name, const char *new_usr) {
  usr = new_usr;
  room_id = db.getRoomId(new_name);
  chats.clear();
  name = new_name;
  load_text();
  titleWidth = MeasureText(name.c_str(), titleSize);
  memset(inBuff, 0, inMax + 1);
}
