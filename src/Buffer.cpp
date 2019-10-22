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
#include <unicode/utypes.h>

#define F_BUF_SIZE 8192
#define F_BUF_TO_STRING_SIZE 128
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
  return *this;
}

const icu::UnicodeString &Buffer::fileName() const { return fileName_; }

Sptr<Buffer> Buffer::open(const icu::UnicodeString &fileName) {
  return Sptr<Buffer>(new Buffer(fileName));
}

void Buffer::close(Sptr<Buffer> file) { file.reset(); }

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
  int32_t cap;
  UErrorCode err;
  char _2[F_BUF_TO_STRING_SIZE];
  u_strToUTF8(_2, F_BUF_TO_STRING_SIZE, &cap, buf_, F_BUF_TO_STRING_SIZE, &err);
  if (U_FAILURE(err)) {
    F_ERROR("buf_ to UTF8 failure, buf_:{}, err:{}, errorName:{}", (void *)buf_,
            err, u_errorName(err));
    F_THROW(UnicodeException,
            "buf_ to UTF8 failure, buf_:{}, err:{}, errorName:{}", (void *)buf_,
            err, u_errorName(err));
  }
  _2[F_BUF_TO_STRING_SIZE - 4] = '.';
  _2[F_BUF_TO_STRING_SIZE - 3] = '.';
  _2[F_BUF_TO_STRING_SIZE - 2] = '.';
  _2[F_BUF_TO_STRING_SIZE - 1] = '\0';
  return fmt::format("[ @Buffer fileName_:{} fp_:{} loaded_:{} buf_:{} "
                     "lineList_#size:{} ]",
                     fileName_.toUTF8String(_1), (void *)fp_, loaded_, _2,
                     lineList_.size());
}

int64_t Buffer::load() {
  if (loaded_) {
    // already EOF
    return 0L;
  }

  int64_t readed = 0L;

  int pos = 0, n;
  while (!loaded_) {
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

  F_INFO("buf_:{}, lineList_#size:{}", buf_.toString(), lineList_.size());
  // if buffer has nothing
  if (buf_.size() <= 0) {
    return readed;
  }

  // split buf_ to lines
  char *start = buf_.head();
  char *end = buf_.head() + buf_.size();
  while (true) {
    if (start >= end) {
      F_INFO("start:{} >= end:{}", (void *)start, (void *)end);
      break;
    }
    char *lineBreak = std::find(start, end, '\n');
    F_INFO("start:{} lineBreak:{} end:{} lineBreak-start:{} end-start:{} "
           "end-lineBreak:{}",
           (void *)start, (void *)lineBreak, (void *)end,
           (int)(lineBreak - start), (int)(end - start),
           (int)(end - lineBreak));
    if (lineBreak >= end) {
      F_INFO("lineBreak:{} >= end:{}", (void *)lineBreak, (void *)end);
      break;
    }

    int sz = lineBreak - start + 1; // 1 is for '\n'
    char ef = '\0';
    // Cowstr cs(sz);
    // cs.concat(start, sz);
    // cs.concat(&ef, 1);
    // Row r(cs, lineList_.size(), false); // 1 is for '\0'
    Line r;
    F_INFO("new line:{}", r.toString());
    lineList_.push_back(r);
    start = lineBreak + 1;
  }

  // F_INFO("buffer read:{} elapse:{}", readed, F_TIMER_ELAPSE(load));
  return readed;
}

} // namespace fastype

#undef F_BUF_SIZE
#undef F_BUF_TO_STRING_SIZE
