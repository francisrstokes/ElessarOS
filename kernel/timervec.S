.global mmode_timer_vec
.align 4
mmode_timer_vec:
  # Swap whatever is in a0 for the mscratch register, which contains a pointer to this
  # harts timer_scratch_t structure
  csrrw a0, mscratch, a0

  # We need a few general purpose registers for this routine - push them into the allocated
  # space in timer_scratch_t
  sd a1, 0(a0)
  sd a2, 8(a0)
  sd a3, 16(a0)

  # Setup the next timer with the CLINT
  ld a1, 24(a0) # mtimecmp
  ld a2, 32(a0) # interval
  ld a3, 0(a1)  # current time
  add a2, a2, a3
  sd a2, 0(a1)

  # Effectively pass this interrupt on to S-Mode by scheduling a software interrupt
  li a1, 2
  csrw sip, a1

  # Restore from scratch
  ld a1, 0(a0)
  ld a2, 8(a0)
  ld a3, 16(a0)
  csrrw a0, mscratch, a0

  mret
