// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include <cstdio>
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

Term::Term() {}

Term::~Term() {
  endwin();

  auto log = fastype::LogManager::getLogger("Term");
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
  while (std::fgets(lineBuf, lineSize, fp)) {
    printw(lineBuf);
  }
  refresh();
  std::free(lineBuf);
  fclose(fp);
}

std::shared_ptr<Term> Term::open(const std::string &termName) {
  std::lock_guard<std::mutex> guard(TermLock);
  if (TermMap.find(termName) == TermMap.end()) {
    std::shared_ptr<Term> term = std::shared_ptr<Term>();
    TermMap.insert(std::make_pair(termName, term));
  }
  return TermMap[termName];
}

} // namespace fastype
