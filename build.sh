#! /usr/bin/env bash
# Copyright 2019- <fastype.org>
# Apache License Version 2.0


OS="Unknown-OS"
ROOT=`pwd`
case "$OSTYPE" in
  solaris*) OS="Solaris" ;;
  darwin*)  OS="MacOSX" ;; 
  linux*)   OS="Linux" && ./build_linux.sh ;;
  bsd*)     OS="BSD" ;;
  *)        echo "[fastype] Unknown *$OSTYPE* Not Support!" && exit 3 ;;
esac
echo [fastype] Build for $OS

echo [fastype] prepare catch2 v2.9.1
if [ ! -d test/catch2 ]; then
    cd test && git clone -b 'v2.9.1' --single-branch --depth 1 https://github.com/catchorg/Catch2.git && cd ..
fi
echo [fastype] prepare catch2 v2.9.1 - done

# build
DEBUG=debug
RELEASE=release
cd $ROOT
if [ ! -d $DEBUG ]; then mkdir $DEBUG; fi
if [ ! -d $RELEASE ]; then mkdir $RELEASE; fi
cd $DEBUG && cmake -DF_OS=$OS -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON .. && make VERBOSE=1 && cd $ROOT
cd $RELEASE && cmake -DF_OS=$OS -DCMAKE_BUILD_TYPE=Release .. && make -j8 && cd $ROOT
