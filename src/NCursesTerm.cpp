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

NCursesTerm::NCursesTerm() : Term(), Logging("NCursesTerm") {
  F_DEBUG("Construct");
}

NCursesTerm::~NCursesTerm() { F_DEBUG("Destruct"); }

void NCursesTerm::show(const std::string &fileName) {
  std::shared_ptr<File> f = File::open(fileName);

  F_DEBUGF("fileName: {}", fileName);
  int ch, row, col, y, x;
  FILE *fp = std::fopen(fileName.data(), "r");
  if (fp == nullptr) {
    F_ERRORF("open fail, fileName: {} errno: {}", fileName, errno);
    return;
  }

  initscr();
  getmaxyx(stdscr, row, col);

  for (int i = 0; i < f->count(); i++) {
    Line &l = f->get(i);
    getyx(stdscr, y, x);
    printw("%c", ch);
  }
}

} // namespace fastype
