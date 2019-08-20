// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "model/Cache.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <numeric>
#include <string>

#define BUF_SIZE 8192

namespace fastype {

Cache::Cache(const std::string &fileName)
    : fileName_(fileName), fd_(std::fopen(fileName.data(), "rw")),
      loaded_(false), readBuffer_() {
  readBuffer_.expand(BUF_SIZE);
  std::memset(readBuffer_.data(), 0, BUF_SIZE * sizeof(char));
  F_DEBUGF("Cache:{}", toString());
}

Cache::~Cache() {
  F_DEBUGF("Cache:{}", toString());
  if (fd_) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
  loaded_ = false;
  readBuffer_.clear();
  lineList_.clear();
}

const std::string &Cache::fileName() const { return fileName_; }

std::shared_ptr<Cache> Cache::create(const std::string &fileName) {
  return std::shared_ptr<Cache>(new Cache(fileName));
}

void Cache::destroy(std::shared_ptr<Cache> file) { file.reset(); }

Line &Cache::get(int lineNumber) { return lineList_[lineNumber]; }

int Cache::count() {
  load();
  return lineList_.size();
}

bool Cache::empty() {
  load();
  return lineList_.empty();
}

int Cache::truncate(int start, int length) {
  load();
  // lineList_.truncate(start, length);
  return length;
}

int Cache::clear() {
  lineList_.clear();
  return 0;
}

int Cache::loaded() const { return loaded_; }

std::string Cache::toString() const {
  return fmt::format("[ @Cache fileName_:{} fd_:{} loaded_:{} "
                     "readBuffer_#data:{} readBuffer_#size:{} "
                     "lineList_#size:{} ]",
                     fileName_, (void *)fd_, loaded_,
                     (void *)readBuffer_.data(), readBuffer_.size(),
                     lineList_.size());
}

int64_t Cache::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;

  // F_TIMER_START(load);
  // load file
  while (!loaded_) {
    if (readBuffer_.capacity() <= readBuffer_.size()) {
      readBuffer_.expand(readBuffer_.capacity() * 2);
    }
    char *start = readBuffer_.data() + readBuffer_.size();
    int length = readBuffer_.capacity() - readBuffer_.size();

    int64_t n = (int64_t)std::fread(start, sizeof(char), length, fd_);

    if (n > 0L) {
      readed += n;
      size_t sizeBefore = readBuffer_.size();
      readBuffer_.incSize(n);
      size_t sizeAfter = readBuffer_.size();
      (void)sizeBefore;
      (void)sizeAfter;
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
    // F_TIMER_STOP(load);
    // F_DEBUGF("buffer has nothing, elapse:{}", F_TIMER_ELAPSE(load));
    return readed;
  }

  // split readBuffer_ to lines
  char *start = readBuffer_.data();
  char *end = readBuffer_.data() + readBuffer_.size();
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

    int sz = lineBreak - start + 1;          // 1 is for '\n'
    Line l(sz + 1, lineList_.size(), false); // 1 is for '\0'
    std::memcpy(l.data(), start, sz);
    l.setSize(sz + 1); // 1 is for '\0', in fastype line '\0' counts
    l[sz] = '\0';
    F_DEBUGF("new line:{}", l.toString());
    lineList_.push_back(l);
    start = lineBreak + 1;
  }

  // F_TIMER_STOP(load);
  // F_DEBUGF("buffer read:{} elapse:{}", readed, F_TIMER_ELAPSE(load));
  return readed;
} // namespace fastype

} // namespace fastype

#undef BUF_SIZE
