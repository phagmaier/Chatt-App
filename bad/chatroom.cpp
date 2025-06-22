#include "chatroom.h"

/*──────────────── helpers ────────────────────────────────────────────*/
static inline void drawHeading(Font f, const char *txt, int y, int size,
                               Color c) {
  float w = MeasureTextEx(f, txt, (float)size, 0).x;
  DrawTextEx(f, txt, {(WIDTH - w) * 0.5f, (float)y}, (float)size, 0, c);
}
/*──────────────── ctor ───────────────────────────────────────────────*/

ChatRoom::ChatRoom(State &s, Font &f, Db &d) : state_{s}, font_{f}, db_{d} {
  // layout ratios
  constexpr float pad = 32.0f;
  historyBox_ = {pad, 120, WIDTH - 2 * pad, HEIGHT - 300};
  inputBox_ = {pad, HEIGHT - 150, WIDTH - 220 - 2 * pad, 48};
  sendBtn_ = {inputBox_.x + inputBox_.width + 12, inputBox_.y, 120, 48};
  backBtn_ = {pad, 60, 100, 32};

  // pre-compute wrap limits
  float cw = MeasureTextEx(font_, "A", FONTSIZE, 0).x;
  colLim_ = int((historyBox_.width - Theme::PAD) / cw);
  rowLim_ = int(historyBox_.height / (FONTSIZE + Theme::PAD / 2));
}

/*──────────────── public ─────────────────────────────────────────────*/

void ChatRoom::open(const std::string &roomName, const char *user) {
  room_ = roomName;
  user_ = user;
  roomId_ = db_.getRoomId(roomName);
  history_ = db_.get_logs(rowLim_ * 2, roomId_);
  memset(input_, 0, sizeof(input_));
}

/*──────────────── frame ──────────────────────────────────────────────*/

void ChatRoom::draw() {
  // heading + back
  drawHeading(font_, room_.c_str(), 40, Theme::HeadingSize, Theme::Accent);
  if (GuiButton(backBtn_, "Back")) {
    state_ = MENU;
    return;
  }

  // scrolling history
  drawHistory();

  // input
  if (GuiTextBox(inputBox_, input_, InLim, true) &&
      (IsKeyPressed(KEY_ENTER) || GuiButton(sendBtn_, "Send"))) {
    if (input_[0]) {
      std::string line = std::string("[") + user_ + "]: " + input_;
      pushWrapped(line);
      db_.insertMessage(room_, user_, input_);
      std::memset(input_, 0, sizeof(input_));
    }
  }
}

/*──────────────── impl details ───────────────────────────────────────*/

void ChatRoom::drawHistory() const {
  DrawRectangleLinesEx(historyBox_, 2, Theme::Accent);

  int start = (int)history_.size() - rowLim_;
  if (start < 0)
    start = 0;

  Vector2 pos{historyBox_.x + Theme::PAD * 0.5f,
              historyBox_.y + Theme::PAD * 0.5f};

  for (int i = start; i < (int)history_.size(); ++i) {
    float fade = 1.0f - 0.4f * (history_.size() - 1 - i) / rowLim_;
    Color c = Fade(Theme::Fg, fade);
    DrawTextEx(font_, history_[i].c_str(), pos, FONTSIZE, 0, c);
    pos.y += FONTSIZE + Theme::PAD / 2;
  }
}

void ChatRoom::pushWrapped(const std::string &s) {
  if ((int)s.size() <= colLim_) {
    history_.push_back(s);
    return;
  }

  for (size_t i = 0; i < s.size(); i += colLim_)
    history_.push_back(s.substr(i, colLim_));
}
