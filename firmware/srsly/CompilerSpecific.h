/*
  This code has been copied from LUFA

  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef __LUFA_COMPILERSPEC_H__
#define __LUFA_COMPILERSPEC_H__

#if defined(__GNUC__)
    /** Forces GCC to use pointer indirection (via the device's pointer register pairs) when accessing the given
     *  struct pointer. In some cases GCC will emit non-optimal assembly code when accessing a structure through
     *  a pointer, resulting in a larger binary. When this macro is used on a (non \c const) structure pointer before
     *  use, it will force GCC to use pointer indirection on the elements rather than direct store and load
     *  instructions.
     *
     *  \param[in, out] StructPtr  Pointer to a structure which is to be forced into indirect access mode.
     */
    #define GCC_FORCE_POINTER_ACCESS(StructPtr)   __asm__ __volatile__("" : "=b" (StructPtr) : "0" (StructPtr))

    /** Forces GCC to create a memory barrier, ensuring that memory accesses are not reordered past the barrier point.
     *  This can be used before ordering-critical operations, to ensure that the compiler does not re-order the resulting
     *  assembly output in an unexpected manner on sections of code that are ordering-specific.
     */
    #define GCC_MEMORY_BARRIER()                  __asm__ __volatile__("" ::: "memory");

    /** Determines if the specified value can be determined at compile-time to be a constant value when compiling under GCC.
     *
     *  \param[in] x  Value to check compile-time constantness of.
     *
     *  \return Boolean true if the given value is known to be a compile time constant, false otherwise.
     */
    #define GCC_IS_COMPILE_CONST(x)               __builtin_constant_p(x)
#else
    #define GCC_FORCE_POINTER_ACCESS(StructPtr)
    #define GCC_MEMORY_BARRIER()
    #define GCC_IS_COMPILE_CONST(x)               0
#endif//__GNUC__
#endif
