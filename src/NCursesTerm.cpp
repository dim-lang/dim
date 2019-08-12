// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "Line.h"
#include "Logging.h"
#include "model/File.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ncurses.h>
#include <string>
#include <unordered_map>

namespace fastype {

NCursesTerm::NCursesTerm() : Term(), Logging("NCursesTerm") {}

NCursesTerm::~NCursesTerm() {}

void NCursesTerm::show(const std::string &fileName) {
  std::shared_ptr<File> f = File::open(fileName);
  int ch, row, col, y, x;
  initscr();
  getmaxyx(stdscr, row, col);

  F_DEBUGF("f#count: {}", f->count());
  for (int i = 0; i < f->count(); i++) {
    Line l = f->get(i);
    getyx(stdscr, y, x);
    F_DEBUGF("line#size:{} l:{}", l.size(), l.toString());
    for (int j = 0; j < l.size() - 1; j++) {
      ch = (int)l[j];
      // F_DEBUGF("line[j](int): {}", ch);
      printw("%c", ch);
      refresh();
    }
  }
  getch();
  endwin();
}

} // namespace fastype
