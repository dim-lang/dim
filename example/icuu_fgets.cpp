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

int main(int argc, char *argv[]) {
  UFILE *fp = u_fopen("CMakeLists.txt", "r", nullptr, "UTF-8");
  assert(fp != nullptr);
  UChar buf1[SZ1];
  UChar *r1 = u_fgets(buf1, SZ1, fp);
  u_printf("get line 1, SZ1: %d, buf1: %s, buf1[%d]: %c, r1: %s\n", SZ1, buf1,
           SZ1 - 1, buf1[SZ1 - 1], r1);
  u_fclose(fp);

  fp = u_fopen("CMakeLists.txt", "r", nullptr, "UTF-8");
  assert(fp != nullptr);
  UChar buf2[SZ2];
  UChar *r2 = u_fgets(buf2, SZ2, fp);
  u_printf("get line 1, SZ1: %d, buf: %s, buf[%d]: %c, r2: %s\n", SZ2, buf2,
           SZ2 - 1, buf2[SZ2 - 1], r2);
  u_fclose(fp);
  return 0;
}
