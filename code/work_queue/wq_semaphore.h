/* date = December 12th 2022 2:57 pm */

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct wq_semaphore_handle wq_semaphore_handle;
struct wq_semaphore_handle
{
  void *Handle;
  uint32_t Value;
  uint32_t MaxValue;
};

internal void WQ_CreateSemaphore(wq_semaphore_handle  *Semaphore, uint32_t InitialValue, uint32_t MaxValue);
internal void WQ_AcquireSemaphore(wq_semaphore_handle *Semaphore);

// NOTE(fakhri): increments the semaphore while keeping it always under MaxValue
internal void WQ_ReleaseSemaphore(wq_semaphore_handle *Semaphore);


#endif //SEMAPHORE_H
