#include "menu.h"

Menu::Menu(State &state, Font &font, Db &db)
    : font{font}, state{state}, db{db} {
  titleWidth = MeasureText(title, titleFontSize);
  titleX = (float)WIDTH / 2.0f - (float)titleWidth / 2.0f;
  titleY = 50;

  rooms = db.get_rooms();
  std::cout << "PRINTING ROOMS\n";
  for (std::string &str : rooms) {
    std::cout << str << "\n";
  }

  float totalHeight =
      rooms.size() * BUTTON_HEIGHT +
      (rooms.size() > 0 ? (rooms.size() - 1) * BUTTON_SPACING : 0);
  float startY = (HEIGHT - totalHeight) / 2.0f;
  float startX = (WIDTH - BUTTON_WIDTH) / 2.0f;

  for (size_t i = 0; i < rooms.size(); ++i) {
    Rectangle btn = {startX, startY + i * (BUTTON_HEIGHT + BUTTON_SPACING),
                     (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT};
    recs.push_back(btn);
  }
}

void Menu::draw_menu(std::string &room_name) {
  DrawText(title, titleX, titleY, titleFontSize, RED);

  for (size_t i = 0; i < rooms.size(); ++i) {
    if (GuiButton(recs[i], rooms[i].c_str())) {
      room_name = rooms[i];
      state = ROOM;
    }
  }
}
