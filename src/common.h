#ifndef _COMMON_H
#define _COMMON_H

#ifdef HAVE_CONFIG_H
#include "mconfig.h"
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "mctop"
#endif
#ifndef PACKAGE_STRING
#define PACKAGE_STRING "mctop (unknown version)"
#endif
#ifndef PACKAGE_BUGREPORT
#define PACKAGE_BUGREPORT "bmatheny@mobocracy.net"
#endif

// Define _DEBUG if appropriate
#ifdef DEBUGGING
#if DEBUGGING
#define _DEBUG 1
#endif
#endif

// Define _DEVEL if appropriate
#ifdef DEVELOPMENT
#if DEVELOPMENT
#define _DEVEL 1
#endif
#endif

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "util/util.h"
#include "logging/logging.h"
#include "config.h"

// Need for typecasting in to_string for older versions of g++
typedef long long unsigned int llui_t;
typedef long long int llsi_t;

#endif
