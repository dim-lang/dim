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
    : Logging(fileName), fileName_(fileName),
      fd_(std::fopen(fileName.data(), "rw")), loaded_(false),
      converter_(nullptr) {
  UErrorCode err;
  readBuffer_.reserve(BUF_SIZE);
  converter_ = ucnv_open(nullptr, &err);
  F_CHECKF(U_SUCCESS(err), "err:{}", (int)err);
  F_DEBUGF("File:{} err:{}", toString(), (int)err);
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
  if (converter_) {
    ucnv_close(converter_);
    converter_ = nullptr;
  }
}

const std::string &File::fileName() const { return fileName_; }

std::shared_ptr<File> File::open(const std::string &fileName) {
  return std::shared_ptr<File>(new File(fileName));
}

void File::close(std::shared_ptr<File> file) { file.reset(); }

Line &File::getLine(int lineNumber) { return lineList_[lineNumber]; }

int File::lineCount() {
  load();
  return lineList_.size();
}

bool File::empty() {
  load();
  return lineList_.empty();
}

int File::loaded() const { return loaded_; }

std::string File::toString() const {
  return fmt::format("[ @File fileName_:{} fd_:{} loaded_:{} "
                     "readBuffer_#data:{} readBuffer_#size:{} "
                     "lineList_#size:{} converter_:{} ]",
                     fileName_, (void *)fd_, loaded_, readBuffer_.data(),
                     readBuffer_.size(), lineList_.size(), (void *)converter_);
}

int64_t File::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;

  // load file
  while (!loaded_) {
    if (readBuffer_.capacity() <= readBuffer_.size()) {
      readBuffer_.reserve(readBuffer_.capacity() * 2);
    }
    char *start = readBuffer_.data() + readBuffer_.size();
    int length = readBuffer_.capacity() - readBuffer_.size();

    int64_t n = (int64_t)std::fread(start, length, sizeof(char), fd_);

    if (n > 0L) {
      readed += n;
      readBuffer_.resize(readBuffer_.size() + n);
    }

    // EOF
    if (n <= 0L) {
      loaded_ = true;
    }
  }

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
    UErrorCode err;
    int n;

    do {
      l.expand(std::max<int64_t>(lineBreak - start, l.capacity() * 2));
      n = ucnv_toUChars(converter_, l.data(), l.capacity(), start,
                        lineBreak - start, &err);
      F_DEBUGF("UErrorCode:{}", (int)err);
    } while (!U_SUCCESS(err));
    lineList_.push_back(l);
    start = lineBreak + 1;
  }

  return readed;
}

} // namespace fastype

#undef BUF_SIZE
