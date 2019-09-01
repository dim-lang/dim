// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include <cstring>

namespace fastype {

Lexer::Lexer(void *resource, ResourceHandler resourceHandler)
    : more_(true), index_(0), length_(strlen(resource)), resource_(resource),
      resourceHandler_(resourceHandler) {}

Lexer::~Lexer() {
  if (resourceHandler_) {
    resourceHandler_(resource_);
  }
  resourceHandler_ = nullptr;
  resource_ = nullptr;
}

Token Lexer::read() {
  if (fillQueue(0)) {
    Token t = queue_.front();
    queue_.pop_front();
    return t;
  } else {
    return Token::EOF_;
  }
}

Token Lexer::peek(int i) {
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
  for (int i = index_; i < length_; i++) {
    if (resource_[i] == '\n') {
      line = std::string(resource_ + index_, i - index_);
      index_ = i + 1;
      break;
    }
  }
  if (line.empty()) {
    more_ = false;
    return;
  }
  int lineNumber = 0;
  int pos = 0;
  int endPos = line.length();

  std::sregex_iterator next(line.begin(), line.end(), regexPattern_);
  std::sregex_iterator end;
  std::vector<std::string> elementList;
  while (next != end) {
    std::smatch match = *next;
    elementList.push_back(match.str());
  }
  if (!elementList.empty()) {
    std::string m1 = elementList[0];
    if (!m1.empty()) {
      if (elementList.size() >= 2 &&)
    }
  }
  queue_.add(IdToken(lineNumber, Token::EOL_));
}

std::string Lexer::toStringLiteral(const std::string &s) {
  std::string buf;
  buf.reserve(s.length() * 2);
  int len = s.length() - 1;
  for (int i = 1; i < len; i++) {
    char c = s.charAt(i);
    if (c == '\\' && i + 1 < len) {
      int c2 = s.charAt(i + 1);
      if (c2 == '"' || c2 == '\\') {
        c = s.charAt(++i);
      } else if (c2 == '\n') {
        ++i;
        c = '\n';
      }
    }
    buf.append(c);
  }
  return buf;
}

} // namespace fastype
