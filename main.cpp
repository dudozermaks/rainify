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
// rand
#include <random>

const int FPS = 40;
const float CYCLE_DURATION = 1000.0/FPS;

const size_t drop_shapes_count = 9;
const wchar_t drop_shapes[drop_shapes_count] = {L'┃', L'┇', L'┋', L'╹', L'╻', L'│', L'┆', L'┊', L'╵'};

struct winsize w;

struct cords{
  float x,y;
};

class Drop{
private:
  cords pos;
  float mass;
  // for correct ncurses printing
  // character[0] = character
  // character[1] = L'\0'
  wchar_t character[2];
  void generate(){
    pos.x = rand() % (w.ws_col + 1); // rand % (max - min + 1) + min; min = 0; max = w.ws_col
    pos.y = rand() % (w.ws_row + 1) - w.ws_row;

    double random_ratio = static_cast<double>(rand()) / RAND_MAX;
    mass = 40 * (random_ratio / 2 + 0.5) / FPS; // 40 * (0.5, 1] / FPS
    character[0] = drop_shapes[static_cast<int>(drop_shapes_count - random_ratio * drop_shapes_count)];
    character[1] = L'\0';
  }
public:
  Drop(){
    generate();
  }
  void update(){
    if (pos.y > w.ws_row){
      generate();
    }
    pos.y += mass;
  }
  void draw(){
    mvaddwstr(pos.y, pos.x, character);
  }
};

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
    drops.push_back({});
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
      drop.update();
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
