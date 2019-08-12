// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

#ifndef F_IN_RANGE_INC
#define F_IN_RANGE_INC(x, a, b) ((a) <= (x) && (x) <= (b))
#endif

#ifndef F_IN_RANGE_EXC
#define F_IN_RANGE_EXC(x, a, b) ((a) < (x) && (x) < (b))
#endif

#ifndef F_OUT_RANGE_INC
#define F_OUT_RANGE_INC(x, a, b) ((x) <= (a) || (x) >= (b))
#endif

#ifndef F_OUT_RANGE_EXC
#define F_OUT_RANGE_EXC(x, a, b) ((x) < (a) || (x) > (b))
#endif
