// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NcursesTerm.h"
#include "Buffer.h"
#include "Logging.h"
#include "Row.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unordered_map>

namespace fastype {

NcursesTerm::NcursesTerm() : Term() {
  (void)row_;
  (void)col_;
}

NcursesTerm::~NcursesTerm() {}

void NcursesTerm::show(const std::string &fileName) {
  Sptr<Buffer> f = Buffer::open(fileName);
  int ch, row, col, y, x;
  initscr();
  getmaxyx(stdscr, row, col);

  F_INFO("f#count: {}", f->count());
  for (int i = 0; i < f->count(); i++) {
    Row r = f->get(i);
    getyx(stdscr, y, x);
    F_INFO("row:{}", r.toString());
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
