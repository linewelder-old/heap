#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAPACITY 64000
#define HEAP_CHUNK_CAPACITY 1024

#define UNIMPLEMENTED \
    printf("%s:%u: %s is not implemented yet", __FILE__, __LINE__, __func__); \
    abort();

typedef struct
{
    void*  start;
    size_t size;
} HeapChunk;

char heap[HEAP_CAPACITY];
HeapChunk allocated[HEAP_CHUNK_CAPACITY];

void* heap_alloc(size_t size)
{
    UNIMPLEMENTED;
    return 0;
}

void heap_free(void* ptr)
{
    UNIMPLEMENTED;
}

int main(void)
{
    int* num = (int*)heap_alloc(4);
    return 0;
}
