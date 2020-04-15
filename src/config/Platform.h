// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#pragma once

#if (defined(linux) || defined(__linux) || defined(__linux__) ||               \
     defined(__GNU__) || defined(__GLIBC__)) &&                                \
    !defined(_CRAYC)
// linux:
#define PLATFORM_LINUX 1

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) ||   \
    defined(__DragonFly__)
// freebsd, netbsd, openbsd, dragonfly:
#define PLATFORM_BSD 1

#elif defined(sun) || defined(__sun)
// sun solaris:
#define PLATFORM_SOLARIS 1

#elif defined(__sgi)
// irix:
#define PLATFORM_IRIX 1

#elif defined(__hpux)
// hp unix:
#define PLATFORM_HPUX

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#define PLATFORM_WINDOWS 1

#elif defined(__HAIKU__)
// Haiku:
#define PLATFORM_HAIKU 1

#elif defined(__BEOS__)
// BeOS
#define PLATFORM_BEOS 1

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#define PLATFORM_MACOS 1

#elif defined(__TOS_MVS__)
// IBM z/OS
#define PLATFORM_ZOS 1

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM AIX
#define PLATFORM_AIX 1

#elif defined(__amigaos__)
// AmigaOS
#define PLATFORM_AMIGAOS 1

#elif defined(__QNXNTO__)
// QNX:
#define PLATFORM_QNX 1

#elif defined(__VXWORKS__)
// vxWorks:
#define PLATFORM_VXWORKDS 1

#elif defined(__VMS)
// VMS:
#define PLATFORM_VMS 1

#elif defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) ||            \
    defined(_POSIX_SOURCE)
// unix
#define PLATFORM_UNIX 1

#endif
