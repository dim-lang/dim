// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

namespace fastype {

class Daemonize {
public:
  static int daemon();

private:
  static void signalHandler(int signalNumber);
};

} // namespace fastype
