// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#ifndef F_STATIC_BEGIN
#define F_STATIC_BEGIN(name)                                                   \
  class FastypeStaticBlock##name {                                             \
  public:                                                                      \
    FastypeStaticBlock##name() {
#endif

#ifndef F_STATIC_END
#define F_STATIC_END(name)                                                     \
  }                                                                            \
  }                                                                            \
  FastypeStaticBlock##name;
#endif
