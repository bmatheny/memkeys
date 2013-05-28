MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = subdir-objects

# Need for getopt_long
AM_CPPFLAGS = -D_GNU_SOURCE

# Always build with Wall and pedantic errors
AM_CFLAGS = -Wall -pedantic-errors
AM_CXXFLAGS = -Wall -pedantic-errors

# Different optimization levels
if DEBUGGING
AM_CFLAGS += -g3 -O0
AM_CXXFLAGS += -g3 -O0
else
AM_CFLAGS += -O2
AM_CXXFLAGS += -O2
endif

if DEVELOPMENT
AM_CFLAGS += -Wextra
AM_CXXFLAGS += -Wextra -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare
endif

# Build a dummy library so we can link tests against it
noinst_LTLIBRARIES = libmemkeys.la
libmemkeys_la_SOURCES = \
	util/util.h \
	util/backoff.h util/backoff.cpp \
	util/exceptions.h \
	util/mqueue.h \
	util/options.h \
	util/util_time.h \
	util/stat.h util/stat.cpp \
	util/stats.h util/stats.cpp \
	util/state.h util/state.cpp \
	logging/record.h logging/record.cpp \
	logging/level.h logging/level.cpp \
	logging/logger.h logging/logger.cpp \
	config.h config.cpp \
	net/device.h net/device.cpp \
	net/pcap.h net/pcap.cpp \
	net/packet.h net/packet.cpp \
	net/pcap_live.h net/pcap_live.cpp \
	report/report_type.h report/report_type.cpp \
	report/report.h report/report.cpp \
	report/csv.h report/csv.cpp \
	report/curses.h report/curses.cpp \
	net/memcache_command.h net/memcache_command.cpp \
	net/capture_engine.h net/capture_engine.cpp \
	cli.h cli.cpp \
	memkeys.h memkeys.cpp

# memkeys binary
bin_PROGRAMS = memkeys
memkeys_LDADD = libmemkeys.la
memkeys_SOURCES =	main.cpp

# Magic needed for linking the dummy memkeys library for building our test suite
check_PROGRAMS = testall
testall_LDADD = libmemkeys.la \
								$(top_builddir)/gtest/lib/libgtest.la \
								$(top_builddir)/gtest/lib/libgtest_main.la
testall_CXXFLAGS = -I$(top_srcdir)/gtest/include \
								-I$(top_builddir)/gtest/include
testall_SOURCES = logging/test_level.cpp \
									logging/test_record.cpp

TESTS = testall
