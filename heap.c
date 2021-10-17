#include <assert.h>
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

typedef struct
{
    HeapChunk* chunks;
    size_t count;
    size_t capacity;
} HeapChunkList;

char heap[HEAP_CAPACITY];

HeapChunk allocated_chunks[HEAP_CHUNK_CAPACITY];
HeapChunkList allocated_chunks_list = {
    .chunks   = allocated_chunks,
    .count    = 0,
    .capacity = HEAP_CHUNK_CAPACITY
};

HeapChunk freed_chunks[HEAP_CHUNK_CAPACITY + 1] = { { .start = heap, .size = HEAP_CAPACITY } };
HeapChunkList freed_chunks_list = {
    .chunks   = freed_chunks,
    .count    = 1,
    .capacity = HEAP_CHUNK_CAPACITY + 1
};

void print_chunk_list(HeapChunkList list)
{
    for (int i = 0; i < list.count; i++)
    {
        printf(
            "{ .start = %p, .size = %u }\n",
            list.chunks[i].start,
            list.chunks[i].size
        );
    }
}

void* heap_alloc(size_t size)
{
    assert(size > 0);

    for (int i = 0; i < freed_chunks_list.count; i++)
    {
        if (freed_chunks[i].size >= size)
        {
            // Cut the free chunk
            // Return the allocated part
        }
    }

    fprintf(stderr, "Unable to allocate memory, size = %u", size);
    abort();
}

void heap_free(void* ptr)
{
    UNIMPLEMENTED;
}

int main(void)
{
    int* num = (int*)heap_alloc(4);
    print_chunk_list(allocated_chunks_list);
    print_chunk_list(freed_chunks_list);

    return 0;
}
