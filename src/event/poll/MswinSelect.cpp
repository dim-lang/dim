// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "event/poll/MswinSelect.h"

#ifdef F_EVENT_HAVE_WINSELECT

namespace fastype {

MswinSelect::MswinSelect(EventLoopImpl *evloop) {}

MswinSelect::~MswinSelect() {}

int MswinSelect::expand(int size) { return 0; }

int MswinSelect::capacity() const { return 0; }

int MswinSelect::add(int64_t fd, int event) { return 0; }

int MswinSelect::remove(int64_t fd, int event) { return 0; }

int MswinSelect::poll(int millisec) { return 0; }

std::string MswinSelect::name() const { return "MswinSelect"; }

} // namespace fastype

#endif
