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

  icu::UnicodeString ret = icu::UnicodeString(data, tot);
  free(data);
  return ret;
}

std::vector<icu::UnicodeString>
Filer::readLines(const icu::UnicodeString &fileName, const char *locale,
                 const char *codepage) {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  F_CHECK(fp != nullptr, "fp {} != nullptr", (void *)fp);
  int l = 1024, n = 0, tot = 0;
  UChar *data = (UChar *)malloc(l);
  std::vector<icu::UnicodeString> ret;

  while (true) {
    int pos = 0;
    int32_t dataLen;
    do {
      UChar *r = u_fgets(data + pos, l, fp);
      if (!r) {
        goto end_of_lines;
      }
      dataLen = u_strlen(data);
      F_CHECK(dataLen > 0, "dataLen {} > 0", dataLen);
      F_CHECK(dataLen < l, "dataLen {} < l {}", dataLen, l);
      if (dataLen < l - 1) {
        ret.push_back(icu::UnicodeString(data, dataLen - 1));
      } else {
        l *= 2;
        data = (UChar *)realloc(data, l);
        pos = dataLen - 1;
      }
    } while (dataLen >= l - 1);
  }

end_of_lines:
  u_fclose(fp);
  free(data);
  return ret;
}

} // namespace fastype
