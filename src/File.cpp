// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "File.h"
#include "Line.h"
#include "Log.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

#define BUF_SIZE 4096

namespace fastype {

File::File(const std::string &fileName)
    : fileName_(fileName), fd_(std::fopen(fileName.data(), "rw")), eof_(false),
      readBuffer_(BUF_SIZE), bufferList_(), lineList_(),
      log_(LogManager::getLogger(fileName)) {
  F_DEBUGF("fileName: {}", fileName);
}

File::~File() {
  if (fd_) {
    std::fclose(fd_);
    fd_ = nullptr;
  }
  readBuffer_.release();
  std::for_each(
      bufferList_.begin(), bufferList_.end(),
      [](std::vector<std::shared_ptr<Buffer>>::iterator i) { (*i).reset(); });
  bufferList_.clear();
  lineList_.clear();
  F_DEBUGF("fileName: {}", fileName_);
}

const std::string &File::fileName() const { return fileName_; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line File::begin() {}

Line File::end() {}

Line File::getLine(int32_t lineNumber) {}

std::string File::toString() const {
  return "[ @File fileName: " + fileName_ + ", fd: 0x" + fd_ +
         ", eof: " + std::to_string(eof_) +
         ", readBuffer: " + readBuffer_.toString() +
         ", bufferList.size: " + bufferList_.size() +
         ", lineList.size: " + lineList_.size() + " ]";
}

int64_t File::loadOne() {
  // read 1 buffer
  readBuffer_.clear();
  int64_t n = (int64_t)std::fread(readBuffer_.data(), readBuffer_.capacity(),
                                  sizeof(char), fd);
  readBuffer_.setSize(n);

  // case 1: if no more bytes
  if (n <= 0L) {
    return n;
  }

  // case 2: read n bytes, append to bufferList, find all lines to lineList
  std::shared_ptr<Buffer> b(new Buffer(BUF_SIZE));
  b->read(readBuffer_, readBuffer_.size());
  bufferList_.push_back(b);

  Line line(std::shared_ptr<File>(this), lineList_.size());

  for (int i = 0; i < b->size(); i++) {
    // line break
    if (b->data()[i] != '\n') {
      continue;
    }

    if (lineList_.size() > 0 && lineList_.back().right().undefined()) {
      // case 1: has previous lines, and last line is opened

      // close last line
      Line &lastLine = lineList_.back();
      lastLine.right().setBuffer(bufferList_.size() - 1);
      lastLine.right().setByte(i + 1);

      // open new line
      line.setLineNumber(lineList_.size());
      line.right().reset();
      line.setLeft(LineBound(bufferList_.size(), i + 1));

    } else if (lineList_.size() == 0 || !lineList_.back().right().undefined()) {
      // case 2: has no previous lines
      // case 3: has previous lines, but last line is closed

      // open new line
      line.setLineNumber(lineList_.size());
      line.right().reset();
      line.setLeft(LineBound(bufferList_.size() - 1, i + 1));
      lineList_.push_back(line);
    }
  }

  // return readed bytes
  return n;
}

} // namespace fastype

#undef BUF_SIZE
