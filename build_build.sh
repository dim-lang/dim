#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0

ROOT=`pwd`
DEBUG=debug
RELEASE=release

cd $ROOT
if [ ! -d $DEBUG ]; then mkdir $DEBUG; fi
if [ ! -d $RELEASE ]; then mkdir $RELEASE; fi

OS="Unknown-OS"
case "$OSTYPE" in
  solaris*) OS="Solaris" ;;
  darwin*)  OS="MacOSX" ;;
  linux*)   OS="Linux" ;;
  bsd*)     OS="BSD" ;;
  *)        echo "[fastype] error! unknown *$OSTYPE* !" && exit 3 ;;
esac
echo [fastype] prepare for $OS

cd $DEBUG && cmake -DF_OS=$OS -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON .. && make VERBOSE=1 && cd $ROOT
cd $RELEASE && cmake -DF_OS=$OS -DCMAKE_BUILD_TYPE=Release .. && make -j8 && cd $ROOT
