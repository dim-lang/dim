// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include <ncurses.h>

namespace fastype {

Term::Term() {
  auto log = fastype::LogManager::getLogger("Term");
  F_DEBUGF(log, "{} {}", "INFOF", 2);
  F_INFO(log, "INFO");
  initscr();
  printw("Hello Terminal!!!\nPress any key to quit...");
  refresh();
  getch();
  endwin();
}

std::shared_ptr<Term> Term::open() { return std::shared_ptr<Term>(nullptr); }

} // namespace fastype
