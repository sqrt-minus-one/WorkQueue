
internal void
W32_CreateSemaphore(wq_semaphore_handle *Semaphore, uint32_t InitialValue, uint32_t MaxValue)
{
  Semaphore->Handle = CreateSemaphoreA(0, InitialValue, MaxValue, 0);
}

internal void
W32_AcquireSemaphore(wq_semaphore_handle *Semaphore)
{
  WaitForSingleObject(Semaphore->Handle, INFINITE);
}

internal void
W32_ReleaseSemaphore(wq_semaphore_handle *Semaphore)
{
  ReleaseSemaphore(Semaphore->Handle, 1, 0);
}

internal int32_t
Win32_ReleaseSemaphoreLimited(wq_semaphore_handle *Semaphore)
{
  W32_ReleaseSemaphore(Semaphore);
}
