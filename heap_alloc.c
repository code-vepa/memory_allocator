#include "mem_alloc.h"

char heap[CAPACITY] = {0};
size_t heap_size = 0;

size_t heap_alloced_amount = 0;
size_t heap_freed_amount = 0;

Chunk_List alloced_chunks = {0};
Chunk_List dealloced_chunks = {0};

void* heap_alloc(size_t size){
	if(size <= 0) return NULL;
    assert(heap_size + size <= CAPACITY);
    
    void* ptr = heap + heap_size; 
    heap_size += size;
	chunk_list_insert(&alloced_chunks, ptr, size);
	return ptr;
}

void head_free(void* ptr){
	const int index = chunk_list_find(&alloced_chunks, ptr); 
	assert(index >= 0);
	chunk_list_insert(&dealloced_chunks, alloced_chunks.chunks[index].start,
					 alloced_chunks.chunks[index].size);
	chunk_list_remove(&alloced_chunks, (size_t)index);
}

