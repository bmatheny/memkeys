MAINTAINERCLEANFILES = Makefile.in

# Need for getopt_long
AM_CPPFLAGS = -D_GNU_SOURCE

AM_CFLAGS = -Wall -pedantic-errors
AM_CXXFLAGS = -Wall -pedantic-errors

if DEBUG
AM_CFLAGS += -g3 -O0
AM_CXXFLAGS += -g3 -O0
else
AM_CFLAGS += -O2
AM_CXXFLAGS += -O2
endif

#AM_LDFLAGS = -lm -lpthread -rdynamic

bin_PROGRAMS = mctop

mctop_SOURCES =			\
	exception.h options.h \
	logging/record.h logging/record.cpp \
	logging/level.h logging/level.cpp \
	logging/logger.h logging/logger.cpp \
	config.h config.cpp \
	net/device.h net/device.cpp \
	net/pcap.h net/pcap.cpp \
	net/pcap_live.h net/pcap_live.cpp \
	report/report.h report/report.cpp \
	report/text.h report/text.cpp \
	cli.h cli.cpp \
	main.cpp
