// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Strings.h"
#include "Exception.h"
#include <cstdlib>
#include <cstring>

char *Strings::dup(const char *s, int n) {
  if (!s || n <= 0) {
    return nullptr;
  }
  char *r = (char *)std::malloc(n + 1);
  EX_ASSERT(r, "r must not be null");
  std::memset(r, 0, n + 1);
  std::strncpy(r, s, n);
  return r;
}

char *Strings::dup(const char *s) { return dup(s, (int)std::strlen(s)); }
