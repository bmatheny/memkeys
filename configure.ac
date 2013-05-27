# Initialize autoconf
AC_PREREQ([2.63])
AC_INIT([memkeys], [0.2], [bmatheny@mobocracy.net])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([mconfig.h:mconfig.h.in])
AC_CONFIG_MACRO_DIR([m4])

# Initialize automake
AM_INIT_AUTOMAKE([1.9.6 foreign])

# Checks for language
AC_LANG([C++])

# Checks for programs
CXXFLAGS="$CXXFLAGS -std=c++0x" # Will fail if not g++44 or later
AC_PROG_CXX([g++44 g++ gcc cxx cc++ c++])
AC_PROG_CXXCPP
CXXCPP="$CXXCPP -std=c++0x" # This must come after AC_PROG_CXXCPP
AC_PROG_LIBTOOL

# Check for typedefs, structures, and compiler characteristics
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Platform specific setup
AC_CANONICAL_HOST
case "$host" in
  *linux*)
    # getconf LEVEL1_DCACHE_LINESIZE
    AC_DEFINE([linux], 1, [Linux])
    ;;
  darwin*)
    # sysctl -a hw.cachelinesize
    AC_DEFINE([__APPLE__], 1, [Apple Hardware])
    ;;
esac

    
# Check for headers
AC_CHECK_HEADERS([getopt.h])
AC_CHECK_HEADERS([pcrecpp.h])
AC_CHECK_HEADERS([cstdatomic])
AC_CHECK_HEADERS([atomic])

# Checks for libraries
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([getopt],[getopt_long])
AC_CHECK_LIB([gnugetopt],[getopt_long])

PCRECPP=""
AC_ARG_WITH([libpcrecpp],
            AS_HELP_STRING([--with-libpcrecpp=DIR], [libpcre base directory]),
            [with_libpcrecpp="$withval"],
            [with_libpcrecpp="no"])
if test "x$with_libpcrecpp" != "xno"; then
  LDFLAGS="${LDFLAGS} -L${with_libpcrecpp}/lib"
  CPPFLAGS="${CPPFLAGS} -I${with_libpcrecpp}/include"
fi
# Fail if pcrecpp is not found
AC_CHECK_LIB([pcrecpp], [main],, [AC_MSG_ERROR([libpcrecpp not found but required])])

AC_ARG_WITH(libpcap_includes,
            AS_HELP_STRING([--with-libpcap-includes=DIR], [libpcap include directory]),
            [with_libpcap_includes="$withval"],
            [with_libpcap_includes="no"])
AC_ARG_WITH(libpcap_libraries,
            AS_HELP_STRING([--with-libpcap-libraries=DIR], [libpcap library directory]),
            [with_libpcap_libraries="$withval"],
            [with_libpcap_libraries="no"])
if test "x$with_libpcap_includes" != "xno"; then
   CPPFLAGS="${CPPFLAGS} -I${with_libpcap_includes}"
fi
if test "x$with_libpcap_libraries" != "xno"; then
   LDFLAGS="${LDFLAGS} -L${with_libpcap_libraries}"
fi
LPCAP=""
AC_CHECK_LIB(pcap, pcap_datalink,, LPCAP="no")
if test "x$LPCAP" = "xno"; then
  echo
  echo "   ERROR!  Libpcap library/headers (libpcap.a (or .so)/pcap.h)"
  echo "   not found, go get it from http://www.tcpdump.org"
  echo "   or use the --with-libpcap-* options, if you have it installed"
  echo "   in unusual place.  Also check if your libpcap depends on another"
  echo "   shared library that may be installed in an unusual place"
  exit 1
fi

CURSES=""
AC_CHECK_LIB(ncurses, initscr,, CURSES="no")
AC_CHECK_HEADERS([ncurses.h])
if test "x$CURSES" = "xno"; then
  echo
  echo "   ERROR!  ncurses library/headers (libncurses.a (or .so)/ncurses.h)"
  echo "   not found."
  exit 1
fi

AC_MSG_CHECKING(whether to create a profiled build)
AC_ARG_ENABLE(profiling,
              AS_HELP_STRING([--enable-profiling], [Turn on profiling (default=no)]),
[ case "${enableval}" in
  yes)
    AC_MSG_RESULT(yes)
    AC_CHECK_LIB([profiler], [ProfilerStop])
    ;;
  *)
    AC_MSG_RESULT(no)
    ;;
  esac],
[AC_MSG_RESULT(no)])

# Enable debugging
AC_MSG_CHECKING(whether to create a debug build)
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug], [Turn on debugging (default=no)]),
[ case "${enableval}" in
  yes)
    AC_MSG_RESULT(yes)
    # Created in mconfig.h for use as #ifdef DEBUG
    AC_DEFINE([DEBUGGING], 1, [Defined if you are debugging])
    debug=true
    ;;
  no)
    AC_MSG_RESULT(no)
    debug=false
    ;;
  *)
    AC_MSG_ERROR([bad value ${enableval} for --enable-debug])
    ;;
  esac],
[
  AC_MSG_RESULT(no)
  debug=false
])
# For use in Makefile.am as if DEBUG
AM_CONDITIONAL([DEBUGGING], [test x$debug = xtrue])

# Enable development
AC_MSG_CHECKING(whether to create a development build)
AC_ARG_ENABLE(development,
              AS_HELP_STRING([--enable-development], [Turn on development (default=no)]),
[ case "${enableval}" in
  yes)
    AC_MSG_RESULT(yes)
    # Created in mconfig.h for use as #ifdef DEVELOPMENT
    AC_DEFINE([DEVELOPMENT], 1, [Defined if you are development])
    development=true
    ;;
  no)
    AC_MSG_RESULT(no)
    development=false
    ;;
  *)
    AC_MSG_ERROR([bad value ${enableval} for --enable-development])
    ;;
  esac],
[
  AC_MSG_RESULT(no)
  development=false
])
# For use in Makefile.am as if DEVELOPMENT
AM_CONDITIONAL([DEVELOPMENT], [test x$development = xtrue])

# Check functions
AC_CHECK_FUNCS([getopt_long], ,
               [AC_MSG_ERROR([no getopt_long function found])])

# Build gtest
AC_CONFIG_SUBDIRS([gtest])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
