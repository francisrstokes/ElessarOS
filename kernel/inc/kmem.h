#ifndef INC_KMEM_H
#define INC_KMEM_H

#include "spinlock.h"

void kmem_init(void);
void* kalloc(void);
void kfree(void* pa);

#endif // INC_KMEM_H
