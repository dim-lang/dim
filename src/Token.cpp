// Copyright 2019- <co-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Log.h"

#define TOKEN_FILE_EXT ".co"
#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 32768
#endif

std::string Buffer::moduleToFile(const std::string &name) {
  std::string r(name);
  char replace;
#if PLATFORM_WINDOWS
  replace = '\\';
#else
  replace = '/';
#endif
  std::for_each(r.begin(), r.end(), [&replace](char &c) {
    if (c == '.') {
      c = replace;
    }
  });
  r.append(TOKEN_FILE_EXT);
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

Buffer::Buffer(const std::string &a_fileName, yyscan_t yy_scaninfo)
    : yyBufferState(nullptr), fileName(a_fileName), lineNo(1), fp(nullptr),
      yy_scaninfo_(yy_scaninfo) {
  CASSERT(fileName.length() > 3, "fileName invalid: {}!", fileName);
  fp = std::fopen(fileName.c_str(), "r");
  if (!fp) {
    release();
    CASSERT(fp, "file {} cannot open!", fileName);
  }
  yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE, yy_scaninfo_);
  if (!yyBufferState) {
    release();
    CASSERT(yyBufferState, "yy_create_buffer for file {} failed: {}", fileName,
            (void *)yy_scaninfo_);
  }
}

Buffer::~Buffer() { release(); }

void Buffer::release() {
  if (fp) {
    std::fclose(fp);
    fp = nullptr;
  }
  if (yyBufferState) {
    yy_delete_buffer(yyBufferState, yy_scaninfo_);
    yyBufferState = nullptr;
  }
  yy_scaninfo_ = nullptr;
}

BufferStack::BufferStack(yyscan_t yy_scaninfo)
    : yy_scaninfo_(yy_scaninfo), bufferStack_() {}

BufferStack::~BufferStack() {
  yy_scaninfo_ = nullptr;
  CASSERT(bufferStack_.empty(), "bufferStack_ not empty: {}",
          bufferStack_.size());
}

int BufferStack::push(const std::string &fileName) {
  Buffer *tb = new Buffer(fileName, yy_scaninfo_);

  /* remember state */
  if (!bufferStack_.empty()) {
    bufferStack_.top()->lineNo = yyget_lineno(yy_scaninfo_);
  }

  /* switch to new file */
  bufferStack_.push(tb);

  yy_switch_to_buffer(tb->yyBufferState, yy_scaninfo_);
  yyset_lineno(1, yy_scaninfo_);
  return 1;
}

int BufferStack::pop() {
  CASSERT(!bufferStack_.empty(), "bufferStack_ must not empty! current file:{}",
          bufferStack_.top()->fileName);
  Buffer *tb = bufferStack_.top();
  bufferStack_.pop();
  delete tb;
  tb = nullptr;
  if (bufferStack_.empty()) {
    return 0;
  }

  tb = bufferStack_.top();
  yy_switch_to_buffer(tb->yyBufferState, yy_scaninfo_);
  yyset_lineno(tb->lineNo, yy_scaninfo_);
  return 1;
}

Buffer *BufferStack::top() const {
  return bufferStack_.empty() ? nullptr : bufferStack_.top();
}

int BufferStack::size() const { return (int)bufferStack_.size(); }

bool BufferStack::empty() const { return bufferStack_.empty(); }
