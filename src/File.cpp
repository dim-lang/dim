// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Line.h"
#include "Log.h"
#include "fmt/format.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <numeric>
#include <string>

#define BUF_SIZE 4096

namespace fastype {

File::File(const std::string &fileName)
    : Logging(fileName), fileName_(fileName),
      fd_(std::fopen(fileName.data(), "rw")), eof_(false),
      readBuffer_(BUF_SIZE), bufferList_(), lineList_() {
  F_DEBUGF("fileName: {}", fileName);
}

File::~File() {
  if (fd_) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
  readBuffer_.release();
  for (int i = 0; i < bufferList_.size(); i++) {
    bufferList_[i].reset();
  }
  // std::for_each(
  // bufferList_.begin(), bufferList_.end(),
  //[](std::vector<std::shared_ptr<Buffer>>::iterator i) { (*i).reset(); });
  bufferList_.clear();
  lineList_.clear();
  F_DEBUGF("fileName: {}", fileName_);
}

const std::string &File::fileName() const { return fileName_; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line File::begin() {
  loadOne();
  // check if empty file
  return lineList_.size() > 0 ? lineList_[0] : end();
}

Line File::end() { return Line::undefinedLine(); }

Line File::line(int32_t lineNumber) {
  int64_t t;
  while (lineList_.size() <= lineNumber && (t = loadOne()) > 0L) {
  }
  return lineList_.size() >= lineNumber ? lineList_[lineNumber] : end();
}

std::string File::toString() const {
  return fmt::format("[ @File fileName: {}, fd: {}, eof: {}, readBuffer: {}, "
                     "bufferList#size: {}, lineList#size: {} ]",
                     fileName_, (void *)fd_, eof_, readBuffer_.size(),
                     bufferList_.size(), lineList_.size());
}

void File::closeLastLine(LineBound right) {
  if (lineList_.size() > 0 && lineList_.back().right().undefined()) {
    F_DEBUGF("last line: {}", lineList_.back().toString());
    Line &lastLine = lineList_.back();
    lastLine.setRight(right);
  }
}

void File::openNewLine(File *fp, int32_t lineNumber, LineBound left) {
  Line line(std::shared_ptr<File>(fp), lineNumber);
  line.setRight(LineBound());
  line.setLeft(left);
  lineList_.push_back(line);
}

int64_t File::loadOne() {
  if (eof_) {
    // for safety, if last line is opened, close the last line
    closeLastLine(
        LineBound(bufferList_.size() - 1, bufferList_.back()->size()));
    return 0;
  }

  // read 1 buffer
  readBuffer_.clear();
  int64_t n = (int64_t)std::fread(readBuffer_.data(), readBuffer_.capacity(),
                                  sizeof(char), fd_);
  readBuffer_.setSize(n);

  // case 1: if no more bytes
  if (n <= 0L) {
    // for safety, if last line is opened, close the last line
    eof_ = true;
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
    if (lineList_.size() > 0 && lineList_.back().right().undefined()) {
      // if has previous lines, and last line is opened

      // close last line
      closeLastLine(LineBound(bufferList_.size() - 1, i + 1));
      // open new line
      openNewLine(this, lineList_.size(),
                  LineBound(bufferList_.size() - 1, i + 1));
    } else if (lineList_.size() == 0 || !lineList_.back().right().undefined()) {
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

int64_t File::load(int32_t n) {
  std::vector<int64_t> r;
  int i = 0;
  int64_t t;
  while (i < n && (t = loadOne()) > 0L) {
    r.push_back(t);
    i++;
  }
  return std::accumulate(r.begin(), r.end(), 0L);
}

int64_t File::loadAll() {
  std::vector<int64_t> r;
  int64_t t;
  while ((t = loadOne()) > 0L) {
    r.push_back(t);
  }
  return std::accumulate(r.begin(), r.end(), 0L);
}

} // namespace fastype

#undef BUF_SIZE
