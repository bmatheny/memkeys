MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure mconfig.h.in mconfig.h.in~ stamp-h.in

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

# Keep objects in separate directories when possible
AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = src

dist_noinst_SCRIPTS = autogen.sh

EXTRA_DIST = README.md NOTICE LICENSE

test: all
