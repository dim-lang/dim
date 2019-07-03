// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "File.h"
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

static WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);
  return local_win;
}

static void destroy_win(WINDOW *local_win) {
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}

void NCursesTerm::show(const std::string &fileName) {
  std::shared_ptr<File> f = File::open(fileName);

  F_DEBUGF(log, "fileName: {}", fileName);

  WINDOW *my_win;
  int startx, starty, width, height;
  int ch;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  height = 5;
  width = 10;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;
  printw("Press F1 to exit");
  refresh();

  my_win = create_newwin(height, width, starty, startx);
  while ((ch = getch()) != KEY_F(1)) {
    F_DEBUGF(log, "loop - ch:{} starty:{} startx:{} height:{} width:{}", ch,
             starty, startx, height, width);
    switch (ch) {
    case KEY_LEFT:
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, --startx);
      break;
    case KEY_RIGHT:
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, ++startx);
      break;
    case KEY_UP:
      destroy_win(my_win);
      my_win = create_newwin(height, width, --starty, startx);
      break;
    case KEY_DOWN:
      destroy_win(my_win);
      my_win = create_newwin(height, width, ++starty, startx);
      break;
    }
  }

  endwin();
}

} // namespace fastype
