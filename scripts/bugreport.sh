#!/bin/bash

# Setup global variables
PATH=$PATH:/sbin:/usr/sbin:/bin:/usr/bin;
DATE=`date +%m-%d-%Y-%s`;
FILE="sysinfo.${DATE}";
DEBUG=0;
# Need ldd command for a few things
LDD='';
VERSION='';

# See if we're in debug mode (output to stdout)
if [[ $# -gt 0 ]]; then
	echo "Putting into debug mode"; echo; echo;
	DEBUG=1;
fi

# Determine whether we should use which or type to find commands in path
TEST=`type type|grep not`
if [[ -z "$TEST" ]]; then
  WHICH="which";
else
  WHICH="type";
fi

# log - log data according to DEBUG flag
#
# Reads input from stdin and outputs it based on DEBUG settings. This function
# is suitable for piping data to.
function log() 
{
	while read data
	do
		if [ $DEBUG -eq 1 ]; then
			echo "[$(date +"%D %T")] $data";
		else
			echo "$data" >> $FILE;
		fi
	done
}

# has_command - Determines whether a command is available
#
# Takes a command name and tries to find that command. Returns 1 if the
# command is available and 0 otherwise.
function has_command()
{
	if [[ $# -ne 1 ]]; then
		echo "Error: has_command passed empty value" | log
	fi

	TEST=`$WHICH $1 2>/dev/null`;
	if [[ $? -eq 0 ]]; then
		return 1;
	else
		return 0;
	fi
}

# Ensure we have gawk, exit otherwise
has_command "gawk"
if [[ $? -ne 1 ]]; then
	echo "Error: We require gawk to run" | log
	exit;
fi

function system_information()
{
	echo "System information" | log
	echo "--------------------------------------------------------------"  | log
	print_os | log
	print_release | log
	print_hardware | log
	print_uname | log
}

function print_release()
{
	uname -r 2>/dev/null |\
		gawk '{print "OS RELEASE     :",$N}';
}

function print_hardware()
{
	uname -m 2>/dev/null |\
		gawk '{print "HARDWARE       :",$N}';
}

function print_uname()
{
	uname -a 2>/dev/null |\
		gawk '{print "UNAME          :",$N}';
}

function print_os()
{
	uname -s 2>/dev/null |\
		gawk '{print "OS             :",$N}';
}

system_information

function print_available_mem()
{
	ERR_MSG="Available Mem  : Unknown";

	if [[ -f "/proc/meminfo" ]]; then
		cat /proc/meminfo |\
			grep MemTotal |\
			awk '{print "Available Mem  : "$2" "$3}';
		return;
	fi

	has_command "hwprefs"
	if [[ $? -eq 1 ]]; then
		MEM=`hwprefs memory_size 2>/dev/null`;
		if [[ $? -eq 0 ]]; then
			echo "Available Mem  : $MEM";
		else
			echo "$ERR_MSG";
		fi
    return;
  fi

  has_command "system_profiler"
  if [[ $? -eq 1 ]]; then
    MEM=`system_profiler | grep "  Memory:" 2>/dev/null | tr -d ' ' | awk -F: '{print $2}'`;
    if [[ $? -eq 0 ]]; then
			echo "Available Mem  : $MEM";
		else
			echo "$ERR_MSG";
		fi
	else
		echo "$ERR_MSG";
	fi
}

CPU_COUNT=0;
CPU_TYPE='';

function _get_cpu_count()
{
	if [[ -f "/proc/cpuinfo" ]]; then
		CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
    return;
	fi

	has_command "hwprefs"
	if [[ $? -eq 1 ]]; then
		CPU_COUNT=`hwprefs cpu_count`;
    return;
	fi

  has_command "system_profiler"
  if [[ $? -eq 1 ]]; then
    CPU_COUNT=`system_profiler|grep -i cores|awk -F: '{print $2}' | tr -d ' '`
	fi
}

function _get_cpu_type()
{
	if [[ -f "/proc/cpuinfo" ]]; then
		CPU_TYPE=`grep "model name" /proc/cpuinfo |head -1|sed 's/.*: //g'`
    return;
	fi

	has_command "hwprefs"
	if [[ $? -eq 1 ]]; then
		CT=`hwprefs cpu_type`;
		CF=`hwprefs cpu_freq`;
		CPU_TYPE="${CT}\t@ ${CF}";
    return;
	fi

  has_command "system_profiler"
  if [[ $? -eq 1 ]]; then
    CT=`system_profiler|grep -i 'Processor Name'|awk -F: '{print $2}'`
    CF=`system_profiler|grep -i 'Processor Speed'|awk -F: '{print $2}'`
    CPU_TYPE="${CT}\t@ ${CF}"
  fi
}

function print_cpu_info()
{
	ERR_MSG="Available CPU  : Unknown";

	_get_cpu_count
	_get_cpu_type

	if [[ -n "$CPU_TYPE" ]]; then
		echo "Available CPU  : ${CPU_COUNT}x${CPU_TYPE}";
	else
		echo "$ERR_MSG";
	fi
}


function hardware_information()
{
	echo "" | log
	echo "Hardware information" | log
	echo "--------------------------------------------------------------"  | log
	print_available_mem | log
	print_cpu_info | log
}

hardware_information

function find_ldd()
{
	has_command "ldd";
	if [[ $? -eq 1 ]]; then
		LDD="ldd";
		return 1;
	else
		has_command "otool";
		if [[ $? -eq 1 ]]; then
			LDD="otool -L";
			return 2;
		fi
	fi
	return 0;
}

function print_c_library()
{
	ERR_MSG="GNU C Library  : Not Found";

	has_command "ls";
	if [[ $? -eq 1 ]]; then
		find_ldd
		RETVAL=$?
		if [[ $RETVAL -eq 1 ]]; then
			ls -l `ldd /bin/sh | gawk '/libc/{print $3}'` |\
				sed -e 's/\.so$//' | gawk -F'[.-]'   '{print "GNU C Library  : " $(NF-2)"."$(NF-1)"."$NF}'
		elif [[ $RETVAL -eq 2 ]]; then
			VERSION=`otool -L /bin/sh | gawk '/libSystem/{print $NF}' | sed 's/)//g'`
			echo "System Library : $VERSION";
		else
			echo "$ERR_MSG";
		fi
	else
		echo "$ERR_MSG";
	fi
}

function print_ldd_version()
{
	find_ldd
	RETVAL=$?
	if [[ $RETVAL -eq 1 ]]; then
		ldd -v >/dev/null 2>&1 && ldd -v || ldd --version | head -1 |\
			gawk 'NR==1{print "ldd            :", $NF}';
	elif [[ $RETVAL -eq 2 ]]; then
		echo "otool          : Available. No version number.";
	else
		echo "ldd/otool      : Not Found";
	fi
}

function print_gcc_version()
{
	has_command "gcc";
	if [[ $? -eq 1 ]]; then
		gcc --version 2>/dev/null |\
			head -1 |\
			gawk '{print "GCC version    :",$N}';
	else
		echo "GCC version    : Not Found";
	fi
}

function print_make_version()
{
	has_command "gmake";
	if [[ $? -eq 1 ]]; then
		gmake --version 2>/dev/null |\
			gawk -F, '{print $1}' |\
			gawk '/GNU Make/{print "GNU gmake      :",$NF}';
	else
		has_command "make";
		if [[ $? -eq 1 ]]; then
			make --version 2>/dev/null |\
				gawk -F, '{print $1}' |\
				gawk '/GNU Make/{print "GNU make       :",$NF}';
		else
			echo "GNU Make       : Not Found";
		fi
	fi
}

function try_exec()
{
	for i in "$@"; do
		TEST=`$i 2>&1`;
		if [[ $? -eq 0 ]]; then
			VERSION=$TEST;
			return;
		fi
	done
	VERSION='';
}

function print_ld_version()
{
	ERR_MSG="ld             : Not Found";

	has_command "ld"
	if [[ $? -eq 1 ]]; then
		try_exec "ld --version" "ld -v"

		if [[ -z $VERSION ]]; then
			echo "$ERR_MSG";
			return;
		fi

		VERSION=`echo "$VERSION" |\
			grep -i version |\
		       	sed 's/.* version //g' |\
			awk '{print $1}'`;
		echo "ld             : $VERSION";
	else
		echo "$ERR_MSG";
		return;
	fi
}

function print_coreutil_version()
{
	ERR_MSG="coreutils      : Not Found"

	has_command "cp"
	if [[ $? -eq 1 ]]; then
		VERSION=`cp --version 2>/dev/null`
		if [[ $? -eq 0 ]]; then
			VERSION=`echo "$VERSION" |\
				head -1 |\
				gawk '{print $NF}'`;
			echo "coreutils      : $VERSION";
		else
			echo "$ERR_MSG";
			return;
		fi
	else
		echo "$ERR_MSG";
		return;
	fi
}

function print_libtool_version()
{
	has_command "libtool"
	if [[ $? -eq 1 ]]; then
		try_exec "libtool --version" "libtool -V"
		if [[ -z $VERSION ]]; then
			echo "libtool        : Couldn't find version";
			return;
		fi
		echo $VERSION |\
			head -1 |\
			gawk '{\
				if (length($4) != 0 && $4 == "version") {\
					print "libtool        : "$5\
				} else if (length($4) == 0) {\
					print "libtool        : "$3\
				} else {\
					print "libtool        : "$4\
				}
			      }';
	else
		echo "libtool        : Not Found";
	fi
}

function print_autoconf()
{
	has_command "autoconf"
	if [[ $? -eq 1 ]]; then
		autoconf --version |\
			head -1 |\
			gawk '{\
				if (length($4) == 0) {\
					print "autoconf       : "$3\
				} else {\
					print "autoconf       : "$4\
				}
			}';
	else
		echo "autoconf       : Not Found";
	fi
}

function print_automake()
{
	has_command "automake"
	if [[ $? -eq 1 ]]; then
		automake --version 2>/dev/null |\
			head -1 |\
			gawk '{print "automake       : "$4}'
	else
		echo "automake       : Not Found"
	fi
}

function build_information()
{
	echo "" | log
	echo "Build/Compiler information" | log
	echo "--------------------------------------------------------------"  | log
	print_c_library | log
	print_ldd_version | log
	print_gcc_version | log
	print_make_version | log
	print_ld_version | log
	print_coreutil_version | log
	print_libtool_version | log
	print_autoconf | log
	print_automake | log
}

build_information

function print_ruby_version()
{
	has_command "ruby"
	if [[ $? -eq 1 ]]; then
		ruby -v |\
			gawk '{print "ruby           : "$2}';
	else
		echo "ruby           : Not Found";
	fi
}

function print_rails_version()
{
	has_command "rails"
	if [[ $? -eq 1 ]]; then
		rails -v |\
			gawk '{print "rails          : "$2}';
	else
		echo "rails          : Not Found";
	fi
}

function ruby_information()
{
	echo "" | log
	echo "Ruby Information" | log
	echo "--------------------------------------------------------------"  | log
	print_ruby_version | log
	print_rails_version | log
}

ruby_information

function print_python_version()
{
	has_command "python"
	if [[ $? -eq 1 ]]; then
		python -V 2>&1 |\
			gawk '{print "python         : "$2}';
	else
		echo "python         : Not Found";
	fi
}

function print_php_version()
{
	has_command "php"
	if [[ $? -eq 1 ]]; then
		php -v |\
			head -1 |\
			gawk '{print "php            : "$2}';
	else
		echo "php            : Not Found";
	fi
}

function other_packages()
{
	echo "" | log
	echo "Other Packages" | log
	echo "--------------------------------------------------------------"  | log
	print_python_version | log
	print_php_version | log
}

other_packages

if [[ $DEBUG -eq 0 ]]; then
	echo "$FILE contains information which may help resolve your";
	echo "bug, please submit it with your bug report.";
fi
