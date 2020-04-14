// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * For example, write code:
 * enumgen(MyType, 1, A, B, C);
 *
 * will generate:
 *
 * enum class MyType { A, B, C };
 *
 * EnumMyType {
 * public:
 *  static const std::vector<MyType> &values();
 *  static int value(const MyType &e);
 *  static std::string name(const MyType &e);
 *  static MyType fromValue(int value);
 *  static MyType fromName(const std::string &name);
 * };
 *
 */

namespace detail {
long long enumCount();
}

#define ENUM_GEN(x, i, ...)                                                    \
  enum class x { __VA_ARGS__ };                                                \
  class BOOST_PP_CAT(Enum, x) {                                                \
    struct BOOST_PP_CAT(EnumImpl, x) {                                         \
      int value;                                                               \
      std::string name;                                                        \
    };                                                                         \
    static std::vector<x> vec_;                                                \
    static std::unordered_map<x, BOOST_PP_CAT(EnumImpl, x)> map_;              \
                                                                               \
  public:                                                                      \
    static const std::vector<x> &values() {                                    \
      static std::vector<x> v = std::initializer_list(__VA_ARGS__);            \
      return v;                                                                \
    }                                                                          \
    static int value(const x &e) {                                             \
      static std::vector<x> v = std::initializer_list(__VA_ARGS__);            \
      for (int p = 0; p < (int)v.size(); p++) {                                \
        if (v[p] == e) {                                                       \
          return i + p;                                                        \
        }                                                                      \
      }                                                                        \
      LOG_ASSERT(false, "enum {}:{} not found", BOOST_PP_STRINGIZE(x),         \
                 BOOST_PP_STRINGIZE(e));                                       \
    }                                                                          \
    static std::string name(const x &e) {                                      \
      static std::vector<x> v = std::initializer_list(__VA_ARGS__);            \
      for (int p = 0; p < (int)v.size(); p++) {                                \
        if (v[p] == e) {                                                       \
          return i + p;                                                        \
        }                                                                      \
      }                                                                        \
      LOG_ASSERT(false, "enum {}:{} not found", BOOST_PP_STRINGIZE(x),         \
                 BOOST_PP_STRINGIZE(e));                                       \
    }                                                                          \
  }
