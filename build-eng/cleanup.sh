#!/bin/bash

# Record where I am
BUILD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )";
SRC_DIR=$(dirname $BUILD_DIR);

function cleanup_src() {
  cd $SRC_DIR;
  if [ -f Makefile ]; then
    make distclean
  fi
  rm -rf build-aux
  rm -f m4/*.m4
  rm -f src/stamp-h1
  rm -rf aclocal.m4 autom4te.cache/ configure depcomp \
          install-sh ltmain.sh missing config config.* libtool *~;
  find . -type f -name Makefile.in -exec rm -f {} \;
  find . -type f -name \*.o -exec rm -f {} \;
  find . -type f -name Makefile -exec rm -f {} \;
  find . -type d -name .deps -exec rm -rf {} \;
  find . -type f -name .dirstamp -exec rm -f {} \;
  find . -type d -name .libs -exec rm -rf {} \;
  cd $BUILD_DIR;
}

function cleanup_gtest() {
  cd $SRC_DIR/gtest;
  git reset --hard
  git clean -f
  rm -rf CMakeFiles/ autom4te.cache/
  cd $BUILD_DIR;
}

cleanup_src
cleanup_gtest
