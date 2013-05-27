#!/bin/bash

function cleanup_src() {
  rm -rf Makefile.in \
        aclocal.m4 autom4te.cache/ config.guess config.sub \
        configure depcomp install-sh ltmain.sh mconfig.h.in \
        missing src/.deps src/Makefile src/Makefile.in \
        config config.* libtool *~ stamp-h1;
  find . -type f -name \*.o -exec rm -f {} \;
  find . -type f -name Makefile -exec rm -f {} \;
  find . -type d -name .deps -exec rm -rf {} \;
  find . -type f -name .dirstamp -exec rm -f {} \;
  find . -type d -name .libs -exec rm -rf {} \;
}

function cleanup_gtest() {
  cd gtest
  git reset --hard
  git clean -f
  rm -rf CMakeFiles/
  cd ..
}

cleanup_src
cleanup_gtest
