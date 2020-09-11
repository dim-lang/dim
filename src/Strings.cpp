// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Strings.h"
#include "Log.h"
#include <cstdlib>
#include <cstring>

char *Strings::dup(const char *s, int n) {
  char *ns = (char *)std::malloc(n + 1);
  std::memset(ns, 0, n + 1);
  std::strncpy(ns, s, n);
  return ns;
}

char *Strings::dup(const char *s) { return dup(s, std::strlen(s)); }
