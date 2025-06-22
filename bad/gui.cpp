#include "gui.h"

Gui::Gui()
    : db_("../Db/chat.db"),

      font_(LoadFontEx("../data/FiraCode-Regular.ttf", Theme::MAX_GLYPH_SIZE, 0,
                       0)),
      start_(state_, font_), login_(state_, font_, db_),
      signup_(state_, font_, db_), menu_(state_, font_, db_),
      room_(state_, font_, db_) {
  if (font_.texture.id == 0)
    font_ = GetFontDefault();

  InitWindow(WIDTH, HEIGHT, "Chat App");
  SetTargetFPS(60);

  /* ---------- global theme (one-time) ----------------------------- */
  GuiLoadStyleDefault();
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(Theme::Fg));
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x2E3236FF);
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(Theme::Accent));
  GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
  GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(Theme::Accent));
  SetTextureFilter(font_.texture,
                   TEXTURE_FILTER_BILINEAR); // smooth downsizing
  GuiSetFont(font_);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
}

Gui::~Gui() {
  if (font_.texture.id != GetFontDefault().texture.id)
    UnloadFont(font_);
  CloseWindow();
}

void Gui::run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(Theme::Bg);

    switch (state_) {
    case START:
      start_.draw();
      break;
    case LOGIN:
      login_.draw();
      break;
    case SIGNUP:
      signup_.draw();
      break;
    case MENU:
      if (menu_.draw(currentRoom_))
        room_.open(currentRoom_, login_.user());
      break;
    case ROOM:
      room_.draw();
      break;
    default:
      break;
    }

    EndDrawing();
  }
}
