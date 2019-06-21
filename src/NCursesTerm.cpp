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

NCursesTerm::NCursesTerm() {}

NCursesTerm::~NCursesTerm() {
  endwin();

  auto log = fastype::LogManager::getLogger("NCursesTerm");
  F_DEBUG(log, "Destruct");
}

void Term::show(const std::string &fileName) {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  getmaxyx(stdscr, ty, tx);

  auto log = fastype::LogManager::getLogger("Term");
  F_DEBUGF(log, "show getmaxyx({}, {})", ty, tx);

  FILE *fp = fopen(fileName.data(), "r");

  size_t lineSize = 1024;
  char *lineBuf = (char *)std::malloc(lineSize);
  for (int i = 0; i < ty - 2 && std::fgets(lineBuf, lineSize, fp); i++) {
    lineBuf[std::min<uint64_t>(tx - 1, std::strlen(lineBuf))] = '\0';
    printw(lineBuf);
  }
  printw("press any to quit...\n");
  refresh();
  getch();
  std::free(lineBuf);
  fclose(fp);
}

} // namespace fastype
