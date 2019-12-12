// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/SymbolTableBuilder.h"
#include "Filer.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

TEST_CASE("SymbolTableBuilder", "[SymbolTableBuilder]") {
  SECTION("SymbolTableBuilder InterpreterTest1.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/InterpreterTest1.fast"));
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::SymbolTableBuilder> interpreter(
        new fastype::SymbolTableBuilder(parser));
    interpreter->interpret();
    F_INFO_MSG(interpreter->toString());
  }
  SECTION("SymbolTableBuilder InterpreterTest2.fast") {
    icu::UnicodeString text = fastype::Filer::readAll(
        UNICODE_STRING_SIMPLE("test/script/InterpreterTest2.fast"));
    std::shared_ptr<fastype::Lexer> lexer(new fastype::Lexer(text));
    std::shared_ptr<fastype::Parser> parser(new fastype::Parser(lexer));
    std::shared_ptr<fastype::SymbolTableBuilder> interpreter(
        new fastype::SymbolTableBuilder(parser));
    interpreter->interpret();
    F_INFO_MSG(interpreter->toString());
  }
}
