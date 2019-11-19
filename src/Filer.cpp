// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Filer.h"
#include "Logging.h"
#include <cstdlib>

namespace fastype {

icu::UnicodeString Filer::readAll(const icu::UnicodeString &fileName,
                                  const char *locale, const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  F_CHECK(fp != nullptr, "fp {} != nullptr", (void *)fp);
  UChar *data = nullptr;
  int l = 1024, n = 0, tot = 0;

  do {
    if (data == nullptr || tot >= l) {
      l *= 2;
      data = (UChar *)realloc(data, l);
    }

    n = u_file_read(data + tot, l - tot, fp);
    tot += n;
  } while (n > 0);

  u_fclose(fp);
  fp = nullptr;

  iuc::UnicodeString ret = icu::UnicodeString(data, tot);
  free(data);
  return ret;
}

std::vector<icu::UnicodeString>
Filer::readLines(const icu::UnicodeString &fileName,
                 const char *locale = nullptr, const char *codepage = "UTF-8") {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  F_CHECK(fp != nullptr, "fp {} != nullptr", (void *)fp);
  UChar *data = nullptr;
  int l = 1024, n = 0, tot = 0;
  std::vector<icu::UnicodeString> ret;

  do {
    if (data == nullptr || tot >= l) {
      l *= 2;
      data = (UChar *)realloc(data, l);
    }

    n = u_file_read(data + tot, l - tot, fp);
    tot += n;
  } while (n > 0);

  u_fclose(fp);
  fp = nullptr;

  iuc::UnicodeString ret = icu::UnicodeString(data, tot);
  free(data);
  return ret;
}

} // namespace fastype
