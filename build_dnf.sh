#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


echo [fastype] prepare boost, icu4c, gperftools via dnf
sudo dnf install libboost-all-dev libicu-dev libgoogle-pertools-dev
echo [fastype] prepare boost, icu4c, gperftools via dnf - done

