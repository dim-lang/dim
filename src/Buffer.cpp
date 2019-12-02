// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Buffer.h"
#include "Logging.h"
#include "exception/BadAllocException.h"
#include "exception/UnicodeException.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fmt/format.h>
#include <numeric>
#include <string>

#define F_BUF_SIZE 8192
#define F_BUF_TO_STRING_SIZE 32
#define F_ALLOC_UNIT 8

namespace fastype {

Buffer::Buffer(const icu::UnicodeString &fileName)
    : fileName_(fileName),
      fp_(u_fopen_u(fileName.getBuffer(), "rw", nullptr, "UTF-8")),
      loaded_(false), buf_(nullptr), bufsize_(0) {
  expand(F_BUF_SIZE);
  F_CHECK(buf_ != nullptr, "buf_ {} != nullptr", (void *)buf_);
  F_INFO("Constructor:{}", toString());
}

Buffer::~Buffer() {
  F_INFO("Destructor:{}", toString());
  if (fp_) {
    u_fclose(fp_);
    fp_ = nullptr;
  }
  if (buf_) {
    std::free(buf_);
    buf_ = nullptr;
  }
  bufsize_ = 0;
  loaded_ = false;
  for (int i = 0; i < lineList_.size(); i++) {
    lineList_[i] = Line();
  }
  lineList_.clear();
}

void Buffer::expand(int n) {
  // do nothing
  if (n <= 0) {
    return;
  }
  if (n <= bufsize_) {
    return;
  }

  UChar *nb = (UChar *)realloc(buf_, n * sizeof(UChar));
  if (!nb) {
    F_ERROR("realloc fail! buf_:{}, n:{}", (void *)buf_, n);
    F_THROW(BadAllocException, "realloc fail! buf_:{}, n:{}", (void *)buf_, n);
  }
  std::memset(nb, 0, sizeof(UChar) * n);
  bufsize_ = n;
}

char *Buffer::ucharToString(UChar *s, int sn, char *d, int dn) {
  F_CHECK(s != nullptr, "s {} != nullptr", (void *)s);
  F_CHECK(d != nullptr, "d {} != nullptr", (void *)d);
  UErrorCode err;
  int32_t len;
  u_strToUTF8(d, dn, &len, s, std::min(dn, sn), &err);
  if (U_FAILURE(err)) {
    F_ERROR("s to UTF8 failure, s:{}, err:{}, errorName:{}", (void *)s, err,
            u_errorName(err));
    return nullptr;
  }
  F_CHECK(len <= dn, "len {} <= dn {}", len, dn);
  d[dn - 1] = '\0';
  return d;
}

const icu::UnicodeString &Buffer::fileName() const { return fileName_; }

std::shared_ptr<Buffer> Buffer::open(const icu::UnicodeString &fileName) {
  return std::shared_ptr<Buffer>(new Buffer(fileName));
}

void Buffer::close(std::shared_ptr<Buffer> file) { file.reset(); }

Line Buffer::get(int lineNumber) { return lineList_[lineNumber]; }

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
  std::string _1;
  char _2[F_BUF_TO_STRING_SIZE];
  if (ucharToString(buf_, bufsize_, _2, F_BUF_TO_STRING_SIZE)) {
    return fmt::format(
        "[ @Buffer fileName_:{} fp_:{} loaded_:{} buf_:{} bufsize_:{} "
        "lineList_#size:{} ]",
        fileName_.toUTF8String(_1), (void *)fp_, loaded_, _2, bufsize_,
        lineList_.size());
  } else {
    return fmt::format(
        "[ @Buffer fileName_:{} fp_:{} loaded_:{} buf_:{} bufsize_:{} "
        "lineList_#size:{} ]",
        fileName_.toUTF8String(_1), (void *)fp_, loaded_, (void *)nullptr,
        bufsize_, lineList_.size());
  }
}

int64_t Buffer::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;

  int pos = 0, n;
  while (true) {
    UChar *rr = u_fgets(buf_ + pos, bufsize_ - 1 - pos, fp_);

    // no more chars
    if (!rr) {
      loaded_ = true;
      break;
    }

    n = u_strlen(buf_ + pos);

    // not enough space for one line
    if (n == bufsize_ - 1 - pos) {
      expand(bufsize_ * 2);
      pos = bufsize_ - 1;
      bufsize_ = bufsize_ * 2;
    } else {
      break;
    }
  }

  readed += u_strlen(buf_);

  char _1[F_BUF_TO_STRING_SIZE];
  if (ucharToString(buf_, bufsize_, _1, F_BUF_TO_STRING_SIZE)) {
    F_INFO("buf_:{}, lineList_#size:{}", _1, lineList_.size());
  } else {
    F_INFO("buf_:{}, lineList_#size:{}", (void *)nullptr, lineList_.size());
  }

  // if read nothing
  if (readed <= 0L) {
    return readed;
  }

  lineList_.push_back(
      Line(icu::UnicodeString(buf_, readed), lineList_.size(), false));

  F_INFO("load:{}", toString());
  return readed;
}

} // namespace fastype

#undef F_BUF_SIZE
#undef F_BUF_TO_STRING_SIZE
