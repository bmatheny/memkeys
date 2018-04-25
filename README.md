# memkeys

Show your memcache key usage in realtime.

This was originally inspired by [mctop](https://github.com/etsy/mctop) from etsy.
I found that under load mctop would drop between 50 and 75 percent of packets.
Under the same load memkeys will typically drop less than 3 percent of packets.
This is on a machine saturating a 1Gb network link.

Build Status: [![Build Status](https://travis-ci.org/bmatheny/memkeys.png?branch=master)](https://travis-ci.org/bmatheny/memkeys)

## Command line options

    Usage: memkeys -i NIC [options]
        -d, --discard=THRESH        Discard keys where req/s rate is below THRESH
        -i, --interface=NIC         Network interface to capture traffic on (required)
        -p, --port=PORT             Network port to capture memcache traffic on (default 11211)
        -r, --refresh=INTERVAL      Refresh the stats display every INTERVAL ms (default 500)
        -l, --logfile=FILE          Output logs to FILE
        -R, --report=REPORT         Output data in REPORT format (CSV or curses, default curses)

        -h, --help                  This help
        -v, --verbose               Increase verbosity. May be used multiple times.
        -V, --version               Show program info and exit.

## Running

You will most likely want to run with something like:

    memkeys -i eth0 -l /tmp/memkeys.log

If you are running memkeys on a very high traffic machine you will want to
specify a discard threshold, otherwise the memory footprint will grow quite
large.

    memkeys -i eth0 -d 10.0 -l /tmp/memkeys.log

If you are running memkeys on a proxy you may want to use `-i lo` to listen on
localhost.

## Screenshot

![Screenshot](https://raw.github.com/wiki/bmatheny/memkeys/misc/screenshot.png)

## Development/Build

Build is based on autoconf.

Install gperftools and gperftools-devel if you want to build with
`--enable-profiling`. You will typically want to configure with
`--enable-debug`, and possibly with `--enable-development`. The latter two
options will enable additional error logging. If you are actually doing
development you should definitely add `--enable-development` as doing so will
add some additional compiler flags to help catch errors.

You will need libpcap-devel, libpcrecpp, and libncurses-devel.

On Ubuntu with all packages:

    sudo apt-get install autoconf libpcap-dev libpcre3-dev and lib32ncurses5-dev google-perftools libgoogle-perftools-dev
    ./build-eng/autogen.sh
    make
    make check

The memkeys should then be present in /usr/local/bin. 

memkeys was developed on CentOS 5.8 with the following software tools:

    GCC version    : g++44 (GCC) 4.4.6 20110731 (Red Hat 4.4.6-3)
    GNU gmake      : 3.81
    ld             : 2.17.50.0.6-20
    coreutils      : 5.97
    libtool        : 2.4.2
    autoconf       : 2.68
    automake       : 1.9.6

The following library versions were used:

    libpcap-devel  : 0.9.4-15
    pcre-devel     : 6.6-6
    ncurses-devel  : 5.5-24.20060715

This should build fine against newer versions of the above tools. Different
libraries are untested.

# License

Copyright 2013 Tumblr.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
