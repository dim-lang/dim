// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Strings.h"
#include "Log.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <sstream>
#include <utility>

char *Strings::duplicate(const char *s, int n) {
  if (!s || n <= 0) {
    return nullptr;
  }
  char *r = (char *)std::malloc(n + 1);
  LOG_ASSERT(r, "r must not be null");
  std::memset(r, 0, n + 1);
  std::strncpy(r, s, n);
  return r;
}

char *Strings::duplicate(const char *s) {
  return duplicate(s, (int)std::strlen(s));
}

bool Strings::startWith(const std::string &s,
                        const std::vector<std::string> &prefixList) {
  return std::all_of(
      prefixList.begin(), prefixList.end(), [&](const std::string &prefix) {
        return s.length() >= prefix.length() && s.find(prefix) == 0;
      });
}

bool Strings::startWith(const std::string &s,
                        const std::vector<char> &prefixList) {
  return std::all_of(prefixList.begin(), prefixList.end(),
                     [&](const char &prefix) {
                       return s.length() >= 1 && s.find(prefix) == 0;
                     });
}

bool Strings::endWith(const std::string &s,
                      const std::vector<std::string> &postfixList) {
  return std::all_of(
      postfixList.begin(), postfixList.end(), [&](const std::string &postfix) {
        return s.length() >= postfix.length() && s.find(postfix) == 0;
      });
}

bool Strings::endWith(const std::string &s,
                      const std::vector<char> &postfixList) {
  return std::all_of(postfixList.begin(), postfixList.end(),
                     [&](const char &postfix) {
                       return s.length() >= 1 && s.find(postfix) == 0;
                     });
}

std::string Strings::replace(const std::string &s, const std::string &from,
                             const std::string &to) {
  std::stringstream ss;
  int i = 0;
  while (i < (int)s.length()) {
    if (s.length() - i >= from.length() &&
        std::strncmp(s.c_str() + i, from.c_str(), from.length())) {
      ss << to;
      i += from.length();
    } else {
      ss << s[i++];
    }
  }
  return ss.str();
}

std::string Strings::replace(const std::string &s, char from, char to) {
  std::string r(s);
  std::replace(r.begin(), r.end(), from, to);
  return r;
}

std::string Strings::tuncate(const std::string &s, int n, bool exhaust) {
  return tuncateRight(tuncateLeft(s, n, exhaust), n, exhaust);
}

std::string Strings::tuncateLeft(const std::string &s, int n, bool exhaust) {
  return s.length() > n ? s.substr(n, s.length() - n) : (exhaust ? "" : s);
}

std::string Strings::tuncateRight(const std::string &s, int n, bool exhaust) {
  return s.length() > n ? s.substr(0, s.length() - n) : (exhaust ? "" : s);
}
