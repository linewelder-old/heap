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
size_t heap_allocated = 0;

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

void chunks_allocate(void* start, size_t size)
{
    assert(allocated_chunks_list.count < allocated_chunks_list.capacity);

    int index = 0;
    while (index < allocated_chunks_list.count
        && allocated_chunks[index].start < start)
    {
        index++;
    }

    for (int j = allocated_chunks_list.count; j > index; j--)
        allocated_chunks[j] = allocated_chunks[j - 1];

    allocated_chunks[index].start = start;
    allocated_chunks[index].size  = size;
    
    allocated_chunks_list.count++;
}

void chunks_remove(HeapChunkList list, size_t index)
{
    printf("remove %u\n", index);
}

void* heap_alloc(size_t size)
{
    assert(size > 0);
    assert(heap_allocated + size <= HEAP_CAPACITY);

    heap_allocated += size;

    for (int i = 0; i < freed_chunks_list.count; i++)
    {
        HeapChunk* chunk = &freed_chunks[i];
        void* pointer = chunk->start;
        
        if (chunk->size < size)
            continue;

        chunks_allocate(chunk->start, size);

        if (chunk->size > size)
        {
            chunk->start += size;
            chunk->size -= size;
        }
        else if (chunk->size == size)
        {
            chunks_remove(freed_chunks_list, i);
        }

        return pointer;
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
    int* num  = (int*)heap_alloc(4);
    int* num2 = (int*)heap_alloc(4);

    printf("Allocated chunks:\n");
    print_chunk_list(allocated_chunks_list);
    printf("Freed chunks:\n");
    print_chunk_list(freed_chunks_list);

    return 0;
}
