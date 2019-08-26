// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Lexer.h"
#include <cstring>

namespace fastype {

Lexer::Lexer(void *resource, ResourceHandler *resourceHandler)
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
  int i;
  for (i = index_; i < length_; i++) {
    if (resource[i] == '\n') {
      line = std::string(resource_ + index_, resource_ + i);
      index_ = i + 1;
      break;
    }
  }
  if (line.length() == 0) {
    more_ = false;
    return;
  }
  int lineNumber = 0;
}

void Lexer::addToken(int lineNumber) {}

std::string Lexer::toStringLiteral(const std::string &s) {}

} // namespace fastype
