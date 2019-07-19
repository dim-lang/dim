// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Line.h"
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
    : Logging(fileName), fileName_(fileName),
      fd_(std::fopen(fileName.data(), "rw")), loaded_(false),
      readBuffer_(nullptr), readBufferSize_(0), readBufferCapacity_(0) {
  readBuffer_ = new char[BUF_SIZE];
  if (readBuffer_) {
    readBufferCapacity_ = BUF_SIZE;
  } else {
    F_ERRORF("fileName:{} readBuffer_:{} readBufferCapacity_:{}", fileName,
             (void *)readBuffer_, readBufferCapacity_);
  }
  F_DEBUGF("fileName:{}", fileName);
}

File::~File() {
  F_DEBUGF("fileName:{}", fileName_);
  if (fd_) {
    F_DEBUGF("close fd_:{} fileName_:{}", (void *)fd_, fileName_);
    std::fclose(fd_);
    fd_ = nullptr;
  }
  loaded_ = false;
  if (readBuffer_) {
    delete[] readBuffer_;
    readBuffer_ = nullptr;
    readBufferSize_ = 0;
    readBufferCapacity_ = 0;
  }
  lineList_.clear();
}

const std::string &File::fileName() const { return fileName_; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

icu::UnicodeString &File::getLine(int32_t lineNumber) {}

int File::lineCount() {}

bool File::empty() {}

std::string File::toString() const {
  return fmt::format(
      "[ @File fileName_: {}, fd_: {}, loaded_: {}, readBuffer_: {}, "
      "lineList_#size: {} ]",
      fileName_, (void *)fd_, loaded_, readBuffer_.size(), lineList_.size());
}

void File::closeLastLine(Position right) {
  if (lineList_.size() > 0 && lineList_.back().right().unset()) {
    F_DEBUGF("last line: {}", lineList_.back().toString());
    Line &lastLine = lineList_.back();
    lastLine.setRight(right);
  }
}

void File::openNewLine(File *fp, int32_t lineNumber, LineBound left) {
  Line line(lineNumber);
  line.setRight(LineBound());
  line.setLeft(left);
  lineList_.push_back(line);
}

int File::expandReadBuffer() {
  int n = readBufferCapacity_ * 2;
  char *buf = std::realloc(readBuffer_, n);
  if (buf) {
    readBuffer_ = buf;
    readBufferCapacity_ = n;
  }
  return readBufferCapacity_;
}

int64_t File::load() {
  if (loaded_) {
    // already read all, EOF
    return 0;
  }

  // try read previous bytes

  // read 1 buffer
  readBuffer_.clear();
  int64_t n = (int64_t)std::fread(readBuffer_.data(), readBuffer_.capacity(),
                                  sizeof(char), fd_);
  readBuffer_.setSize(n);

  // case 1: if no more bytes
  if (n <= 0L) {
    // for safety, if last line is opened, close the last line
    loaded_ = true;
    closeLastLine(
        LineBound(bufferList_.size() - 1, bufferList_.back()->size()));
    return n;
  }

  // case 2: read n bytes, append to bufferList, find all lines to lineList
  std::shared_ptr<Buffer> b(new Buffer(BUF_SIZE));
  b->read(readBuffer_, readBuffer_.size());
  bufferList_.push_back(b);

  for (int i = 0; i < b->size(); i++) {
    if (b->data()[i] != '\n') {
      continue;
    }

    // at line break
    if (lineList_.size() > 0 && lineList_.back().right().unset()) {
      // if has previous lines, and last line is opened

      // close last line
      closeLastLine(LineBound(bufferList_.size() - 1, i + 1));
      // open new line
      openNewLine(this, lineList_.size(),
                  LineBound(bufferList_.size() - 1, i + 1));
    } else if (lineList_.size() == 0 || !lineList_.back().right().unset()) {
      // case 2: has no previous lines
      // case 3: has previous lines, but last line is closed

      // open new line
      openNewLine(this, lineList_.size(),
                  LineBound(bufferList_.size() - 1, i + 1));
    }
  }

  // return readed bytes
  return n;
}

int64_t File::loadUntil(int32_t n) {
  std::vector<int64_t> r;
  int64_t t;
  while (lineList_.size() <= n && (t = load()) > 0L) {
    r.push_back(t);
  }
  return std::accumulate(r.begin(), r.end(), 0L);
}

int64_t File::loadAll() {
  std::vector<int64_t> r;
  int64_t t;
  while ((t = load()) > 0L) {
    r.push_back(t);
  }
  return std::accumulate(r.begin(), r.end(), 0L);
}

} // namespace fastype

#undef BUF_SIZE
