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
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    interpreter->interpret();
  }
  SECTION("Interpret LexerTest2.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/InterpreterTest2.fast"));
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    REQUIRE(interpreter->interpret() == 14LL);
  }
}
