// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include <cstdlib>
#include <ncurses.h>

namespace fastype {

Term::Term() {
  auto log = fastype::LogManager::getLogger("Term");

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  F_DEBUG(log, "initscr, raw, noecho, keypad");

  printw("Please type, press Q to quit...\n");
  F_DEBUG(log, "refresh");
  refresh();

  while (1) {
    int ch = getch();
    F_DEBUGF(log, "getch: {}", (char)ch);
    if (ch == (int)'Q') {
      endwin();
      exit(0);
    } else {
      printw("%c", ch);
      refresh();
    }
  }
}

std::shared_ptr<Term> Term::open() { return std::shared_ptr<Term>(new Term()); }

} // namespace fastype
