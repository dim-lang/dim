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
  readBuffer_.resize(BUF_SIZE);
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

icu::UnicodeString &File::getLine(int lineNumber) {}

int File::lineCount() {}

bool File::empty() {}

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
  char *lineBreak = nullptr;

  // read buffer until line break or EOF
  while (true) {
    if (readBuffer_.capacity() <= readBuffer_.size()) {
      readBuffer_.reserve(std::max<size_t>(128, readBuffer_.capacity() * 2));
    }
    char *start = readBuffer_.data() + readBuffer_.size();
    int length = readBuffer_.capacity() - readBuffer_.size();

    int64_t n = (int64_t)std::fread(start, length, sizeof(char), fd_);
    if (n > 0) {
      readed += n;
      readBuffer_.resize(readBuffer_.size() + n);
    }

    // case 1: find EOF
    if (n <= 0L) {
      loaded_ = true;
      break;
    }

    char *end = readBuffer_.data() + readBuffer_.size();
    F_DEBUGF("read margin, start:{} length:{} n:{} loaded_:{}", (void *)start,
             length, n, loaded_);
    lineBreak = std::find(start, end, '\n');

    // case 2: find line break
    if (lineBreak != end) {
      F_CHECKF(lineBreak > start, "lineBreak:{} > start:{}", (void *)lineBreak,
               (void *)start);
      F_CHECKF(lineBreak < end, "lineBreak:{} < end:{}", (void *)lineBreak,
               (void *)end);
      break;
    }
  }

  // if buffer has nothing or no line break
  // do nothing
  if (readBuffer_.size() <= 0 || lineBreak == nullptr) {
    return readed;
  }

  // drain readBuffer_ to new line
  Line l;
  int len = ucnv_toUChars(converter_, );

  // return readed bytes
  return n;
}

int64_t File::loadUntil(int n) {
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
