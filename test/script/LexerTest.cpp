// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static char *readFile(const std::string &fileName, int &length) {
  FILE *fp = std::fopen(fileName.data(), "rb");
  if (fp == nullptr) {
    F_ERRORF("file:{} not found", fileName);
    exit(EXIT_FAILURE);
  }

  char *data = nullptr;
  length = 1024;
  int64_t t = 0, n = 0;

  do {
    if (data == nullptr || n >= length) {
      length = length * 2;
      data = (char *)realloc(data, length);
    }

    t = (int64_t)fread(data + n, 1, length - n, fp);
    n += t;
  } while (t > 0);
  std::fclose(fp);
  fp = nullptr;

  length = n;
  return data;
}

TEST_CASE("Lexer", "[Lexer]") {
  SECTION("Simple Lexer") {
    int len = 0;
    char *data = readFile("test/script/LexerTest1.fast", len);
    REQUIRE(data != nullptr);
    REQUIRE(len > 0);

    fastype::Lexer lex(data, &std::free);
    while (true) {
      std::shared_ptr<fastype::Token> t = lex.read();
      F_INFOF("Lexer Token t:{}", t->toString());
      if (t == fastype::Token::EOF_) {
        break;
      }
    }
  }
}
