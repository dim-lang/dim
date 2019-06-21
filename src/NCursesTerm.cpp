// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "Log.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unordered_map>

namespace fastype {

NCursesTerm::NCursesTerm()
    : Term(), log(fastype::LogManager::getLogger("NCursesTerm")) {
  F_DEBUG(log, "Construct");
}

NCursesTerm::~NCursesTerm() { F_DEBUG(log, "Destruct"); }

void NCursesTerm::show(const std::string &fileName) {
  int row, col, y, x;

  initscr();
  getmaxyx(stdscr, row, col);
  F_DEBUGF(log, "row:{}, col:{}, LINES:{}", row, col, LINES);
  y = 0;
  x = 0;
  mvprintw(y, x, "(%d,%d)", y, x);
  y = row;
  x = 0;
  mvprintw(y - 1, x, "(%d-1,%d)", y, x);
  y = 0;
  x = col;
  mvprintw(y, x - 10, "(%d,%d-10)", y, x);
  y = row / 2;
  x = col / 2;
  mvprintw(y, x, "(%d,%d) (row:%d,col:%d)", y, x, row, col);
  y = row;
  x = col;
  mvprintw(y - 1, x - 10, "(%d-1,%d-10)", y, x);
  getch();
  endwin();
}

} // namespace fastype
