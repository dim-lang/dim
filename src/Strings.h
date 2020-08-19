// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <string>
#include <vector>

class Strings {
public:
  static char *duplicate(const char *s);
  static char *duplicate(const char *s, int n);

  static bool startWith(const std::string &s,
                        const std::vector<std::string> &prefixList);
  static bool startWith(const std::string &s,
                        const std::vector<char> &prefixList);

  static bool endWith(const std::string &s,
                      const std::vector<std::string> &postfixList);
  static bool endWith(const std::string &s,
                      const std::vector<char> &postfixList);

  static std::string replace(const std::string &s, const std::string &from,
                             const std::string &to);
  static std::string replace(const std::string &s, char from, char to);

  static std::string tuncate(const std::string &s, int n, bool exhaust = true);
  static std::string tuncateLeft(const std::string &s, int n,
                                 bool exhaust = true);
  static std::string tuncateRight(const std::string &s, int n,
                                  bool exhaust = true);
};
