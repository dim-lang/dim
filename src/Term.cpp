// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "ConcurrentHashMap.h"
#include "Logging.h"
#include "boost/config.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#if BOOST_WINDOWS
#include "WinTerm.h"
#else
#include "NCursesTerm.h"
#endif // #if BOOST_WINDOWS

namespace fastype {

static ConcurrentHashMap<std::string, std::shared_ptr<Term>> TermMap;

std::shared_ptr<Term> Term::create(const std::string &termName) {
  TermMap.lock();
  if (TermMap.find(termName) == TermMap.end()) {

#if BOOST_WINDOWS
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new WinTerm());
#else
    std::shared_ptr<Term> term = std::shared_ptr<Term>(new NCursesTerm());
#endif

    TermMap.insert(std::make_pair(termName, term));
  }
  TermMap.unlock();
  return TermMap[termName];
}

} // namespace fastype
