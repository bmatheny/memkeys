#ifndef _COMMON_H
#define _COMMON_H

#ifdef HAVE_CONFIG_H
#include "mconfig.h"
#endif

#ifdef DEBUGGING
  #if DEBUGGING
    #define _DEBUG 1
  #endif
#endif

#include "config.h"
#include "exception.h"
#include "logging/logger.h"

#endif
