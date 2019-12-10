// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include <cassert>
#include <cstdio>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <vector>

#define SZ1 10
#define SZ2 50

int writeFile(const icu::UnicodeString &fileName,
              const icu::UnicodeString &text, const char *locale = nullptr,
              const char *codepage = "UTF-8") {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), perm, locale, codepage);
  assert(fp);
  int n = u_file_write(text.getBuffer(), text.length(), fp);
  u_fclose(fp);
  return n;
}

icu::UnicodeString readFile(const icu::UnicodeString &fileName,
                            const char *locale = nullptr,
                            const char *codepage = "UTF-8") {
  UFILE *fp = u_fopen_u(fileName.getBuffer(), "r", locale, codepage);
  assert(fp);
  UChar *data = nullptr;
  int l = 1024, n = 0, tot = 0;

  do {
    if (data == nullptr || tot >= l) {
      l *= 2;
      data = (UChar *)realloc(data, l);
      assert(data);
    }
    n = u_file_read(data + tot, l - tot, fp);
    tot += n;
  } while (n > 0);
  u_fclose(fp);
  icu::UnicodeString ret = icu::UnicodeString(data, tot);
  free(data);
  return ret;
}

void getString(const std::string &fileName) {
  UFILE *fp = u_fopen("CMakeLists.txt", "r", nullptr, "UTF-8");
  assert(fp != nullptr);
  UChar buf1[SZ1];
  UChar *r1 = u_fgets(buf1, SZ1, fp);
  u_printf(
      "get line 1, SZ1: %d, buf1: %s, strlen1: %d, buf1[%d]: %c (%d), r1: %s\n",
      SZ1, buf1, u_strlen(buf1), SZ1 - 1, buf1[SZ1 - 1], (int)buf1[SZ1 - 1],
      r1);
  u_fclose(fp);

  fp = u_fopen("CMakeLists.txt", "r", nullptr, "UTF-8");
  assert(fp != nullptr);
  UChar buf2[SZ2];
  UChar *r2 = u_fgets(buf2, SZ2, fp);
  u_printf(
      "get line 1, SZ2: %d, buf2: %s, strlen2: %d, buf2[%d]: %c (%d), r2: %s\n",
      SZ2, buf2, u_strlen(buf2), SZ2 - 1, buf2[SZ2 - 1], (int)buf2[SZ2 - 1],
      r2);
  u_fclose(fp);
}

int main(int argc, char *argv[]) {
  icu::UnicodeString text =
      UNICODE_STRING_SIMPLE("Hello World, Goodbye World, This world is so "
                            "beautiful, I love it so much.\nI need a very long "
                            "string to test ICU4C library.\n");
  writeFile(UNICODE_STRING_SIMPLE("icu_ustdio_write.log"), text);
  icu::UnicodeString text2 =
      readFile(UNICODE_STRING_SIMPLE("icu_ustdio_write.log"));
  assert((int)text.compare(text2) == 0);
  getString("icu_ustdio_write.log");
  return 0;
}
