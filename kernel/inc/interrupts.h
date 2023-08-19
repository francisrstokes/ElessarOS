#ifndef INC_INTERRUPTS_H
#define INC_INTERRUPTS_H

#include "inc/types.h"

u32 interrupts_get(void);
void interrupts_off(void);
void interrupts_on(void);

#endif // INC_INTERRUPTS_H
