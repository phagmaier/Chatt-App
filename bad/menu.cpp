
#include "menu.h"

// helper – centred heading -------------------------------------------
static inline void drawHeading(Font f, const char *txt, int y, int size,
                               Color c) {
  float w = MeasureTextEx(f, txt, (float)size, 0).x;
  DrawTextEx(f, txt, {(WIDTH - w) * 0.5f, (float)y}, (float)size, 0, c);
}

/*--------------------------------------------------------------------*/

Menu::Menu(State &s, Font &f, Db &db) : state_{s}, font_{f}, db_{db} {
  refreshRooms();
}

void Menu::refreshRooms() {
  rooms_ = db_.get_rooms();

  roomLabels_.clear();
  roomLabels_.reserve(rooms_.size());
  for (const std::string &s : rooms_)
    roomLabels_.push_back(s.c_str());

  rebuildLayout();
}

void Menu::rebuildLayout() {
  cards_.clear();

  const float cx = WIDTH * 0.5f;
  const float firstY = 200.0f;
  const float gapY = Theme::CardH + Theme::Gap;

  for (std::size_t i = 0; i < rooms_.size(); ++i) {
    cards_.push_back(Theme::card(cx, firstY + i * gapY));
  }
}

bool Menu::draw(std::string &outRoom) {
  // ---------- heading -------------------------------------------------
  drawHeading(font_, "Choose a room", 80, Theme::HeadingSize, Theme::Accent);

  // ---------- few rooms → card buttons --------------------------------
  if (rooms_.size() <= 8) {
    for (std::size_t i = 0; i < rooms_.size(); ++i) {
      if (GuiButton(cards_[i], rooms_[i].c_str())) {
        outRoom = rooms_[i];
        state_ = ROOM;
        return true;
      }
    }
    return false; // nothing picked this frame
  }

  // ---------- many rooms → scrollable list view -----------------------
  static int focus = -1;  // keyboard focus index
  static int active = -1; // currently selected row

  Rectangle bounds{WIDTH * 0.5f - 220, 180, 440, 400};

  int sel = GuiListViewEx(bounds,
                          roomLabels_.data(), // const char** items
                          roomLabels_.size(), // item count
                          &listScroll_,       // scroll offset
                          &focus,             // keyboard focus
                          &active);           // active (visual)

  if (sel != -1) // user clicked/enter
  {
    outRoom = rooms_[sel];
    state_ = ROOM;
    return true;
  }

  return false; // no selection yet
}
