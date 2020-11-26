// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "infra/CycleBuffer.h"
#include "catch2/catch.hpp"
#include "fmt/format.h"
#include "infra/Log.h"
#include <algorithm>
#include <deque>

#define C_MIN 0
#define C_MAX 100

TEST_CASE("CycleBuffer", "[CycleBuffer]") {
  SECTION("attribute") {
    {
      DynamicCycleBuffer db;
      REQUIRE(db.capacity() == 0);
      REQUIRE(db.size() == 0);
      REQUIRE(db.empty());
      REQUIRE(db.full());
    }
    {
      DynamicCycleBuffer db(C_MAX);
      REQUIRE(db.capacity() >= C_MAX);
      REQUIRE(db.size() == 0);
      REQUIRE(db.empty());
      REQUIRE(!db.full());
    }
    {
      FixedCycleBuffer fb(C_MAX);
      REQUIRE(fb.capacity() == C_MAX);
      REQUIRE(fb.size() == 0);
      REQUIRE(fb.empty());
      REQUIRE(!fb.full());
    }
  }
  SECTION("foreach") {
    {
      DynamicCycleBuffer db;
      LOG_INFO("db-1: {}", db.str());
      char c;
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        REQUIRE(db.write(&c, 1) == 0);
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        c = (char)i;
        REQUIRE(db.read(&c, 1) == 1);
      }
      LOG_INFO("db-2: {}", db.str());
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
      FixedCycleBuffer fb(C_MAX);
      LOG_INFO("fb-1: {}", fb.str());
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
      LOG_INFO("fb-2: {}", fb.str());
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
      // DynamicCycleBuffer single byte read
      DynamicCycleBuffer db;
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
      // DynamicCycleBuffer block bytes read
      DynamicCycleBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        REQUIRE(db.read(buf, i + 1) == i + 1);
        delete[] buf;
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
      // FixedCycleBuffer single byte read
      FixedCycleBuffer fb(C_MAX);
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
      // FixedCycleBuffer block bytes read
      int count = 0;
      FixedCycleBuffer fb(C_MAX);
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
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
        delete[] buf;
      }
      const char *cp = fb.begin();
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
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
        delete[] buf;
      }
      count = 0;
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
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
        delete[] buf;
      }
    }
    {
      // DynamicCycleBuffer single byte write
      DynamicCycleBuffer db;
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
      // DynamicCycleBuffer block bytes write
      DynamicCycleBuffer db;
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
        for (int j = 0; j < i + 1; j++) {
          buf[j] = (char)i;
        }
        REQUIRE(db.read(buf, i + 1) == i + 1);
        delete[] buf;
      }
      for (int i = C_MIN; i < C_MAX; i++) {
        char *buf = new char[i + 1];
        REQUIRE(db.write(buf, i + 1) == i + 1);
        for (int j = 0; j < i + 1; j++) {
          REQUIRE((int)buf[j] == i);
        }
        delete[] buf;
      }
    }
  }
  SECTION("random read/write") {
    // fake random integers
    const std::vector<int> randint = {
        14, 3,  89, 77, 38, 38, 90, 90, 15, 2,  82, 38,  39, 19, 9,  1,  100,
        29, 39, 32, 45, 18, 85, 57, 14, 3,  99, 17, 38,  38, 90, 90, 15, 92,
        82, 38, 39, 19, 9,  7,  19, 13, 39, 33, 15, 88,  85, 57, 18, 85, 57,
        14, 3,  19, 9,  7,  19, 13, 83, 71, 74, 91, 73,  81, 1,  38, 29, 27,
        89, 77, 38, 38, 32, 45, 18, 85, 39, 19, 9,  7,   19, 9,  7,  19, 99,
        14, 3,  19, 9,  7,  19, 13, 83, 71, 74, 91, 73,  81, 1,  38, 29, 27,
        89, 77, 38, 38, 32, 45, 18, 85, 39, 19, 9,  7,   19, 9,  7,  19, 99,
        17, 38, 33, 15, 88, 74, 91, 73, 19, 9,  7,  100, 29, 15, 92, 18, 85,
        17, 38, 33, 15, 88, 74, 91, 73, 19, 9,  7,  100, 29, 15, 92, 18, 85,
        14, 3,  89, 77, 38, 38, 90, 90, 15, 2,  82, 38,  39, 19, 9,  1,  100,
        29, 39, 32, 45, 18, 85, 57, 14, 3,  99, 17, 38,  38, 90, 90, 15, 92,
        82, 38, 39, 19, 9,  7,  19, 13, 39, 33, 15, 88,  85, 57, 18, 85, 57,
    };
    {
      std::deque<char> q;
      DynamicCycleBuffer db;
      int randc = 0;
      for (int i = C_MIN; i < C_MAX; i++) {
        int rn = randint[randc++];
        std::vector<char> rbuf(rn);
        for (int j = 0; j < rn; j++) {
          rbuf[j] = (char)rn;
          q.push_back(rbuf[j]);
        }
        REQUIRE(db.read(rbuf.data(), rn) == rn);
        int wn = std::min(randint[randc++], rn);
        std::vector<char> wbuf(wn);
        REQUIRE(db.write(wbuf.data(), wn) == wn);
        for (int j = 0; j < wn; j++) {
          REQUIRE(wbuf[j] == q.front());
          q.pop_front();
        }
      }
    }
    {
      std::deque<char> q;
      FixedCycleBuffer fb(C_MAX);
      int randc = 0;
      for (int i = C_MIN; i < C_MAX; i++) {
        int rn = std::min(randint[randc++], fb.capacity() - fb.size());
        std::vector<char> rbuf(rn);
        for (int j = 0; j < rn; j++) {
          rbuf[j] = (char)rn;
          q.push_back(rbuf[j]);
        }
        REQUIRE(fb.read(rbuf.data(), rn) == rn);
        int wn = std::min(randint[randc++], fb.size());
        std::vector<char> wbuf(wn);
        REQUIRE(fb.write(wbuf.data(), wn) == wn);
        for (int j = 0; j < wn; j++) {
          REQUIRE(wbuf[j] == q.front());
          q.pop_front();
        }
      }
    }
  }
}
