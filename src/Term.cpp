// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include <cstdlib>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unordered_map>

namespace fastype {

static std::mutex TermLock;
static std::unordered_map<std::string, std::shared_ptr<Term>> TermMap =
    std::unordered_map<std::string, std::shared_ptr<Term>>();

Term::Term() {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  getmaxyx(stdscr, ty, tx);

  auto log = fastype::LogManager::getLogger("Term");
  F_DEBUGF(log, "initscr, raw, keypad, noecho, getmaxyx({}, {})", ty, tx);
}

Term::~Term() {
  endwin();

  auto log = fastype::LogManager::getLogger("Term");
  F_DEBUG(log, "endwin");
}

void Term::show(const std::string &fileName) {
  printw("Please type, press Q to quit...\n");
}

std::shared_ptr<Term> Term::open(const std::string &termName) {
  std::lock_guard<std::mutex> guard(TermLock);
  if (TermMap.find(termName) == TermMap.end()) {
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new Term());
    TermMap.insert(std::make_pair(termName, term));
  }
  return TermMap[termName];
}

} // namespace fastype
