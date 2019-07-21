// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#ifndef F_STATIC_BLOCK_BEG
#define F_STATIC_BLOCK_BEG(name)                                               \
  class FastypeStaticBlock##name {                                             \
  public:                                                                      \
    FastypeStaticBlock##name() {
#endif

#ifndef F_STATIC_BLOCK_END
#define F_STATIC_BLOCK_END(name)                                               \
  }                                                                            \
  }                                                                            \
  FastypeStaticBlock##name;
#endif
