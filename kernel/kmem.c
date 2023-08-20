#include "kmem.h"
#include "kstring.h"
#include "memory-map.h"
#include "printf.h"

typedef struct linked_node_t {
  struct linked_node_t* next;
} linked_node_t;

typedef struct kmemory_t {
  spinlock_t lock;
  linked_node_t* free_list;
} kmemory_t;

static kmemory_t kmem;
extern char end[]; // first address after kernel, defined by kernel.ld

void kmem_init(void) {
  spinlock_init(&kmem.lock, "kmem");

  kmem.free_list = NULL;

  // Add all of the memory after the kernel to the list of free pages
  char* current_page = (char*)PAGE_ROUNDUP((u64)end);
  while (current_page < (char*)PHSYICAL_TOP) {
    printf("Initialising page: %p\n", current_page);
    kfree(current_page);
    current_page += PAGE_SIZE;
  }
}

void* kalloc(void) {
  linked_node_t* node;

  spinlock_acquire(&kmem.lock);
  node = kmem.free_list;
  if (node) {
    kmem.free_list = node->next;
  }
  spinlock_release(&kmem.lock);

  if (node) {
    // The page is cleared to all ones when it has been freed. Fill the page with a different value
    // now for the same reason.
    memset(node, 42, PAGE_SIZE);
  }

  // node is either a valid page, or a null pointer
  return (void*)node;
}

void kfree(void* pa) {
  linked_node_t* node;

  if ((u64)pa % PAGE_SIZE != 0) {
    panic("kfree align");
  }

  if ((char*)pa < end) {
    panic("kfree low");
  }

  if ((u64)pa > PHSYICAL_TOP) {
    panic("kfree high");
  }

  // xv6 fills the page with a fixed constant, so that programs that use after free are more likely
  // to find the error. I don't see a reason not to do the same
  memset(pa, 1, PAGE_SIZE);

  // Cast the whole page to a linked list node, so we can use it in the free list.
  node = (linked_node_t*)pa;

  spinlock_acquire(&kmem.lock);
  node->next = kmem.free_list;
  kmem.free_list = node;
  spinlock_release(&kmem.lock);
}
