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

/* Creates space for a new chunk at index index */
static void chunks_insert(HeapChunkList* list, size_t index)
{
    assert(list->count < list->capacity);

    for (int i = list->count; i > index; i--)
        list->chunks[i] = list->chunks[i - 1];

    list->count++;
}

/* Removes the chunk at index index */
static void chunks_remove(HeapChunkList* list, size_t index)
{
    list->count--;
    for (int i = index; i < list->count; i++)
        list->chunks[i] = list->chunks[i + 1];
}

/* Adds the chunk { .start = start, .size = size } to the allocated chunks list */
static void chunks_allocate(void* start, size_t size)
{
    // Find the chunk going right after our chunk
    int index = 0;
    while (index < allocated_chunks_list.count
        && allocated_chunks[index].start < start)
    {
        index++;
    }

    chunks_insert(&allocated_chunks_list, index);

    allocated_chunks[index].start = start;
    allocated_chunks[index].size  = size;
}

/* Adds the chunk to the freed chunks list, merges it with neighbours if needed */
static void chunks_free(HeapChunk chunk)
{
    heap_allocated -= chunk.size;

    // Find the chunk going right after our chunk
    int index = 0;
    while (index < freed_chunks_list.count
        && freed_chunks[index].start < chunk.start)
    {
        index++;
    }


    int merge_with_next = index != freed_chunks_list.count
                       && chunk.start + chunk.size >= freed_chunks[index].start;
    int merge_with_last = index != 0
                       && freed_chunks[index - 1].start + freed_chunks[index - 1].size >= chunk.start; 

    if (merge_with_next && merge_with_last)
    {
        freed_chunks[index - 1].size += chunk.size + freed_chunks[index].size;
        chunks_remove(&freed_chunks_list, index);
    }
    else if (merge_with_next)
    {
        freed_chunks[index].start =  chunk.start;
        freed_chunks[index].size  += chunk.size;   
    }
    else if (merge_with_last)
    {
        freed_chunks[index - 1].size += chunk.size;
    }
    else
    {
        chunks_insert(&freed_chunks_list, index);
        freed_chunks[index] = chunk;
    }
}

void* heap_alloc(size_t size)
{
    assert(size > 0);
    assert(heap_allocated + size <= HEAP_CAPACITY);

    heap_allocated += size;

    // Find the free chunk able to fit the allocated memory
    for (int i = 0; i < freed_chunks_list.count; i++)
    {
        HeapChunk* chunk = &freed_chunks[i];
        void* pointer = chunk->start;
        
        if (chunk->size < size)
            continue;

        // Add the chunk to the allocated list
        chunks_allocate(chunk->start, size);

        // If the free chunk is bigger than the allocated one, cut it
        if (chunk->size > size)
        {
            chunk->start += size;
            chunk->size -= size;
        }
        // Else remove it entirely
        else if (chunk->size == size)
        {
            chunks_remove(&freed_chunks_list, i);
        }

        return pointer;
    }

    fprintf(stderr, "Unable to allocate memory, size = %u\n", size);
    abort();
}

/* Comparison function for binary search */
static int chunk_compare(const void* a, const void* b)
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

    // If the pointer does not point to an allocated memory
    if (!chunk)
    {
        fprintf(stderr, "Invalid pointer, unable to free memory\n");
        abort();
    }

    size_t index = ((void*)chunk - (void*)allocated_chunks) / sizeof(allocated_chunks[0]);

    chunks_free(*chunk);
    chunks_remove(&allocated_chunks_list, index);
}

