// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Logging.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fmt/format.h>
#include <numeric>
#include <string>

#define BUF_SIZE 8192

namespace fastype {

Buffer::Buffer(const std::string &fileName)
    : fileName_(fileName), fd_(std::fopen(fileName.data(), "rw")),
      loaded_(false), readBuffer_(BUF_SIZE) {
  F_INFO("Constructor:{}", toString());
}

Buffer::~Buffer() {
  F_INFO("Destructor:{}", toString());
  if (fd_) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
  loaded_ = false;
  for (int i = 0; i < lineList_.size(); i++) {
    lineList_[i] = Row();
  }
  lineList_.clear();
}

const std::string &Buffer::fileName() const { return fileName_; }

Sptr<Buffer> Buffer::open(const std::string &fileName) {
  return Sptr<Buffer>(new Buffer(fileName));
}

void Buffer::close(Sptr<Buffer> file) { file.reset(); }

Row Buffer::get(int lineNumber) { return lineList_[lineNumber]; }

int Buffer::count() {
  load();
  return lineList_.size();
}

bool Buffer::empty() {
  load();
  return lineList_.empty();
}

int Buffer::truncate(int start, int length) {
  load();
  // lineList_.truncate(start, length);
  return length;
}

int Buffer::clear() {
  lineList_.clear();
  return 0;
}

int Buffer::loaded() const { return loaded_; }

std::string Buffer::toString() const {
  return fmt::format("[ @Buffer fileName_:{} fd_:{} loaded_:{} readBuffer_:{} "
                     "lineList_#size:{} ]",
                     fileName_, (void *)fd_, loaded_, readBuffer_.toString(),
                     lineList_.size());
}

int64_t Buffer::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;
  char buf[BUF_SIZE];

  while (!loaded_) {
    int64_t n = (int64_t)std::fread(buf, sizeof(char), BUF_SIZE, fd_);

    if (n > 0L) {
      readBuffer_.concat(buf, n);
      readed += n;
    } else {
      // n <= 0L, EOF
      loaded_ = true;
    }
  }

  F_INFO("readBuffer_:{}, lineList_#size:{}", readBuffer_.toString(),
         lineList_.size());
  // if buffer has nothing
  if (readBuffer_.size() <= 0) {
    return readed;
  }

  // split readBuffer_ to lines
  char *start = readBuffer_.head();
  char *end = readBuffer_.head() + readBuffer_.size();
  while (true) {
    if (start >= end) {
      F_INFO("start:{} >= end:{}", (void *)start, (void *)end);
      break;
    }
    char *lineBreak = std::find(start, end, '\n');
    F_INFO("start:{} lineBreak:{} end:{} lineBreak-start:{} end-start:{} "
           "end-lineBreak:{}",
           (void *)start, (void *)lineBreak, (void *)end,
           (int)(lineBreak - start), (int)(end - start),
           (int)(end - lineBreak));
    if (lineBreak >= end) {
      F_INFO("lineBreak:{} >= end:{}", (void *)lineBreak, (void *)end);
      break;
    }

    int sz = lineBreak - start + 1; // 1 is for '\n'
    char ef = '\0';
    Cowstr cs(sz);
    cs.concat(start, sz);
    cs.concat(&ef, 1);
    Row r(cs, lineList_.size(), false); // 1 is for '\0'
    F_INFO("new line:{}", r.toString());
    lineList_.push_back(r);
    start = lineBreak + 1;
  }

  // F_INFO("buffer read:{} elapse:{}", readed, F_TIMER_ELAPSE(load));
  return readed;
}

} // namespace fastype

#undef BUF_SIZE
