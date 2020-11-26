// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once

enum class Op { Add = 1, Sub, Mul, Div };
const char *getOpTmp(Op op);
const char *getOpName(Op op);
