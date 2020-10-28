// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "infra/Strings.h"
#include "infra/Log.h"
#include <cstdlib>
#include <cstring>
// #include <sstream>
// #include <unordered_map>
// #include <vector>

// static std::unordered_map<size_t, std::vector<char *>> StringPool;
//
// static void dumpStringPool(const Cowstr &msg) {
//   std::stringstream ss;
//   ss << "string pool size:" << StringPool.size() << "\n";
//   for (auto i = StringPool.begin(); i != StringPool.end(); i++) {
//     size_t key = i->first;
//     std::vector<char *> &vec = i->second;
//     ss << "key:" << key << ", vec.size:" << vec.size() << ", ";
//     for (int j = 0; j < (int)vec.size(); j++) {
//       ss << "[" << (void *)vec[j] << "," << vec[j] << "]";
//       if (j < vec.size() - 1) {
//         ss << ", ";
//       }
//     }
//     ss << "\n";
//   }
//   ss << "\n";
//   LOG_INFO("{}, {}", msg, ss.str());
// }
//
// char *Strings::dup(const char *s) { return dup(s, (int)std::strlen(s)); }
//
// char *Strings::dup(const char *s, int n) {
//   if (!s || n <= 0) {
//     dumpStringPool(fmt::format("invalid parameter s:{}, n:{}", s, n));
//     return nullptr;
//   }
//   size_t key = std::hash<std::string>()(std::string(s, n));
//   if (StringPool.find(key) == StringPool.end()) {
//     std::vector<char *> vec;
//     StringPool[key] = std::vector<char *>();
//   }
//   for (int i = 0; i < (int)StringPool[key].size(); i++) {
//     char *data = StringPool[key][i];
//     if (std::strlen(data) == n && std::strncmp(data, s, n) == 0) {
//       dumpStringPool(fmt::format("find s:{}, n:{} in string pool", s, n));
//       return data;
//     }
//   }
//   char *ndata = (char *)std::malloc(n + 1);
//   std::memset(ndata, 0, n + 1);
//   std::strncpy(ndata, s, n);
//   StringPool[key].push_back(ndata);
//   dumpStringPool(fmt::format("create new s:{}, n:{}", s, n));
//   return ndata;
// }

char *Strings::dup(const char *s, int n) {
  char *ns = (char *)std::malloc(n + 1);
  std::memset(ns, 0, n + 1);
  std::strncpy(ns, s, n);
  return ns;
}

char *Strings::dup(const char *s) { return dup(s, std::strlen(s)); }
