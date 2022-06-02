#include <curses.h> // output
// FPS
#include <chrono>
#include <thread>
// Terminal size
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
// Signals
#include <csignal>

// for wchar characters
#include <clocale>

#include "definition.hpp"
#include "drop.hpp"

struct winsize w;

void update_size(int sig=0){
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // bind winsize variable
}

int main (int argc, char *argv[]){
  // for FPS
  auto start_cycle = std::chrono::system_clock::now();
  auto end_cycle = std::chrono::system_clock::now();
  // *******
  update_size(); // load size into w
  std::signal(SIGWINCH, update_size); // bind signal window change to function update_size
  std::setlocale(LC_ALL, "");
  initscr();   // init ncurses 
  curs_set(0); // cursor invisible
  noecho();    // input invisible

  srand(time(NULL));

  std::vector<Drop> drops;
  for(int i=0; i<100; i++){
    drops.push_back({w});
  }

  while (true) {
    // for FPS 
    start_cycle = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = start_cycle - end_cycle;

    if (work_time.count() < CYCLE_DURATION){
      std::chrono::duration<double, std::milli> delta_ms(CYCLE_DURATION - work_time.count());
      auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
      std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }
    // *******
    // update
    for(auto& drop : drops){
      drop.update(w);
    }
    // draw
    clear();
    for(auto& drop : drops){
      drop.draw();
    }
    refresh();

    // for FPS
    end_cycle = std::chrono::system_clock::now();
  }
  endwin();
  return 0;
}
