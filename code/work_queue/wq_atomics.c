
internal int32_t
WQ_AtomicCompareAndExchange64(volatile int64_t *Dest, int64_t Expect, int64_t Exchange)
{
#if defined(IMPL_AtomicCompareAndExchange64)
  int32_t Result = IMPL_AtomicCompareAndExchange64(Dest, Expect, Exchange);
#endif
  return Result;
}


internal int64_t
WQ_AtomicIncrement64(volatile int64_t *Addend)
{
#if defined(IMPL_AtomicIncrement64)
  int64_t Result = IMPL_AtomicIncrement64(Addend);
#endif
  return Result;
}

internal int64_t
WQ_AtomicDecrement64(volatile int64_t *Addend)
{
#if defined(IMPL_AtomicDecrement64)
  int64_t Result = IMPL_AtomicDecrement64(Addend);
#endif
  return Result;
}
