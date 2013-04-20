MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = subdir-objects

# Need for getopt_long
AM_CPPFLAGS = -D_GNU_SOURCE

AM_CFLAGS = -Wall -pedantic-errors
AM_CXXFLAGS = -Wall -pedantic-errors

if DEBUGGING
AM_CFLAGS += -g3 -O0
AM_CXXFLAGS += -g3 -O0
else
AM_CFLAGS += -O2
AM_CXXFLAGS += -O2
endif

bin_PROGRAMS = mctop

mctop_SOURCES =			\
	util/util.h \
	util/exceptions.h \
	util/options.h \
	util/state.h util/state.cpp \
	logging/record.h logging/record.cpp \
	logging/level.h logging/level.cpp \
	logging/logger.h logging/logger.cpp \
	config.h config.cpp \
	net/device.h net/device.cpp \
	net/pcap.h net/pcap.cpp \
	net/pcap_live.h net/pcap_live.cpp \
	report/report.h report/report.cpp \
	report/text.h report/text.cpp \
	net/memcache_command.h net/memcache_command.cpp \
	net/capture_engine.h net/capture_engine.cpp \
	cli.h cli.cpp \
	mctop.h mctop.cpp \
	main.cpp
