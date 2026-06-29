#include "mem_alloc.h"

char heap[CAPACITY] = {0};

size_t heap_alloced_amount = 0;
size_t heap_freed_amount = 0;

Chunk_List alloced_chunks = {0};
Chunk_List dealloced_chunks = {
	.count = 1, .chunks = {
		[0] = {.start = heap, .size = sizeof(heap)}
	},
};

void* heap_alloc(size_t size){
	if(size > 0){
		for(size_t i = 0; i < dealloced_chunks.count; ++i){
			const Heap_Chunk chunk = dealloced_chunks.chunks[i];
			if(chunk.size >= size){
				
				chunk_list_remove(&dealloced_chunks, i);
				const size_t tail = chunk.size - size;
				chunk_list_insert(&alloced_chunks, chunk.start, size);
				
				if(tail > 0){
					chunk_list_insert(&dealloced_chunks, chunk.start + size, tail);
				}
				
				return chunk.start;
			}
		}
	}

	return NULL; // out of memory or 0 sized
}


void* heap_calloc(size_t count, size_t size){
	if(count != 0 && size > SIZE_MAX / count) 
	return NULL;

	size_t total = count * size;
	void* result = heap_alloc(total);

	if(result != NULL)
		memset(result, 0, total);
	
	return result;
}

void heap_free(void* ptr){
	if(ptr != NULL){
		const int index = chunk_list_find(&alloced_chunks, ptr); 
		assert(index >= 0);
		chunk_list_insert(&dealloced_chunks, alloced_chunks.chunks[index].start,
						alloced_chunks.chunks[index].size);
		chunk_list_remove(&alloced_chunks, (size_t)index);
	}
	
	//Coalesce
	Chunk_List merged = {0};
	chunk_list_merge(&merged, &dealloced_chunks);
	dealloced_chunks = merged;
}

