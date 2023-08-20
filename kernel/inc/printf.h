#ifndef INC_PRINTF_H
#define INC_PRINTF_H

#include <stdarg.h>
#include "types.h"

void printf_init(void);
void printf(char *fmt, ...);
void panic(char* str);

#endif // INC_PRINTF_H
