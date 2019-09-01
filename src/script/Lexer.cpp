// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include "exception/ScriptException.h"
#include "script/token/IdToken.h"
#include "script/token/NumberToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>

namespace fastype {

const std::regex RegexPattern(
    "\\s*((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")"
    "|[A-Z_a-z][A-Z_a-z0-9]*|==|!=|<|<=|>|>=|&&|\\|\\||\\p{Punct})?");

Lexer::Lexer(void *resource, ResourceHandler resourceHandler)
    : more_(true), index_(0), length_(std::strlen((char *)resource)),
      resourceHolder_(resourceHandler, resource) {}

Lexer::~Lexer() {
  while (!queue_.empty()) {
    std::shared_ptr<Token> t = queue_.front();
    t.reset();
    queue_.pop_front();
  }
  queue_.clear();
}

std::shared_ptr<Token> Lexer::read() {
  if (fillQueue(0)) {
    std::shared_ptr<Token> t = queue_.front();
    queue_.pop_front();
    return t;
  } else {
    return Token::EOF_;
  }
}

std::shared_ptr<Token> Lexer::peek(int i) {
  if (fillQueue(i)) {
    return queue_[i];
  } else {
    return Token::EOF_;
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
  std::string line;
  char *r = (char *)resourceHolder_.resource();

  for (int i = index_; i < length_; i++) {
    if (r[i] == '\n') {
      line = std::string(r + index_, i - index_);
      index_ = i + 1;
      break;
    }
  }
  if (line.empty()) {
    more_ = false;
    return;
  }
  int lineNumber = 0;

  std::sregex_iterator next(line.begin(), line.end(), RegexPattern);
  std::sregex_iterator end;
  std::vector<std::string> elementList;
  while (next != end) {
    std::smatch match = *next;
    elementList.push_back(match.str());
  }
  if (!elementList.empty()) {
    std::string m1 = elementList[0];
    if (!m1.empty()) {
      if (elementList.size() >= 2 && elementList[1].empty()) {
        Token *token = nullptr;
        if (elementList.size() >= 3 && !elementList[2].empty()) {
          token = new NumberToken(lineNumber, std::atoi(m1.data()));
        } else if (elementList.size() >= 4 && !elementList[3].empty()) {
          token = new StringToken(lineNumber, m1);
        } else {
          token = new IdToken(lineNumber, m1);
        }
        queue_.push_back(std::shared_ptr<Token>(token));
      }
    }
  } else {
    throw new ScriptException(fmt::format("bad token at {}", lineNumber));
  }
  queue_.push_back(
      std::shared_ptr<Token>(new IdToken(lineNumber, Token::EOL_)));
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
