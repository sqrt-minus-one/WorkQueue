/* date = December 12th 2022 2:30 pm */

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "stdint.h"

#define internal static
#define global static

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define Statement(S) do { S } while(0)
#define AssertBreak (*(volatile int *)0 = 0)
#define Assert(b) Statement(if (!(b)) { AssertBreak; })

#include "wq_semaphore.h"
#include "wq_atomics.h"

#if defined(_WIN32)
# include <windows.h>
# include "win32_atomics.h"
# include "win32_semaphore.h"
#define SYS_THREAD_SIG(ThreadName) DWORD WINAPI ThreadName(LPVOID Input)
#else
#error Platform not supported yet
#endif

typedef uint32_t worker_id;
typedef void worker_work(worker_id, void *);
#define WORK_TASK_SIG(Name) void Name(worker_id WorkerID, void *Input)

typedef struct work_entry work_entry;
struct work_entry
{
  worker_work *Work;
  void *Data;
};

typedef struct next_work next_work;
struct next_work
{
  work_entry WorkEntry;
  int32_t IsValid;
};

typedef struct work_queue work_queue;
struct work_queue
{
  work_entry Entries[1024];
  volatile int64_t Head;
  
  volatile int64_t CommittedTail;
  volatile int64_t ClaimedTail;
  
  wq_semaphore_handle AvailableWorkSemaphore;
  
  uint32_t WorkersCount;
};

struct worker_info
{
  work_queue *WorkQueue;
  worker_id WorkerID;
};

//- NOTE(fakhri): work queue functions
internal void      WQ_SetupQueue(work_queue *WorkQueue, uint32_t WorkersCount);
internal next_work WQ_GetNextWorkToDo(work_queue *WorkQueue);
internal int32_t   WQ_ProcessOneEntry(worker_id WorkerID, work_queue *WorkQueue);
internal int32_t   WQ_PushEntrySP(work_queue *WorkQueue, worker_work *Work, void *Data);
internal int32_t   WQ_PushEntry(work_queue *WorkQueue, worker_work *Work, void *Data);

#if defined(WORK_QUEUE_IMPLEMENTATION)

#if defined(_WIN32)
# include "win32_atomics.c"
# include "win32_semaphore.c"
#else
#error Platform not supported yet
#endif

#include "wq_semaphore.c"
#include "wq_atomics.c"
#include "wq.c"

#endif //WORK_QUEUE_IMPLEMENTATION

#endif //WORK_QUEUE_H
