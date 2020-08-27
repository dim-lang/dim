// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <memory>
#include <stack>

class Scanner;
class Buffer;

class BufferStack {
public:
  BufferStack(Scanner *scanner);
  virtual ~BufferStack();
  virtual int push(const std::string &fileName);
  virtual int pop();
  virtual Buffer *top() const;
  virtual int size() const;
  virtual bool empty() const;

private:
  Scanner *scanner_;
  std::stack<Buffer *> bufstack_;
};
