// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Filer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("Interpreter", "[Interpreter]") {
  SECTION("ScriptTest1.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/ScriptTest1.fast"));
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    interpreter->interpret();
    F_INFO_MSG(interpreter->toString());
  }
  SECTION("ScriptTest2.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/ScriptTest2.fast"));
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::Interpreter> interpreter(
        new fastype::Interpreter(parser));
    interpreter->interpret();
    F_INFO_MSG(interpreter->toString());
  }
}
