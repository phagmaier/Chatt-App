
#pragma once
#include "constants.h"
#include "raygui.h"
#include "theme.h"

class Start {
public:
  Start(State &state, Font &font);

  void draw(); // renamed to draw() – shorter

private:
  State &state_;
  Font &font_;
  Rectangle loginCard_;
  Rectangle signupCard_;
};
