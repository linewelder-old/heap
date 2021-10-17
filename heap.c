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

HeapChunk allocated_chunks[HEAP_CHUNK_CAPACITY];
size_t allocated_chunks_count = 0;

HeapChunk freed_chunks[HEAP_CHUNK_CAPACITY + 1] = { { .start = heap, .size = HEAP_CAPACITY } };
size_t freed_chunks_count = 0;

void print_chunk_array(const HeapChunk* array, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        printf(
            "{ .start = %p, .size = %u }\n",
            array[i].start,
            array[i].size
        );
    }
}

void* heap_alloc(size_t size)
{
    assert(size > 0);

    for (int i = 0; i < freed_chunks_count; i++)
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
    print_chunk_array(allocated_chunks, allocated_chunks_count);
    
    return 0;
}
