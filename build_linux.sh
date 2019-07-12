#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


echo [fastype] prepare spdlog v1.3.1
if [ ! -d src/spdlog ]; then
    cd src && git clone -b 'v1.3.1' --single-branch --depth 1 https://github.com/gabime/spdlog.git && cd ..
fi
echo [fastype] prepare spdlog v1.3.1 - done
