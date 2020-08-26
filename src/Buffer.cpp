// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Buffer.h"
#include "Exception.h"
#include "Scanner.h"

Buffer::Buffer(const std::string &a_fileName, Scanner *scanner)
    : fileName(a_fileName), yyBufferState(nullptr), location(), fp(nullptr),
      scanner_(scanner) {
  EX_ASSERT(scanner_, "scanner_ must not null");
  EX_ASSERT(fileName.length() > 0, "fileName invalid: {}!", fileName);
  fp = std::fopen(fileName.c_str(), "r");
  if (!fp) {
    release();
    EX_ASSERT(fp, "file {} cannot open!", fileName);
  }
  yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE, scanner->yyscanner);
  if (!yyBufferState) {
    release();
    EX_ASSERT(yyBufferState, "yy_create_buffer {} failed with scanner {}",
              fileName, (void *)scanner->yyscanner);
  }
}

Buffer::~Buffer() { release(); }

void Buffer::release() {
  if (fp) {
    std::fclose(fp);
    fp = nullptr;
  }
  if (yyBufferState) {
    yy_delete_buffer(yyBufferState, scanner_->yyscanner);
    yyBufferState = nullptr;
  }
  scanner_ = nullptr;
}
