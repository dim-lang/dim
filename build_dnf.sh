#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


sudo echo [fastype] prepare boost, icu4c, gperftools via dnf || { echo [fastype] sudo not found; exit 1; }
sudo dnf install boost-devel icu-devel gperftools-devel
echo [fastype] prepare boost, icu4c, gperftools via dnf - done

