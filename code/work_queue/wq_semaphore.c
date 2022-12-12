
internal void
WQ_CreateSemaphore(wq_semaphore_handle *Semaphore, uint32_t InitialValue, uint32_t MaxValue)
{
#if defined(IMPL_CreateSemaphore)
  IMPL_CreateSemaphore(Semaphore, InitialValue, MaxValue);
#endif
}


internal void
WQ_AcquireSemaphore(wq_semaphore_handle *Semaphore)
{
#if defined(IMPL_AcquireSemaphore)
  IMPL_AcquireSemaphore(Semaphore);
#endif
}

internal void
WQ_ReleaseSemaphore(wq_semaphore_handle *Semaphore)
{
#if defined(IMPL_ReleaseSemaphore)
  IMPL_ReleaseSemaphore(Semaphore);
#endif
}
