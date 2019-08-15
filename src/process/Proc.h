// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace fastype {

typedef int64_t ProcId;
typedef void procRoutine();

class ProcManager {
public:
  static int createProcess(procRoutine routine);
  static int closeProcess(int p);
  static int size();

private:
  static void signalHandler(int signalNumber);
};

} // namespace fastype
