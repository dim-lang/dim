// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "Log.h"
#include "boost/config.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unordered_map>

#if BOOST_WINDOWS
#include "WinTerm.h"
#else
#include "NCursesTerm.h"
#endif // #if BOOST_WINDOWS

namespace fastype {

static std::mutex TermLock;
static std::unordered_map<std::string, std::shared_ptr<Term>> TermMap =
    std::unordered_map<std::string, std::shared_ptr<Term>>();

std::shared_ptr<Term> Term::open(const std::string &termName) {
  std::lock_guard<std::mutex> guard(TermLock);
  if (TermMap.find(termName) == TermMap.end()) {
#if BOOST_WINDOWS
    std::shared_ptr<Term> term = std::shared_ptr<WinTerm>(new WinTerm());
#else
    std::shared_ptr<Term> term =
        std::shared_ptr<NCursesTerm>(new NCursesTerm());
#endif
    TermMap.insert(std::make_pair(termName, term));
  }
  return TermMap[termName];
}

} // namespace fastype
