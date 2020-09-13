// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Strings.h"
#include "Log.h"
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>

static std::unordered_map<size_t, std::vector<char *>> StringPool;

char *Strings::dup(const char *s) { return dup(s, (int)std::strlen(s)); }

char *Strings::dup(const char *s, int n) {
  if (!s || n <= 0) {
    return nullptr;
  }
  size_t key = std::hash<std::string>()(std::string(s, n));
  if (StringPool.find(key) == StringPool.end()) {
    std::vector<char *> vec;
    StringPool[key] = std::vector<char *>();
  }
  for (int i = 0; i < (int)StringPool[key].size(); i++) {
    char *data = StringPool[key][i];
    if (std::strlen(data) == n && std::strncmp(data, s, n) == 0) {
      return data;
    }
  }
  char *ndata = (char *)std::malloc(n + 1);
  std::memset(ndata, 0, n + 1);
  std::strncpy(ndata, s, n);
  StringPool[key].push_back(ndata);
  return ndata;
}

// char *Strings::dup(const char *s, int n) {
//   char *ns = (char *)std::malloc(n + 1);
//   std::memset(ns, 0, n + 1);
//   std::strncpy(ns, s, n);
//   return ns;
// }
//
// char *Strings::dup(const char *s) { return dup(s, std::strlen(s)); }
