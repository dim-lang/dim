// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#if (defined(linux) || defined(__linux) || defined(__linux__) ||               \
     defined(__GNU__) || defined(__GLIBC__)) &&                                \
    !defined(_CRAYC)
// linux:
#define F_PLATFORM_LINUX 1

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) ||   \
    defined(__DragonFly__)
// freebsd, netbsd, openbsd, dragonfly:
#define F_PLATFORM_BSD 1

#elif defined(sun) || defined(__sun)
// sun solaris:
#define F_PLATFORM_SOLARIS 1

#elif defined(__sgi)
// irix:
#define F_PLATFORM_IRIX 1

#elif defined(__hpux)
// hp unix:
#define F_PLATFORM_HPUX

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#define F_PLATFORM_WINDOWS 1

#elif defined(__HAIKU__)
// Haiku:
#define F_PLATFORM_HAIKU 1

#elif defined(__BEOS__)
// BeOS
#define F_PLATFORM_BEOS 1

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#define F_PLATFORM_MACOS 1

#elif defined(__TOS_MVS__)
// IBM z/OS
#define F_PLATFORM_ZOS 1

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM AIX
#define F_PLATFORM_AIX 1

#elif defined(__amigaos__)
// AmigaOS
#define F_PLATFORM_AMIGAOS 1

#elif defined(__QNXNTO__)
// QNX:
#define F_PLATFORM_QNX 1

#elif defined(__VXWORKS__)
// vxWorks:
#define F_PLATFORM_VXWORKDS 1

#elif defined(__VMS)
// VMS:
#define F_PLATFORM_VMS 1

#elif defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) ||            \
    defined(_POSIX_SOURCE)
// unix
#define F_PLATFORM_UNIX 1

#endif
