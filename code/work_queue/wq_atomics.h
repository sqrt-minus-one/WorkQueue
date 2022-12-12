/* date = December 12th 2022 3:11 pm */

#ifndef WQ_ATOMICS_H
#define WQ_ATOMICS_H

// NOTE(fakhri): returns true if the new value is successfuly written
internal int32_t WQ_AtomicCompareAndExchange64(volatile int64_t *Dest, int64_t Expect, int64_t Exchange);

// NOTE(fakhri): returns the old value that was in Addend
internal int64_t WQ_AtomicIncrement64(volatile int64_t *Addend);
internal int64_t WQ_AtomicDecrement64(volatile int64_t *Addend);

#endif //WQ_ATOMICS_H
