// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static icu::UnicodeString readFile(const icu::UnicodeString &fileName) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", nullptr, "UTF-8");
  UChar *data = nullptr;
  int length = 4096, t = 0, n = 0;
  REQUIRE(fp);

  do {
    if (data == nullptr || n >= length) {
      length = length * 2;
      data = (UChar *)realloc(data, length);
    }

    t = u_file_read(data + n, length - n, fp);
    n += t;
  } while (t > 0);
  u_fclose(fp);
  fp = nullptr;

  return icu::UnicodeString(data, n);
}

static void readToken(const icu::UnicodeString &data) {
  fastype::Lexer lex(data);
  lex.parse();
  while (true) {
    fastype::Sptr<fastype::Token> t = lex.read();
    REQUIRE(t.get() != nullptr);
    F_INFO("read Token:{}", t->toString());
    if (t->isEof()) {
      REQUIRE(t == fastype::Token::T_EOF);
      break;
    }
  }
}

TEST_CASE("Lexer", "[Lexer]") {
  SECTION("LexerTest1.fast") {
    icu::UnicodeString data =
        readFile(UNICODE_STRING_SIMPLE("test/script/LexerTest1.fast"));
    REQUIRE(data.length() > 0);
    readToken(data);
  }
  SECTION("LexerTest2.fast") {
    icu::UnicodeString data =
        readFile(UNICODE_STRING_SIMPLE("test/script/LexerTest2.fast"));
    REQUIRE(data.length() > 0);
    readToken(data);
  }
}
