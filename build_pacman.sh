#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


sudo echo [fastype] prepare boost, icu, gperftools via pacman || { echo [fastype] sudo not found; exit 1; }
yes | sudo pacman -S boost icu gperftools
echo [fastype] prepare boost, icu, gperftools via pacman - done
