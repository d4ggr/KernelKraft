#pragma once
#include "peripherals/base.h"

#define IRQ_PENDING_1       REG32(PBASE + 0x0000B204)
#define ENABLE_IRQS_1       REG32(PBASE + 0x0000B210)
#define DISABLE_IRQS_1      REG32(PBASE + 0x0000B21C)

#define SYSTEM_TIMER_IRQ_1  (1 << 1)
