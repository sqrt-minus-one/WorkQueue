/* date = December 12th 2022 2:58 pm */

#ifndef WIN32_SEMAPHORE_H
#define WIN32_SEMAPHORE_H

#define IMPL_CreateSemaphore  W32_CreateSemaphore
#define IMPL_AcquireSemaphore W32_AcquireSemaphore
#define IMPL_ReleaseSemaphore  W32_ReleaseSemaphore


internal void W32_CreateSemaphore(wq_semaphore_handle *Semaphore, uint32_t InitialValue, uint32_t MaxValue);
internal void W32_AcquireSemaphore(wq_semaphore_handle *Semaphore);
internal void W32_ReleaseSemaphore(wq_semaphore_handle *Semaphore);

#endif //WIN32_SEMAPHORE_H
