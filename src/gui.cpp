#include "gui.h"
#include "chatroom.h"

Gui::Gui() : state{LOGIN} {}

void Gui::run() {
  InitWindow(WIDTH, HEIGHT, "Chat App");
  SetTargetFPS(30);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

  // Try loading custom font with correct path
  // FiraCode-Regular.ttf
  // font = LoadFontEx("../data/FiraCode-VariableFont_wght.ttf", FONTSIZE, 0,
  // 0);

  font = LoadFontEx("../data/FiraCode-Regular.ttf", FONTSIZE, 0, 0);
  if (font.texture.id == 0) {
    std::cout << "Custom font failed to load, using default font" << std::endl;
    font = GetFontDefault(); // Fallback to default font
  } else {
    std::cout << "Custom font loaded successfully" << std::endl;
  }
  Db db = Db("../Db/chat.db");
  Menu menu = Menu(state, font, db);
  ChatRoom room = ChatRoom(state, font, db);
  Login login = Login(state, db);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (state == LOGIN) {
      login.draw_login();
    } else if (state == MENU) {
      menu.draw_menu(room_name);
      if (state == ROOM) {
        room.update(room_name, login.usrBuff);
      }
    } else if (state == ROOM) {
      room.draw_room();
    }

    EndDrawing();
  }

  // Cleanup
  if (font.texture.id != GetFontDefault().texture.id) {
    UnloadFont(font);
  }
  CloseWindow();
}
