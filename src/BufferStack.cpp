// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "BufferStack.h"
#include "Exception.h"
#include "Log.h"

BufferStack::BufferStack(Scanner *scanner) : scanner_(scanner), bufstack_() {}

BufferStack::~BufferStack() {
  EX_ASSERT(bufstack_.empty(), "bufstack_ must empty:{}", bufstack_.size());
  scanner_ = nullptr;
}

int BufferStack::push(const std::string &fileName) {
  std::shared_ptr<Buffer> buffer(new Buffer(fileName, scanner_->yyscanner));

  /* remember state */
  if (!bufstack_.empty()) {
    LOG_INFO("top buffer:{} location:{}", bufstack_.top()->fileName,
             (std::stringstream() << bufstack_.top()->location).str());
  }

  /* switch to new file */
  bufstack_.push(buffer);

  yy_switch_to_buffer(buffer->yyBufferState, scanner_->yyscanner);
  yyset_lineno(1, scanner_->yyscanner);
  return 1;
}

int BufferStack::pop() {
  EX_ASSERT(!bufstack_.empty(), "bufstack_ must not empty: {}",
            bufstack_.size());
  std::shared_ptr<Buffer> buffer = bufstack_.top();
  bufstack_.pop();

  // if no more buffer on stack, stop yylex
  if (bufstack_.empty()) {
    return 0;
  }

  // if there's more buffer on stack, restore old buffer
  buffer = bufstack_.top();
  yy_switch_to_buffer(buffer->yyBufferState, scanner_->yyscanner);
  yyset_lineno(buffer->location.end.line, scanner_->yyscanner);
  return 1;
}

std::shared_ptr<Buffer> BufferStack::top() const { return bufstack_.top(); }

int BufferStack::size() const { return (int)bufstack_.size(); }

bool BufferStack::empty() const { return bufstack_.empty(); }
