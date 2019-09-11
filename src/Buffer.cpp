// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Logging.h"
#include "Profile.h"
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
  F_DEBUGF("Constructor:{}", toString());
}

Buffer::~Buffer() {
  F_DEBUGF("Destructor:{}", toString());
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

Buffer::Buffer(Buffer &&other) {}

Buffer &Buffer::operator=(Buffer &&other) {
  if (this == &other) {
    return *this;
  }
}

const std::string &Buffer::fileName() const { return fileName_; }

std::shared_ptr<Buffer> Buffer::open(const std::string &fileName) {
  return std::shared_ptr<Buffer>(new Buffer(fileName));
}

void Buffer::close(std::shared_ptr<Buffer> file) { file.reset(); }

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
      F_DEBUGF("before resize readBuffer_#size: {}, after resize "
               "readBuffer_#size:{}",
               sizeBefore, sizeAfter);
    } else {
      // n <= 0L, EOF
      loaded_ = true;
    }
  }

  F_DEBUGF("readBuffer_#data: {} readBuffer_#size:{} lineList_#size:{}",
           (void *)readBuffer_.data(), readBuffer_.size(), lineList_.size());
  // if buffer has nothing
  if (readBuffer_.size() <= 0) {
    return readed;
  }

  // split readBuffer_ to lines
  char *start = readBuffer_.head();
  char *end = readBuffer_.head() + readBuffer_.size();
  while (true) {
    if (start >= end) {
      F_DEBUGF("start:{} >= end:{}", (void *)start, (void *)end);
      break;
    }
    char *lineBreak = std::find(start, end, '\n');
    F_DEBUGF("start:{} lineBreak:{} end:{} lineBreak-start:{} end-start:{} "
             "end-lineBreak:{}",
             (void *)start, (void *)lineBreak, (void *)end,
             (int)(lineBreak - start), (int)(end - start),
             (int)(end - lineBreak));
    if (lineBreak >= end) {
      F_DEBUGF("lineBreak:{} >= end:{}", (void *)lineBreak, (void *)end);
      break;
    }

    int sz = lineBreak - start + 1;         // 1 is for '\n'
    Row l(sz + 1, lineList_.size(), false); // 1 is for '\0'
    std::memcpy(l.data(), start, sz);
    l.setSize(sz + 1); // 1 is for '\0', in fastype line '\0' counts
    (*l)[sz] = '\0';
    F_DEBUGF("new line:{}", l.toString());
    lineList_.push_back(l);
    start = lineBreak + 1;
  }

  // F_TIMER_STOP(load);
  // F_DEBUGF("buffer read:{} elapse:{}", readed, F_TIMER_ELAPSE(load));
  return readed;
}

} // namespace fastype

#undef BUF_SIZE
