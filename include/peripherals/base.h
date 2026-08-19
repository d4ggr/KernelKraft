#pragma once

#include "types.h"

#define PBASE 0x3F000000 //for RPi3b
#define REG32(addr) (*(volatile uint32_t*)(addr))
