// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Daemonize.h"
#include "config/Platform.h"
#ifdef F_PLATFORM_WINDOWS
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

  // linux file permission has: read/write/execute for user/group/other
  // for example `ls -l` will give:
  //
  //  -1--2--3- : 1st is user, 2nd is group, 3rd is other
  // drwxr-xr-x : directory
  // drwx------ : directory
  // -rw-r--r-- : file
  // -rw------- : file
  //
  // permission can be present by number and bit operation:
  // read: 4, write: 2, execute: 1
  // so above 4 examples can be present as: 755, 700, 644, 600
  //
  // fastyped daemon handles read/write/execute directories/files
  // so set umask 002: directory permission - 0775, file permission - 664

  umask(002);
  chdir("/");
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
