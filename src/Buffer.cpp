// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Buffer.h"
#include "Exception.h"
#include "Scanner.h"

#define BUFFER_SUFFIX ".nerd"

std::string Buffer::moduleToFile(const std::string &name) {
  std::string r(name);
  char replace;
#if BOOST_OS_WINDOWS
  replace = '\\';
#else
  replace = '/';
#endif
  std::for_each(r.begin(), r.end(), [&replace](char &c) {
    if (c == '.') {
      c = replace;
    }
  });
  r.append(BUFFER_SUFFIX);
  return r;
}

std::string Buffer::fileToModule(const std::string &name) {
  std::string r(name.substr(0, name.length() - 5));
  std::for_each(r.begin(), r.end(), [](char &c) {
    if (c == '/' || c == '\\') {
      c = '.';
    }
  });
  return r;
}

Buffer::Buffer(const std::string &a_fileName, Scanner *scanner)
    : yyBufferState(nullptr), fileName(a_fileName), lineNo(1), fp(nullptr),
      scanner_(scanner) {
  EX_ASSERT(scanner_, "scanner must not null");
  EX_ASSERT(fileName.length() > 3, "fileName invalid: {}!", fileName);
  fp = std::fopen(fileName.c_str(), "r");
  if (!fp) {
    release();
    EX_ASSERT(fp, "file {} cannot open!", fileName);
  }
  yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE, scanner->yy_scanner);
  if (!yyBufferState) {
    release();
    EX_ASSERT(yyBufferState, "yy_create_buffer for file {} failed: {}",
              fileName, (void *)scanner->yy_scanner);
  }
}

Buffer::~Buffer() { release(); }

void Buffer::release() {
  if (fp) {
    std::fclose(fp);
    fp = nullptr;
  }
  if (yyBufferState) {
    yy_delete_buffer(yyBufferState, scanner_->yy_scanner);
    yyBufferState = nullptr;
  }
  scanner_->yy_scanner = nullptr;
}

BufferStack::BufferStack(Scanner *scanner)
    : scanner_(scanner), bufferStack_() {}

BufferStack::~BufferStack() {
  EX_ASSERT(bufferStack_.empty(), "bufferStack_ not empty: {}",
            bufferStack_.size());
}

int BufferStack::push(const std::string &fileName) {
  Buffer *tb = new Buffer(fileName, scanner_->yy_scanner);

  /* remember state */
  if (!bufferStack_.empty()) {
    bufferStack_.top()->lineNo = yyget_lineno(scanner_->yy_scanner);
  }

  /* switch to new file */
  bufferStack_.push(tb);

  yy_switch_to_buffer(tb->yyBufferState, scanner_->yy_scanner);
  yyset_lineno(1, scanner_->yy_scanner);
  return 1;
}

int BufferStack::pop() {
  EX_ASSERT(!bufferStack_.empty(),
            "bufferStack_ must not empty! current file:{}",
            bufferStack_.top()->fileName);
  Buffer *tb = bufferStack_.top();
  bufferStack_.pop();
  delete tb;
  tb = nullptr;
  if (bufferStack_.empty()) {
    return 0;
  }

  tb = bufferStack_.top();
  yy_switch_to_buffer(tb->yyBufferState, scanner_->yy_scanner);
  yyset_lineno(tb->lineNo, scanner_->yy_scanner);
  return 1;
}

Buffer *BufferStack::top() const {
  return bufferStack_.empty() ? nullptr : bufferStack_.top();
}

int BufferStack::size() const { return (int)bufferStack_.size(); }

bool BufferStack::empty() const { return bufferStack_.empty(); }
