// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <string>

std::string tokenName(int value);

int tokenValue(const std::string &name);

bool tokenValid(int token);

bool tokenValid(const std::string &name);
