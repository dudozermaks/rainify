#pragma once
#include <curses.h> // output
#include <sys/ioctl.h> // winsize struct
#include "definition.hpp"
// rand
#include <random>
class Drop{
private:
  cords pos;
  float mass;
  unsigned int color_pair;
  // for correct ncurses printing
  // character[0] = character
  // character[1] = L'\0'
  wchar_t character[2];
  void generate(winsize& w){
    pos.x = rand() % (w.ws_col + 1); // rand % (max - min + 1) + min; min = 0; max = w.ws_col
    pos.y = rand() % (w.ws_row + 1) - w.ws_row;

    double random_ratio = static_cast<double>(rand()) / RAND_MAX;
    
    mass = 40 * (random_ratio / 2 + 0.5) / FPS; // 40 * [0.5, 1) / FPS

    character[0] = drop_shapes[static_cast<int>(random_ratio * drop_shapes.size())];
    character[1] = L'\0';

    color_pair = round(random_ratio + 1);
  }
  const int& FPS;
public:
  static void setup_color(){
    if (has_colors() == TRUE){
      start_color();
      init_pair(1, COLOR_BLUE, COLOR_BLACK); 
      init_pair(2, COLOR_CYAN, COLOR_BLACK);
    }
  }
  Drop(winsize& w, int& _FPS) : FPS{_FPS}{
    generate(w);
  }
  void update(winsize& w){
    if (pos.y > w.ws_row){
      generate(w);
    }
    pos.y += mass;
  }
  void draw(){
    attron(COLOR_PAIR(color_pair));
    mvaddwstr(pos.y, pos.x, character);
    attroff(COLOR_PAIR(color_pair));
  }
};
