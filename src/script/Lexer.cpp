// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "exception/ParseException.h"
#include "script/token/BooleanToken.h"
#include "script/token/CommentToken.h"
#include "script/token/EofToken.h"
#include "script/token/EolToken.h"
#include "script/token/FloatingToken.h"
#include "script/token/IdentifierToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/KeywordToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/PunctuationToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <regex>

#define F_TO_STRING_TEXT_MAX 128

namespace fastype {

Lexer::Lexer(const icu::UnicodeString &text)
    : more_(true), pos_(0), text_(text), {
  F_INFO("Constructor:{}", toString());
}

std::string Lexer::toString() const {
  std::string utf8;
  if (text_.length() > F_TO_STRING_TEXT_MAX) {
    icu::UnicodeString tmp;
    text_.extract(0, F_TO_STRING_TEXT_MAX, tmp);
    tmp.toUTF8String(utf8);
  } else {
    text_.toUTF8String(utf8);
  }
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
  while (i >= queue_.size()) {
    if (more_) {
      readLine();
    } else {
      return false;
    }
  }
  return true;
}

void Lexer::readLine() {
  icu::UnicodeString unixLineBreak = UNICODE_STRING("\n");
  icu::UnicodeString line;

  int nextPos = text_.indexOf(unixLineBreak, i);
  if (nextPos < 0) {
    more_ = false;
    return;
  }
  line = text_.tempSubString(pos_, nextPos - pos_);
  pos_ = nextPos + 1;

  int lineNumber = 0;
  int i = 0;
  while (i < line.size()) {
    if (line.tempSubString(i, 1) == Token::T_ADD->literal()) {
      queue_.push_back(Token::T_ADD);
    }
    if (line.tempSubString(i, 1) == Token::T_SUB->literal()) {
      queue_.push_back(Token::T_SUB);
    }
    if (line.tempSubString(i, 1) == Token::T_MUL->literal()) {
      queue_.push_back(Token::T_MUL);
    }
    if (line.tempSubString(i, 1) == Token::T_DIV->literal()) {
      queue_.push_back(Token::T_DIV);
    }
    if (line.tempSubString(i, 1) == Token::T_MOD->literal()) {
      queue_.push_back(Token::T_MOD);
    }
    if (line.tempSubString(i, 1) == Token::T_ASSIGNMENT->literal()) {
      queue_.push_back(Token::T_ASSIGNMENT);
    }
    if (line.length() >= 2 &&
        line.tempSubString(i, 2) == Token::T_EQ->literal()) {
      queue_.push_back(Token::T_EQ);
    }
    i++;
  }

  std::string Lexer::toStringLiteral(const std::string &s) {
    std::string buf;
    buf.reserve(s.length() * 2);
    int len = s.length() - 1;
    for (int i = 1; i < len; i++) {
      char c = s[i];
      if (c == '\\' && i + 1 < len) {
        int c2 = s[i + 1];
        if (c2 == '"' || c2 == '\\') {
          c = s[++i];
        } else if (c2 == '\n') {
          ++i;
          c = '\n';
        }
      }
      buf += c;
    }
    return buf;
  }

} // namespace fastype

#undef F_TO_STRING_TEXT_MAX
