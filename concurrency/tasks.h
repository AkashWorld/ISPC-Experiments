#ifndef _HEADER_TASKS_
#define _HEADER_TASKS_

void *ISPCAlloc(void **handlePtr, int64_t size, int32_t alignment);
void ISPCLaunch(void **handlePtr, void *f, void *data, int count0, int count1, int count2);
void ISPCSync(void *handle);

#endif