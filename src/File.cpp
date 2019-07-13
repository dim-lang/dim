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
  load();
  // check if empty file
  return lineList_.size() > 0 ? lineList_[0] : end();
}

Line File::end() { return Line::undefined(); }

Line File::line(int32_t lineNumber) {
  int64_t n = loadUntil(lineNumber);
  F_DEBUGF("load n: {}", n);
  (void)n;
  return lineList_.size() >= lineNumber ? lineList_[lineNumber] : end();
}

Line File::next(const Line &l) {
  int64_t n = loadUntil(l.lineNumber() + 1);
  F_DEBUGF("load n: {}", n);
  (void)n;
  // if has next line, or set endline
  return hasNext(l) ? lineList_[l.lineNumber() + 1] : end();
}

bool File::hasNext(const Line &l) {
  int64_t n = loadUntil(l.lineNumber() + 1);
  F_DEBUGF("load n: {}", n);
  (void)n;
  // check if has next line
  return l.lineNumber() + 1 < lineList_.size();
}

Line File::previous(const Line &l) {
  int64_t n = loadUntil(l.lineNumber() - 1);
  F_DEBUGF("load n: {}", n);
  (void)n;
  // if has previous line, or set endline
  return hasPrevious(l) ? lineList_[l.lineNumber() - 1] : end();
}

bool File::hasPrevious(const Line &l) {
  int64_t n = loadUntil(l.lineNumber() + 1);
  F_DEBUGF("load n: {}", n);
  (void)n;
  // check if has previous line
  return l.lineNumber() - 1 >= 0;
}

std::string File::toString() const {
  return fmt::format("[ @File fileName: {}, fd: {}, eof: {}, readBuffer: {}, "
                     "bufferList#size: {}, lineList#size: {} ]",
                     fileName_, (void *)fd_, eof_, readBuffer_.size(),
                     bufferList_.size(), lineList_.size());
}

void File::closeLastLine(LineBound right) {
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

int64_t File::load() {
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
