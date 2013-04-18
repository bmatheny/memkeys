# Initialize autoconf
AC_PREREQ([2.59])
AC_INIT([mctop], [0.1], [bmatheny@mobocracy.net])
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
AC_PROG_LIBTOOL

# Check for typedefs, structures, and compiler characteristics
AC_TYPE_UINT16_T

# Check for headers
AC_CHECK_HEADERS([getopt.h])

# Checks for libraries
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB(getopt,getopt_long)
AC_CHECK_LIB(gnugetopt,getopt_long)
AC_ARG_WITH(libpcap_includes,
  [  --with-libpcap-includes=DIR    libpcap include directory],
  [with_libpcap_includes="$withval"],[with_libpcap_includes="no"])
AC_ARG_WITH(libpcap_libraries,
  [  --with-libpcap-libraries=DIR   libpcap library directory],
  [with_libpcap_libraries="$withval"],[with_libpcap_libraries="no"])
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

# Enable debugging
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

# Check functions
AC_CHECK_FUNCS([getopt_long])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
