// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "CursesTerm.h"
#include "Buffer.h"
#include "Line.h"
#include "Logging.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unordered_map>

namespace fastype {

CursesTerm::CursesTerm() : Term() {
  (void)row_;
  (void)col_;
}

CursesTerm::~CursesTerm() {}

void CursesTerm::show(const icu::UnicodeString &fileName) {
  std::shared_ptr<Buffer> f = Buffer::open(fileName);
  int ch, row, col, y, x;
  initscr();
  getmaxyx(stdscr, row, col);

  F_INFO("f#count: {}", f->count());
  for (int i = 0; i < f->count(); i++) {
    Line l = f->get(i);
    getyx(stdscr, y, x);
    F_INFO("row:{}", l.toString());
    for (int j = 0; j < l.value().length() - 1; j++) {
      ch = (int)l.value()[j];
      printw("%c", ch);
      refresh();
    }
  }
  getch();
  endwin();
}

} // namespace fastype
