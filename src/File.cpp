// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Logging.h"
#include "fmt/format.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <numeric>
#include <string>

#define BUF_SIZE 8192

namespace fastype {

File::File(const std::string &fileName)
    : Logging("File"), fileName_(fileName),
      fd_(std::fopen(fileName.data(), "rw")), loaded_(false) {
  readBuffer_.reserve(BUF_SIZE);
  F_DEBUGF("File:{}", toString());
}

File::~File() {
  F_DEBUGF("File:{}", toString());
  if (fd_) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
  loaded_ = false;
  readBuffer_.clear();
  lineList_.clear();
}

const std::string &File::fileName() const { return fileName_; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line &File::get(int lineNumber) { return lineList_[lineNumber]; }

int File::count() {
  load();
  return lineList_.size();
}

bool File::empty() {
  load();
  return lineList_.empty();
}

int File::truncate(int start, int length) {
  load();
  // lineList_.truncate(start, length);
  return length;
}

int File::clear() {
  lineList_.clear();
  return 0;
}

int File::loaded() const { return loaded_; }

std::string File::toString() const {
  return fmt::format("[ @File fileName_:{} fd_:{} loaded_:{} "
                     "readBuffer_#data:{} readBuffer_#size:{} "
                     "lineList_#size:{} ]",
                     fileName_, (void *)fd_, loaded_, readBuffer_.data(),
                     readBuffer_.size(), lineList_.size());
}

int64_t File::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;

  // load file
  while (!loaded_) {
    F_DEBUGF("before expand, readBuffer_#capacity:{} readBuffer_#size:{}",
             readBuffer_.capacity(), readBuffer_.size());
    if (readBuffer_.capacity() <= readBuffer_.size()) {
      readBuffer_.reserve(readBuffer_.capacity() * 2);
    }
    F_DEBUGF("after expand, readBuffer_#capacity:{} readBuffer_#size:{}",
             readBuffer_.capacity(), readBuffer_.size());
    char *start = readBuffer_.data() + readBuffer_.size();
    int length = readBuffer_.capacity() - readBuffer_.size();

    int64_t n = (int64_t)std::fread(start, sizeof(char), length, fd_);
    F_DEBUGF("read n:{}", n);

    if (n > 0L) {
      readed += n;
      F_DEBUGF("readed:{}", readed);
      readBuffer_.resize(readBuffer_.size() + n);
    }

    // EOF
    if (n <= 0L) {
      loaded_ = true;
      F_DEBUGF("loaded_:{}", loaded_);
    }
  }

  F_DEBUGF("readBuffer_#data: {} readBuffer_#size:{}",
           (void *)readBuffer_.data(), readBuffer_.size());
  // if buffer has nothing
  if (readBuffer_.size() <= 0) {
    return readed;
  }

  // split readBuffer_ to lines
  char *start = readBuffer_.data();
  char *end = readBuffer_.data() + readBuffer_.size();
  while (true) {
    char *lineBreak = std::find(start, end, '\n');
    if (lineBreak == end) {
      break;
    }

    Line l;
    int sz = lineBreak - start;
    l.expand(std::max<int64_t>(sz + 1, l.capacity() * 2));
    std::memcpy(l.data(), start, sz + 1);
    l.setSize(sz + 1);
    F_DEBUGF("new line:{}", l.toString());
    lineList_.push_back(l);
    start = lineBreak + 1;
  }

  return readed;
}

} // namespace fastype

#undef BUF_SIZE
