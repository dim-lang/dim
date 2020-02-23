// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "TokenBuffer.h"
#include "Log.h"

#ifndef FILE_EXT
#define FILE_EXT ".co"
#endif

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 16384
#endif

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);
extern YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
extern void yy_delete_buffer(YY_BUFFER_STATE b);

std::string TokenBuffer::CurrentBuffer = "";

static std::stack<TokenBuffer *> BufferStack;

int TokenBuffer::pushImport(const std::string &module) {
  TokenBuffer *fb = new TokenBuffer(module);

  /* remember state */
  if (!BufferStack.empty()) {
    BufferStack.top()->lineNo = yylineno;
  }

  /* switch to new file */
  BufferStack.push(fb);

  yy_switch_to_buffer((YY_BUFFER_STATE)fb->yyBufferState);
  yylineno = 1;
  CurrentBuffer = fb->fileName;
  return 1;
}

int TokenBuffer::popImport() {
  CASSERT(!BufferStack.empty(),
          "BufferStack must not empty! CurrentBuffer:{} yylineno:{}",
          CurrentBuffer, yylineno);
  TokenBuffer *fb;

  fb = BufferStack.top();
  BufferStack.pop();
  delete fb;
  fb = nullptr;
  if (BufferStack.empty()) {
    CurrentBuffer = "";
    return 0;
  }

  fb = BufferStack.top();
  yy_switch_to_buffer((YY_BUFFER_STATE)fb->yyBufferState);
  yylineno = fb->lineNo;
  CurrentBuffer = fb->fileName;
  return 1;
}

std::string TokenBuffer::moduleToFile(const std::string &name) {
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
  r.append(FILE_EXT);
  return r;
}

std::string TokenBuffer::fileToModule(const std::string &name) {
  std::string r(name.substr(0, name.length() - 5));
  std::for_each(r.begin(), r.end(), [](char &c) {
    if (c == '/' || c == '\\') {
      c = '.';
    }
  });
  return r;
}

TokenBuffer::TokenBuffer(const std::string &module)
    : yyBufferState(nullptr), fileName(moduleToFile(module)), lineNo(1),
      fp(nullptr) {
  CASSERT(fileName.length() > 5, "{}:{} error! fileName {} invalid!",
          CurrentBuffer, yylineno, fileName);
  fp = std::fopen(fileName.c_str(), "r");
  if (!fp) {
    release();
    CASSERT(fp, "{}:{} error! file {} cannot open!", CurrentBuffer, yylineno,
            fileName);
  }
  yyBufferState = (void*)yy_create_buffer(fp, YY_BUF_SIZE);
  if (!yyBufferState) {
    release();
    CASSERT(yyBufferState, "{}:{} error! yy_create_buffer {} failed!",
            CurrentBuffer, yylineno, fileName);
  }
}

TokenBuffer::~TokenBuffer() { release(); }

void TokenBuffer::release() {
  if (fp) {
    std::fclose(fp);
    fp = nullptr;
  }
  if (yyBufferState) {
    yy_delete_buffer((YY_BUFFER_STATE)yyBufferState);
    yyBufferState = nullptr;
  }
}
