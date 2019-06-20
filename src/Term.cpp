// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include <ncurses.h>

namespace fastype {

Term::Term() {
  auto log = fastype::LogManager::getLogger("Term");
  F_DEBUG(log, "initscr");
  initscr();
  F_DEBUG(log, "printw");
  printw("Hello Terminal!!!\nPress any key to quit...");
  F_DEBUG(log, "refresh");
  refresh();
  F_DEBUG(log, "getch");
  getch();
  F_DEBUG(log, "endwin");
  endwin();
}

std::shared_ptr<Term> Term::open() { return std::shared_ptr<Term>(new Term()); }

} // namespace fastype
