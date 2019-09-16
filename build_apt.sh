#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


sudo echo [fastype] prepare boost, icu4c, gperftools via apt || { echo [fastype] sudo not found; exit 1; }
sudo apt-get install libboost-all-dev libicu-dev libgoogle-pertools-dev
echo [fastype] prepare boost, icu4c, gperftools via apt - done
