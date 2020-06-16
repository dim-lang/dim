// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "container/CycleBuffer.h"
#include "Log.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include <algorithm>
#include <deque>

#define C_MIN 0
#define C_MAX 100

TEST_CASE("container/CycleBuffer", "[container/CycleBuffer]") {
  SECTION("attribute") {
    {
      DynamicBuffer db;
      REQUIRE(db.capacity() == 0);
      REQUIRE(db.size() == 0);
      REQUIRE(db.empty());
      REQUIRE(db.full());
    }
    {
      DynamicBuffer db(C_MAX);
      REQUIRE(db.capacity() >= C_MAX);
      REQUIRE(db.size() == 0);
      REQUIRE(db.empty());
      REQUIRE(!db.full());
    }
    {
      FixedBuffer fb(C_MAX);
      REQUIRE(fb.capacity() == C_MAX);
      REQUIRE(fb.size() == 0);
      REQUIRE(fb.empty());
      REQUIRE(!fb.full());
    }
  }
  SECTION("foreach") {
    {
      DynamicBuffer db;
      LOG_INFO("db-1: {}", db.toString());
      char c;
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        REQUIRE(db.write(&c, 1) == 0);
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        REQUIRE(db.read(&c, 1) == 1);
      }
      LOG_INFO("db-2: {}", db.toString());
      const char *cp = db.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        REQUIRE((int)*cp == i);
        cp = db.next(cp);
      }
      cp = db.rbegin();
      for (int i = C_MAX - 1; i >= C_MIN; i--) {
        REQUIRE((int)*cp == i);
        cp = db.prev(cp);
      }
      int i = C_MAX - 1;
      for (char *mp = db.begin(); mp != db.end(); mp = db.next(mp), i--) {
        *mp = (char)i;
      }
      for (int i = C_MAX - 1; i >= 0; i--) {
        REQUIRE(db.write(&c, 1) == 1);
        REQUIRE((int)c == i);
      }
    }
    {
      FixedBuffer fb(C_MAX);
      LOG_INFO("fb-1: {}", fb.toString());
      char c;
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        REQUIRE(fb.write(&c, 1) == 0);
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        fmt::format("i:{}", i);
        REQUIRE(fb.read(&c, 1) == 1);
      }
      LOG_INFO("fb-2: {}", fb.toString());
      const char *cp = fb.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        REQUIRE((int)*cp == i);
        cp = fb.next(cp);
      }
      cp = fb.rbegin();
      for (int i = C_MAX - 1; i >= C_MIN; i--) {
        REQUIRE((int)*cp == i);
        cp = fb.prev(cp);
      }
      int v = C_MAX - 1;
      for (char *mp = fb.begin(); mp != fb.end(); mp = fb.next(mp), v--) {
        *mp = (char)v;
      }
      for (int i = C_MAX - 1; i >= 0; i--) {
        REQUIRE(fb.write(&c, 1) == 1);
        REQUIRE((int)c == i);
      }
    }
  }
  SECTION("read/write") {
    {
      // DynamicBuffer single byte read
      DynamicBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char c = (char)i;
        REQUIRE(db.read(&c, 1) == 1);
      }
      const char *cp = db.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        REQUIRE((int)*cp == i);
        cp = db.next(cp);
      }
      cp = db.rbegin();
      for (int i = C_MAX - 1; i >= C_MIN; i--) {
        REQUIRE((int)*cp == i);
        cp = db.prev(cp);
      }
    }
    {
      // DynamicBuffer block bytes read
      DynamicBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        REQUIRE(db.read(buf, i + 1) == i + 1);
      }
      const char *cp = db.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        for (int j = 0; j < i + 1; j++) {
          REQUIRE((int)*cp == i);
          cp = db.next(cp);
        }
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        for (int j = 0; j < i + 1; j++) {
          char c;
          REQUIRE(db.write(&c, 1) == 1);
          REQUIRE((int)c == i);
        }
      }
    }
    {
      // FixedBuffer single byte read
      FixedBuffer fb(C_MAX);
      for (int i = C_MIN; i < C_MAX; i++) {
        char c = (char)i;
        REQUIRE(fb.read(&c, 1) == 1);
      }
      const char *cp = fb.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        REQUIRE((int)*cp == i);
        cp = fb.next(cp);
      }
      cp = fb.rbegin();
      for (int i = C_MAX - 1; i >= C_MIN; i--) {
        REQUIRE((int)*cp == i);
        cp = fb.prev(cp);
      }
    }
    {
      // FixedBuffer block bytes read
      int count = 0;
      FixedBuffer fb(C_MAX);
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        if (count + i + 1 <= fb.capacity()) {
          REQUIRE(fb.read(buf, i + 1) == i + 1);
          count += i + 1;
        } else {
          int oldcap = fb.capacity();
          REQUIRE(fb.read(buf, i + 1) == oldcap - count);
          count += oldcap - count;
          REQUIRE(count == C_MAX);
          break;
        }
      }
      const char *cp = fb.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        for (int j = 0; j < i + 1; j++) {
          count -= 1;
          if (count < 0) {
            break;
          }
          REQUIRE((int)*cp == i);
          cp = fb.next(cp);
        }
      }
      count = 0;
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        if (fb.size() > i + 1) {
          REQUIRE(fb.write(buf, i + 1) == i + 1);
          for (int j = 0; j < i + 1; j++) {
            REQUIRE((int)buf[j] == i);
          }
          count += i + 1;
        } else {
          int oldsz = fb.size();
          REQUIRE(fb.write(buf, i + 1) == oldsz);
          for (int j = 0; j < i + 1; j++) {
            REQUIRE((int)buf[j] == i);
          }
          REQUIRE(count + oldsz == C_MAX);
          break;
        }
      }
    }
    {
      // DynamicBuffer single byte write
      DynamicBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char c = (char)i;
        REQUIRE(db.read(&c, 1) == 1);
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        char c;
        REQUIRE(db.write(&c, 1) == 1);
        REQUIRE((int)c == i);
      }
    }
    {
      // DynamicBuffer block bytes write
      DynamicBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        REQUIRE(db.read(buf, i + 1) == i + 1);
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        char buf[i + 1];
        REQUIRE(db.write(buf, i + 1) == i + 1);
        for (int j = 0; j < i + 1; j++) {
          REQUIRE((int)buf[j] == i);
        }
      }
    }
  }
  SECTION("random read/write") {
    {
      std::deque<char> dq;
      DynamicBuffer db;
      RandomInt<int> randint(C_MIN, C_MAX);
      for (int i = C_MIN; i < C_MAX; i++) {
        int rn = randint.next();
        char *rbuf = new char[rn];
        for (int j = 0; j < rn; j++) {
          rbuf[j] = (char)rn;
          dq.push_back((char)rn);
        }
        REQUIRE(db.read(rbuf, rn) == rn);
        int wn = std::min(randint.next(), rn);
        char *wbuf = new char[wn];
        REQUIRE(db.write(wbuf, wn) == wn);
        for (int j = 0; j < wn; j++) {
          REQUIRE(wbuf[j] == dq.front());
          REQUIRE(!dq.empty());
          dq.pop_front();
        }
        delete[] rbuf;
        delete[] wbuf;
      }
    }
    {
      std::deque<char> dq;
      FixedBuffer fb(C_MAX);
      RandomInt<int> randint(C_MIN, C_MAX);
      for (int i = C_MIN; i < C_MAX; i++) {
        int rn = randint.next();
        rn = std::min(randint.next(), fb.capacity() - fb.size());
        char *rbuf = new char[rn];
        for (int j = 0; j < rn; j++) {
          rbuf[j] = (char)rn;
          dq.push_back((char)rn);
        }
        REQUIRE(fb.read(rbuf, rn) == rn);
        int wn = std::min(randint.next(), rn);
        char *wbuf = new char[wn];
        REQUIRE(fb.write(wbuf, wn) == wn);
        for (int j = 0; j < wn; j++) {
          REQUIRE(wbuf[j] == dq.front());
          REQUIRE(!dq.empty());
          dq.pop_front();
        }
        delete[] rbuf;
        delete[] wbuf;
      }
    }
  }
}
