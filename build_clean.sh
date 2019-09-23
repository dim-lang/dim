#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0

function clean_macos() {
    rm *.log
    cd debug && make clean && cd ..
    cd release && make clean && cd ..
}
