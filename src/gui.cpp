#include "gui.h"

Gui::Gui() : state{START} {}

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
  Start start = Start(state, font);
  Signup signup = Signup(state, db);
  Login login = Login(state, db);
  Menu menu = Menu(state, font, db);
  ChatRoom room = ChatRoom(state, font, db);
  /*
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x33373BFF);   // #33373B – mid-grey
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xE0E0E0FF);   // near-white
  GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x3C9CE8FF); // cyan ring
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x00A8E8FF);  // bright cyan fill
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);  // white text
*/

  while (!WindowShouldClose()) {
    BeginDrawing();
    // ClearBackground(RAYWHITE);
    ClearBackground({24, 26, 27, 255});
    if (state == START) {
      start.draw_start();
    } else if (state == LOGIN) {
      login.draw_login();
    } else if (state == SIGNUP) {
      signup.draw_signup();
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
