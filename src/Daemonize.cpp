// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Daemonize.h"
#include "Logging.h"
#include "boost/config.hpp"
#if BOOST_WINDOWS
#else
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
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

  umask(0);
  chdir("/");
  for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--) {
    close(i);
  }

  F_LOGGER("Daemonize");
  F_DEBUG("starting...");

  return 0;
#endif
}

void Daemonize::signalHandler(int signalNumber) {
  F_LOGGER("Daemonize");
  F_DEBUGF("signalNumber: {}", signalNumber);
}

} // namespace fastype
