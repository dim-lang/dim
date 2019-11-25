// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Filer.h"
#include "Logging.h"
#include "SmartPointer.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("Interpreter", "[Interpreter]") {
  SECTION("Interpret LexerTest1.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/InterpreterTest1.fast"));
    fastype::Sptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    fastype::Sptr<fastype::Parser> parser(new fastype::Parser(lexer));
    fastype::Sptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    REQUIRE(interpreter->interpret() == -9LL);
  }
  SECTION("Interpret LexerTest2.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/InterpreterTest2.fast"));
    fastype::Sptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    fastype::Sptr<fastype::Parser> parser(new fastype::Parser(lexer));
    fastype::Sptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    REQUIRE(interpreter->interpret() == 14LL);
  }
}
