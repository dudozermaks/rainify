#pragma once
#include <vector>

struct cords{
  float x,y;
};
const int FPS = 40;
const float CYCLE_DURATION = 1000.0/FPS;

const std::vector<wchar_t> drop_shapes = {L'┃', L'┇', L'┋', L'╹', L'╻', L'│', L'┆', L'┊', L'╵'};
