#!/bin/bash

# Record where I am
BUILD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )";
PROJ_DIR=$(dirname $BUILD_DIR);

# Move up one level
cd $PROJ_DIR

# Initialize (if needed) and update submodules
git submodule update --init --recursive

# Autoreconf to generate build scripts and such
autoreconf --force --install --verbose "$PROJ_DIR"
test -n "$NOCONFIGURE" || "$PROJ_DIR/configure" "$@"

cd $BUILD_DIR
