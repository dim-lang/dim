// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Strings.h"
#include "Log.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <list>
#include <sstream>
#include <utility>

static std::unordered_map<size_t, std::list<char *>> StringPool;

static char *cached_string(const char *s, int n) {
  if (!s || n <= 0) {
    return nullptr;
  }
  size_t key = std::hash<std::string>()(std::string(s, n));
  if (StringPool.find(key) != StringPool.end()) {
    StringPool[key] = std::list<char *>();
  }
  for (auto i = StringPool[key].begin(); i != StringPool[key].end(); i++) {
    if (std::strlen(*i) == n && std::strncmp(*i, s, n) == 0) {
      return *i;
    }
  }
  char *ns = new char[n + 1];
  std::memset(ns, 0, n + 1);
  std::memcpy(ns, s, n);
  StringPool[key].push_back(ns);
  return ns;
}

char *Strings::dup(const char *s, int n) { return cached_string(s, n); }

char *Strings::dup(const char *s) { return cached_string(s, std::strlen(s)); }

char *Strings::dup(const std::string &s) {
  return cached_string(s.c_str(), (int)s.length());
}

bool Strings::startWith(const char *s,
                        std::vector<const char *>::const_iterator begin,
                        std::vector<const char *>::const_iterator end) {
  return startWith(s, (int)std::strlen(s), begin, end);
}

bool Strings::startWith(const char *s, int n,
                        std::vector<const char *>::const_iterator begin,
                        std::vector<const char *>::const_iterator end) {
  return std::all_of(begin, end, [&](const char *prefix) {
    size_t pn = std::strlen(prefix);
    return n >= pn && std::strncmp(s, prefix, pn) == 0;
  });
}

bool Strings::startWith(const std::string &s,
                        std::vector<std::string>::const_iterator begin,
                        std::vector<std::string>::const_iterator end) {
  return std::all_of(begin, end, [&](const std::string &prefix) {
    return s.length() >= prefix.length() && s.find(prefix) == 0;
  });
}

bool Strings::startWith(const char *s, std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end) {
  return startWith(s, (int)std::strlen(s), begin, end);
}

bool Strings::startWith(const char *s, int n,
                        std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end) {
  return std::all_of(begin, end,
                     [&](char prefix) { return n >= 1 && s[0] == prefix; });
}

bool Strings::startWith(const std::string &s,
                        std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end) {
  return startWith(s.c_str(), (int)s.length(), begin, end);
}

bool Strings::endWith(const char *s,
                      std::vector<const char *>::const_iterator begin,
                      std::vector<const char *>::const_iterator end) {
  return endWith(s, (int)std::strlen(s), begin, end);
}

bool Strings::endWith(const char *s, int n,
                      std::vector<const char *>::const_iterator begin,
                      std::vector<const char *>::const_iterator end) {
  return std::all_of(begin, end, [&](const char *postfix) {
    size_t pn = std::strlen(postfix);
    return n >= pn && std::strncmp(s + n - pn, postfix, pn) == 0;
  });
}

bool Strings::endWith(const std::string &s,
                      std::vector<std::string>::const_iterator begin,
                      std::vector<std::string>::const_iterator end) {
  return std::all_of(begin, end, [&](const std::string &postfix) {
    return s.length() >= postfix.length() &&
           s.rfind(postfix) == s.length() - postfix.length();
  });
}

bool Strings::endWith(const char *s, std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end) {
  return endWith(s, (int)std::strlen(s), begin, end);
}

bool Strings::endWith(const char *s, int n,
                      std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end) {
  return std::all_of(
      begin, end, [&](char postfix) { return n >= 1 && s[n - 1] == postfix; });
}

bool Strings::endWith(const std::string &s,
                      std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end) {
  return std::all_of(begin, end, [&](char postfix) {
    return s.length() >= 1 && s.rfind(postfix) == s.length() - 1;
  });
}

char *Strings::replace(const char *s, const char *from, const char *to) {
  return replace(s, (int)std::strlen(s), from, (int)std::strlen(from), to,
                 (int)std::strlen(to));
}

char *Strings::replace(const char *s, int n, const char *from, int fromn,
                       const char *to, int ton) {
  if (!s || n <= 0) {
    return (char *)s;
  }
  if (!from || fromn <= 0) {
    return (char *)s;
  }
  std::stringstream ss;
  int i = 0;
  while (i < n) {
    if (n - i >= fromn && std::strncmp(s + i, from, fromn) == 0) {
      if (to && ton > 0) {
        ss << std::string(to, ton);
      }
      i += fromn;
    } else {
      ss << s[i++];
    }
  }
  return dup(ss.str().c_str());
}

char *Strings::replace(const std::string &s, const std::string &from,
                       const std::string &to) {
  return replace(s.c_str(), (int)s.length(), from.c_str(), (int)from.length(),
                 to.c_str(), (int)to.length());
}

char *Strings::replace(const char *s, char from, char to) {
  return replace(s, (int)std::strlen(s), &from, 1, &to, 1);
}

char *Strings::replace(const char *s, int n, char from, char to) {
  return replace(s, n, &from, 1, &to, 1);
}

char *Strings::replace(const std::string &s, char from, char to) {
  return replace(s.c_str(), (int)s.length(), &from, 1, &to, 1);
}

char *Strings::replace(const char *s,
                       const std::unordered_map<char, const char *> &fromto) {
  return replace(s, (int)std::strlen(s), fromto);
}

char *Strings::replace(const char *s, int n,
                       const std::unordered_map<char, const char *> &fromto) {
  if (!s || n <= 0) {
    return (char *)s;
  }
  if (fromto.size() == 0) {
    return (char *)s;
  }
  std::stringstream ss;
  for (int i = 0; i < n; i++) {
    if (fromto.find(s[i]) != fromto.end()) {
      ss << fromto.find(s[i])->second;
    } else {
      ss << s[i];
    }
  }
  return dup(ss.str().c_str());
}

char *Strings::replace(const std::string &s,
                       const std::unordered_map<char, std::string> &fromto) {
  if (s.length() == 0) {
    return (char *)s.c_str();
  }
  if (fromto.size() == 0) {
    return (char *)s.c_str();
  }
  std::stringstream ss;
  for (int i = 0; i < (int)s.length(); i++) {
    if (fromto.find(s[i]) != fromto.end()) {
      ss << fromto.find(s[i])->second;
    } else {
      ss << s[i];
    }
  }
  return dup(ss.str().c_str());
}
