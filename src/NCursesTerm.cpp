// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "Log.h"
#include "unicode/ustring.h"
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
  F_DEBUGF(log, "fileName: {}", fileName);

  int ch, prev = EOF, row, col;
  int y, x;
  FILE *fp = std::fopen(fileName.data(), "r");

  initscr();
  getmaxyx(stdscr, row, col);
  F_DEBUGF(log, "start loop, fp:{}", (void *)fp);
  while ((ch = fgetc(fp)) != EOF) {
    getyx(stdscr, y, x);
    F_DEBUGF(log, "loop, ch:{} prev:{} y:{} x:{} row:{} col:{}", ch, prev, y, x,
             row, col);
    if (y == (row - 1)) {
      F_DEBUG(log, "loop - 1");
      printw("<-Press Any Key->");
      getch();
      clear();
      move(0, 0);
    }
    if (prev == '/' && ch == '*') {
      F_DEBUG(log, "loop - 2");
      attron(A_BOLD);
      getyx(stdscr, y, x);
      move(y, x - 1);
      printw("%c%c", '/', ch);
    } else {
      F_DEBUG(log, "loop - 3");
      printw("%c", ch);
    }
    refresh();
    if (prev == '*' && ch == '/') {
      F_DEBUG(log, "loop - 4");
      attroff(A_BOLD);
    }
    prev = ch;
  }
  F_DEBUGF(log, "end loop, fp:{}", (void *)fp);
  getch();
  endwin();
  fclose(fp);
}

} // namespace fastype
