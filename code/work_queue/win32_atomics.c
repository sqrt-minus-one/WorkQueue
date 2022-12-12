
internal int64_t
Win32_AtomicIncrement64(volatile int64_t *Addend)
{
  int64_t Result = InterlockedIncrement64(Addend) - 1;
  return Result;
}

internal int64_t
Win32_AtomicDecrement64(volatile int64_t *Addend)
{
  int64_t Result = InterlockedDecrement64(Addend) + 1;
  return Result;
}

internal int32_t
Win32_AtomicCompareAndExchange64(volatile int64_t *Dest, int64_t Expect, int64_t Exchange)
{
  int32_t Result = (InterlockedCompareExchange64(Dest, Exchange, Expect) == Expect);
  return Result;
}

internal void
Win32_CompletePreviousWritesBeforeFutureWrites()
{
  MemoryBarrier(); __faststorefence();
}