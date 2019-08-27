// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "ConcurrentHashMap.h"
#include "Logging.h"
#include "config/Platform.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#ifdef F_PLATFORM_WINDOWS
#include "WinTerm.h"
#else
#include "NCursesTerm.h"
#endif // #ifdef F_PLATFORM_WINDOWS

namespace fastype {

static ConcurrentHashMap<std::string, std::shared_ptr<Term>> TermMap;

std::shared_ptr<Term> Term::open(const std::string &termName) {
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
