// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "NCursesTerm.h"
#include "File.h"
#include "Line.h"
#include "Logging.h"
#include <cerrno>
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

NCursesTerm::NCursesTerm() : Term(), Logging() { F_DEBUG("Construct"); }

NCursesTerm::~NCursesTerm() { F_DEBUG("Destruct"); }

void NCursesTerm::show(const std::string &fileName) {
  std::shared_ptr<File> f = File::open(fileName);

  F_DEBUGF("fileName: {}", fileName);
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
  endwin();
}

} // namespace fastype
