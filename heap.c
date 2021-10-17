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

void chunks_remove(HeapChunkList* list, size_t index)
{
    list->count--;
    for (int i = index; i < list->count; i++)
        list->chunks[i] = list->chunks[i + 1];
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
            chunks_remove(&freed_chunks_list, i);
        }

        return pointer;
    }

    fprintf(stderr, "Unable to allocate memory, size = %u", size);
    abort();
}

int chunk_compare(const void* a, const void* b)
{
    const HeapChunk* a_chunk = a;
    const HeapChunk* b_chunk = b;

    return a_chunk->start - b_chunk->start;
}

void heap_free(void* ptr)
{
    HeapChunk search_key = { .start = ptr };
    HeapChunk* chunk = (HeapChunk*)bsearch(
        &search_key, allocated_chunks, allocated_chunks_list.count, 
        sizeof(allocated_chunks[0]), chunk_compare 
    );

    printf("Found: { .start = %p, .size = %u }\n", chunk->start, chunk->size);
}

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

void debug(void)
{
    printf("Allocated chunks:\n");
    print_chunk_list(allocated_chunks_list);
    printf("Freed chunks:\n");
    print_chunk_list(freed_chunks_list);
}

int main(void)
{
    void* a = heap_alloc(4);
    void* b = heap_alloc(4);
    debug();

    // heap_free(a);
    heap_free(b);
    debug();

    return 0;
}
