#include <curses.h> // output
#include "argparse_2.5.hpp" // args
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

#include <stdexcept>

#include "definition.hpp"
#include "drop.hpp"

struct winsize w;

void update_size(int sig=0){
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // bind winsize variable
}

int main (int argc, char *argv[]){
  argparse::ArgumentParser arg_parser("rainify", "2.0.0");

  arg_parser.add_argument("--fps")
    .help("set fps (between 10 and 60)")
    .scan<'i', int>()
    .default_value(40);

  arg_parser.add_argument("--nocolor")
    .help("set monochrome")
    .default_value(false)
    .implicit_value(true); // when program runs with --nocolor, then arg_parser["--nocolor"] == true

  
  try {
    arg_parser.parse_args(argc, argv);
  }
  catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << arg_parser;
    exit(1);
  }


  int FPS;
  float CYCLE_DURATION;
  {
    auto possible_fps = arg_parser.get<int>("--fps"); // the default value is 40
    if (possible_fps >= 10 && possible_fps <= 60){
      FPS = possible_fps;
      CYCLE_DURATION = 1000.0 / FPS;
    }else{
      std::cerr << "--fps must be between 10 and 60";
      exit(1);
    }
  }

  // for FPS
  auto start_cycle = std::chrono::system_clock::now();
  auto end_cycle = std::chrono::system_clock::now();
  // *******
  update_size(); // load size into w
  std::signal(SIGWINCH, update_size); // bind signal window change to function update_size
  std::setlocale(LC_ALL, ""); // for wchar_t works fine with ncurses
  initscr();   // init ncurses 
  curs_set(0); // cursor invisible
  noecho();    // input invisible

  srand(time(NULL));

  if (arg_parser["--nocolor"] == false){ // if (!arg_parser["--nocolor"]) doesn`t work
    Drop::setup_color();
  }

  std::vector<Drop> drops;
  for(int i=0; i<100; i++){
    drops.push_back({w, FPS});
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
