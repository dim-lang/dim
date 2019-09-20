#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


echo [fastype] prepare compiler and toolchain for macOS
brew install gcc cmake automake autoconf
echo [fastype] prepare compiler and toolchain for macOS - done
echo [fastype] prepare spdlog, fmt, boost, icu4c, gperftools via homebrew
brew install spdlog fmt boost icu4c gperftools
echo [fastype] prepare spdlog, fmt, boost, icu4c, gperftools via homebrew - done
