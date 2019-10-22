// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Line.h"
#include "SmartPointer.h"
#include "Stringify.h"
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <vector>

namespace fastype {

// Buffer is a file's memory storage
class Buffer : public Stringify {
public:
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(Buffer &&) = delete;
  virtual ~Buffer();

  const icu::UnicodeString &fileName() const;
  Line get(int lineNumber);
  int count();
  bool empty();
  int loaded() const;
  int truncate(int start, int length);
  int clear();
  virtual std::string toString() const;

  static Sptr<Buffer> open(const icu::UnicodeString &fileName);
  static void close(Sptr<Buffer> file);

private:
  Buffer(const icu::UnicodeString &fileName);
  void expand(int n);
  static char *ucharToString(UChar *s, int sn, char *d, int dn);

  // load all lines
  // @return readed bytes
  int64_t load();

  icu::UnicodeString fileName_;
  UFILE *fp_;
  bool loaded_;
  UChar *buf_;
  int bufsize_;
  std::vector<Line> lineList_;
};

} // namespace fastype
