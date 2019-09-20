// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "Buffer.h"
#include "Logging.h"
#include "Row.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ncurses.h>
#include <string>
#include <unordered_map>

namespace fastype {

NCursesTerm::NCursesTerm() : Term() {
  (void)row_;
  (void)col_;
}

NCursesTerm::~NCursesTerm() {}

void NCursesTerm::show(const std::string &fileName) {
  std::shared_ptr<Buffer> f = Buffer::open(fileName);
  int ch, row, col, y, x;
  initscr();
  getmaxyx(stdscr, row, col);

  F_DEBUGF("f#count: {}", f->count());
  for (int i = 0; i < f->count(); i++) {
    Row r = f->get(i);
    getyx(stdscr, y, x);
    F_DEBUGF("row:{}", r.toString());
    for (int j = 0; j < r.str().size() - 1; j++) {
      ch = (int)r.str()[j];
      printw("%c", ch);
      refresh();
    }
  }
  getch();
  endwin();
}

} // namespace fastype
