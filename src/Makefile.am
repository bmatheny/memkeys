MAINTAINERCLEANFILES = Makefile.in

# Need for getopt_long
AM_CPPFLAGS = -D_GNU_SOURCE

AM_CFLAGS = -Wall -ggdb -pedantic-errors

#AM_LDFLAGS = -lm -lpthread -rdynamic

bin_PROGRAMS = mctop

mctop_SOURCES =			\
	options.h \
	config.h config.cpp \
	cli.h cli.cpp \
	main.cpp
