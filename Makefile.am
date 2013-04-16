## Helloworld C++: an example project using Automake

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

## Define an executable target "hello", which will be installed into the
## directory named by the predefined variable $(bindir).
bin_PROGRAMS = mctop

## Define the list of source files for the "hello" target. The file extension
## .cc is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. The
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project.
mctop_SOURCES = src/options.h src/config.h src/config.cpp \
								src/main.cpp

## Define an independent executable script for inclusion in the distribution
## archive. It will not be installed on an end user's system, however.
dist_noinst_SCRIPTS = autogen.sh
