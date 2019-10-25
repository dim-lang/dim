// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "exception/ParseException.h"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <regex>
#include <unicode/uchar.h>

#define F_TO_STRING_TEXT_MAX 64

namespace fastype {

Lexer::Lexer(const icu::UnicodeString &text)
    : queue_(), more_(true), pos_(0), text_(text) {
  F_INFO("Constructor:{}", toString());
}

std::string Lexer::toString() const {
  std::string utf8;
  text_.tempSubString(0, std::min<int>(F_TO_STRING_TEXT_MAX, text_.length()))
      .toUTF8String(utf8);
  return fmt::format("[ @Lexer pos_:{}, more_:{}, text_:{} ]", pos_, more_,
                     utf8);
}

Lexer::~Lexer() {
  while (!queue_.empty()) {
    Sptr<Token> t = queue_.front();
    t.reset();
    queue_.pop_front();
  }
  queue_.clear();
}

Sptr<Token> Lexer::read() {
  if (fillQueue(0)) {
    Sptr<Token> t = queue_.front();
    queue_.pop_front();
    return t;
  } else {
    return Token::T_EOF;
  }
}

Sptr<Token> Lexer::peek(int i) {
  if (fillQueue(i)) {
    return queue_[i];
  } else {
    return Token::T_EOF;
  }
}

bool Lexer::fillQueue(int i) {
  while (i >= (int)queue_.size()) {
    if (!more_) {
      return false;
    }
    parse();
  }
  return true;
}

void Lexer::parse() {
  if (pos_ >= text_.length()) {
    more_ = false;
    return;
  }

  icu::UnicodeString unixLineBreak = UNICODE_STRING_SIMPLE("\n");
  icu::UnicodeString blockCommetEnd = UNICODE_STRING_SIMPLE("*/");

  int i = 0;
  while (i < text_.length()) {
    if (u_isspace(text_.charAt(i))) {
      i += 1;
      continue;
    }
    switch (text_.charAt(i)) {
    case (UChar)'+':
      queue_.push_back(Token::T_ADD);
      i += 1;
      break;
    case (UChar)'-':
      queue_.push_back(Token::T_SUB);
      i += 1;
      break;
    case (UChar)'*': // * or */
      queue_.push_back(Token::T_MUL);
      i += 1;
      break;
    case (UChar)'/': // / or // or /*
    {
      if (i + 1 < text_.length()) {
        switch (text_.charAt(i + 1)) {
        case (UChar)'/': // line comment //
        {
          // find line break from i+1
          int lineEndPos = text_.indexOf(unixLineBreak, i + 1);
          i = lineEndPos + 1;
        } break;
        case (UChar)'*': // block comment /*
        {
          // find end of block comment
          int lineEndPos = text_.indexOf(blockCommetEnd, i + 1);
          i = lineEndPos + 2;
        } break;
        default: {
          std::string _1;
          F_CHECK(false, "Parse Error! i:{}, text_: {}", i,
                  text_
                      .tempSubString(
                          i, std::max<int>(text_.tempSubString().length(),
                                           F_TO_STRING_TEXT_MAX))
                      .toUTF8String(_1));
        }
        }
      } else { // DIV /
        queue_.push_back(Token::T_DIV);
        i += 1;
      }
    } break;
    case (UChar)'%':
      queue_.push_back(Token::T_MOD);
      i += 1;
      break;
    case (UChar)'"': // string or char
    {
      int j = i;
      bool findString = false;
      while (j < text_.length()) {
        if (text_.charAt(j) == (UChar)'\\') {
          j += 2;
          continue;
        }
        if (text_.charAt(j) == (UChar)'\"') {
          j += 1;
          findString = true;
          break;
        }
        j += 1;
      }
      std::string _1;
      F_CHECK(
          findString, "Parse Error! i:{}, j:{}, text_: {}", i, j,
          text_
              .tempSubString(i, std::max<int>(text_.tempSubString(i).length(),
                                              F_TO_STRING_TEXT_MAX))
              .toUTF8String(_1));
      Sptr<Token> strToken =
          Sptr<Token>(new StringToken(text_.tempSubString(i, j - i)));
      queue_.push_back(strToken);
      i = j + 1;
    } break;
    case (UChar)'=': // = or ==
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("==")) {
        queue_.push_back(Token::T_EQ);
        i += 2;
      } else {
        queue_.push_back(Token::T_ASSIGNMENT);
        i += 1;
      }
      break;
    case (UChar)'!': // ! or !=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("!=")) {
        queue_.push_back(Token::T_NEQ);
        i += 2;
      } else {
        queue_.push_back(Token::T_NOT);
        i += 1;
      }
      break;
    case (UChar)'<': // < or <=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE("<=")) {
        queue_.push_back(Token::T_LE);
        i += 2;
      } else {
        queue_.push_back(Token::T_LT);
        i += 1;
      }
      break;
    case (UChar)'>': // > or >=
      if (text_.tempSubString(i, 2) == UNICODE_STRING_SIMPLE(">=")) {
        queue_.push_back(Token::T_GE);
        i += 2;
      } else {
        queue_.push_back(Token::T_GT);
        i += 1;
      }
      break;
    case (UChar)'T': // True
      if (text_.tempSubString(i, 4) == UNICODE_STRING_SIMPLE("True")) {
        queue_.push_back(Token::T_TRUE);
        i += 4;
      }
      break;
    case (UChar)'F': // False
      if (text_.tempSubString(i, 5) == UNICODE_STRING_SIMPLE("False")) {
        queue_.push_back(Token::T_FALSE);
        i += 5;
      }
      break;
    }
  }
}

} // namespace fastype

#undef F_TO_STRING_TEXT_MAX
