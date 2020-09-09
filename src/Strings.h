// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Strings {
public:
  static char *dup(const char *s);
  static char *dup(const char *s, int n);
  static char *dup(const std::string &s);

  static bool startWith(const char *s,
                        std::vector<const char *>::const_iterator begin,
                        std::vector<const char *>::const_iterator end);
  static bool startWith(const char *s, int n,
                        std::vector<const char *>::const_iterator begin,
                        std::vector<const char *>::const_iterator end);
  static bool startWith(const std::string &s,
                        std::vector<std::string>::const_iterator begin,
                        std::vector<std::string>::const_iterator end);

  static bool startWith(const char *s, std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end);
  static bool startWith(const char *s, int n,
                        std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end);
  static bool startWith(const std::string &s,
                        std::vector<char>::const_iterator begin,
                        std::vector<char>::const_iterator end);

  static bool endWith(const char *s,
                      std::vector<const char *>::const_iterator begin,
                      std::vector<const char *>::const_iterator end);
  static bool endWith(const char *s, int n,
                      std::vector<const char *>::const_iterator begin,
                      std::vector<const char *>::const_iterator end);
  static bool endWith(const std::string &s,
                      std::vector<std::string>::const_iterator begin,
                      std::vector<std::string>::const_iterator end);

  static bool endWith(const char *s, std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end);
  static bool endWith(const char *s, int n,
                      std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end);
  static bool endWith(const std::string &s,
                      std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end);

  static char *replace(const char *s, const char *from, const char *to);
  static char *replace(const char *s, int n, const char *from, int fromn,
                       const char *to, int ton);
  static char *replace(const std::string &s, const std::string &from,
                       const std::string &to);

  static char *replace(const char *s, char from, char to);
  static char *replace(const char *s, int n, char from, char to);
  static char *replace(const std::string &s, char from, char to);

  static char *replace(const char *s,
                       const std::unordered_map<char, const char *> &fromto);
  static char *replace(const char *s, int n,
                       const std::unordered_map<char, const char *> &fromto);
  static char *replace(const std::string &s,
                       const std::unordered_map<char, std::string> &fromto);
};
