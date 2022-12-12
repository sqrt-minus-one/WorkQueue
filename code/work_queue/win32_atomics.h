/* date = December 12th 2022 3:21 pm */

#ifndef WIN32_ATOMICS_H
#define WIN32_ATOMICS_H

#define IMPL_AtomicCompareAndExchange64 Win32_AtomicCompareAndExchange64
#define IMPL_AtomicIncrement64 Win32_AtomicIncrement64
#define IMPL_AtomicDecrement64 Win32_AtomicDecrement64

internal int64_t Win32_AtomicIncrement64(volatile int64_t *Addend);
internal int64_t Win32_AtomicDecrement64(volatile int64_t *Addend);
internal int32_t Win32_AtomicCompareAndExchange64(volatile int64_t *Dest, int64_t Expect, int64_t Exchange);

#define CompletePreviousWritesBeforeFutureWrites() MemoryBarrier(); __faststorefence()

#endif //WIN32_ATOMICS_H
