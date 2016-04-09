#ifndef __TNG_CONFIG_H__
#define __TNG_CONFIG_H__
#include "arch_detect.h"

#ifdef TNG_OS_FLASCC
#define TNG_NO_UVLOOP
#endif // TNG_OS_FLASCC

#endif