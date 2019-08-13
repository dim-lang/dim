// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Daemonize.h"
#include "boost/config.hpp"
#if BOOST_WINDOWS
#else
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace fastype {

int Daemonize::daemon() {

#if BOOST_WINDOWS
#else
  pid_t pid = fork();

  // error
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  // parent process terminate
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  // child process becomes session leader
  if (setsid() < 0) {
    exit(EXIT_FAILURE);
  }

  // log SIGCHLD, SIGHUP
  signal(SIGCHLD, signalHandler);
  signal(SIGHUP, signalHandler);

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  // umask(0);
  // chdir("/");
  for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--) {
    close(i);
  }
  stdin = fopen("/dev/null", "r");
  stdout = fopen("/dev/null", "w+");
  stderr = fopen("/dev/null", "w+");

  return 0;
#endif
}

void Daemonize::signalHandler(int signalNumber) {}

} // namespace fastype
