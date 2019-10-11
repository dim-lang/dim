// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Term.h"
#include "ConcurrentHashMap.h"
#include "Logging.h"
#include "config/Platform.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef F_PLATFORM_WINDOWS
#include "WinTerm.h"
#else
#include "NcursesTerm.h"
#endif // #ifdef F_PLATFORM_WINDOWS

namespace fastype {

static ConcurrentHashMap<std::string, Sptr<Term>> TermMap;

Sptr<Term> Term::open(const std::string &termName) {
  TermMap.lock();
  if (TermMap.find(termName) == TermMap.end()) {

#if BOOST_WINDOWS
    Sptr<Term> term = Sptr<Term>(new WinTerm());
#else
    Sptr<Term> term = Sptr<Term>(new NcursesTerm());
#endif

    TermMap.insert(std::make_pair(termName, term));
  }
  TermMap.unlock();
  return TermMap[termName];
}

} // namespace fastype
