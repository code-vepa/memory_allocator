#include "mem_alloc.h"

int chunk_start_compare(const void *ptr1, const void *ptr2){
	
	const Heap_Chunk * a_chunk = ptr1;
	const Heap_Chunk * b_chunk = ptr2;

	return a_chunk->start - b_chunk->start;
}

int chunk_list_find(const Chunk_List* list, void* ptr){
	Heap_Chunk key = {
		.start = ptr
	};
	
	Heap_Chunk* result = bsearch(&key, list->chunks, list->count,
					sizeof(list->chunks[0]), chunk_start_compare);

	if(result != 0){
		assert(list->chunks <= result);
		return (result - list->chunks) / sizeof(list->chunks[0]);
	}
	else return -1;
}

void chunk_list_insert(Chunk_List * list, void* ptr, size_t size){
	assert(list->count < ALLOCED_CAP);
	list->chunks[list->count].start = ptr;
	list->chunks[list->count].size = size;

	for(size_t i = list->count; i > 0 
			&& list->chunks[i].start < list->chunks[i - 1].start;
			--i){
				const Heap_Chunk temp = list->chunks[i];
				list->chunks[i] = list->chunks[i - 1];
				list->chunks[i - 1] = temp;
	}

	list->count += 1;	
}

void chunk_list_remove(Chunk_List* list, size_t index){
	assert(index < list->count);
	
	for(size_t i = index; i < list->count - 1; ++i){
		list->chunks[i] = list->chunks[i + 1];
	}
	
	list->count -= 1;
}

void chunk_list_display_chunks(const Chunk_List* list){
	for(int i = 0; i < list->count; ++i){
		printf("\tstart: %p,\tsize: %zu\n",
			list->chunks[i].start, 
			list->chunks[i].size);
	} 
}