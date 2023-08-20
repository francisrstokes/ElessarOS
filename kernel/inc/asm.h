#ifndef INC_ASM_H
#define INC_ASM_H

#include "types.h"

// M-Mode
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_S    (1L << 11)

static inline u64 r_mstatus(void) {
  u64 x;
  asm volatile("csrr %0, mstatus" : "=r"(x));
  return x;
}

static inline void w_mstatus(u64 x) {
  asm volatile("csrw mstatus, %0" : : "r"(x));
}

static inline u64 r_mepc(void) {
  u64 x;
  asm volatile("csrr %0, mepc" : "=r"(x));
  return x;
}

static inline void w_mepc(u64 x) {
  asm volatile("csrw mepc, %0" : : "r"(x));
}

static inline u64 r_mideleg(void) {
  u64 x;
  asm volatile("csrr %0, mideleg" : "=r"(x));
  return x;
}

static inline void w_mideleg(u64 x) {
  asm volatile("csrw mideleg, %0" : : "r"(x));
}

static inline u64 r_medeleg(void) {
  u64 x;
  asm volatile("csrr %0, medeleg" : "=r"(x));
  return x;
}

static inline void w_medeleg(u64 x) {
  asm volatile("csrw medeleg, %0" : : "r"(x));
}

static inline u64 r_pmpcfg0(void) {
  u64 x;
  asm volatile("csrr %0, pmpcfg0" : "=r"(x));
  return x;
}

static inline void w_pmpcfg0(u64 x) {
  asm volatile("csrw pmpcfg0, %0" : : "r"(x));
}

static inline u64 r_pmpaddr0(void) {
  u64 x;
  asm volatile("csrr %0, pmpaddr0" : "=r"(x));
  return x;
}

static inline void w_pmpaddr0(u64 x) {
  asm volatile("csrw pmpaddr0, %0" : : "r"(x));
}

static inline u64 r_mhartid(void) {
  u64 x;
  asm volatile("csrr %0, mhartid" : "=r"(x));
  return x;
}

static inline void w_mhartid(u64 x) {
  asm volatile("csrw mhartid, %0" : : "r"(x));
}

// S-Mode
static inline u64 r_satp(void) {
  u64 x;
  asm volatile("csrr %0, satp" : "=r"(x));
  return x;
}

static inline void w_satp(u64 x) {
  asm volatile("csrw satp, %0" : : "r"(x));
}

#define SIE_SEIE  (1L << 9)
#define SIE_STIE  (1L << 5)
#define SIE_SSIE  (1L << 1)

static inline u64 r_sie(void) {
  u64 x;
  asm volatile("csrr %0, sie" : "=r"(x));
  return x;
}

static inline void w_sie(u64 x) {
  asm volatile("csrw sie, %0" : : "r"(x));
}

static inline u64 r_sip(void) {
  u64 x;
  asm volatile("csrr %0, sip" : "=r"(x));
  return x;
}

static inline void w_sip(u64 x) {
  asm volatile("csrw sip, %0" : : "r"(x));
}

#define SSTATUS_SIE (1)

static inline u64 r_sstatus(void) {
  u64 x;
  asm volatile("csrr %0, sstatus" : "=r"(x));
  return x;
}

static inline void w_sstatus(u64 x) {
  asm volatile("csrw sstatus, %0" : : "r"(x));
}

// General purpose registers
static inline u64 r_tp(void) {
  u64 x;
  asm volatile("mv %0, tp" : "=r"(x));
  return x;
}

static inline void w_tp(u64 x) {
  asm volatile("mv tp, %0" : : "r"(x));
}

#endif // INC_ASM_H
