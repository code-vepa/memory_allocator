#include "mem_alloc.h"

int chunk_start_compare(const void *ptr1, const void *ptr2){
	
	const Heap_Chunk * a_chunk = ptr1;
	const Heap_Chunk * b_chunk = ptr2;

	if (a_chunk->start < b_chunk->start)
        return -1;
    if (a_chunk->start > b_chunk->start)
        return 1;
    return 0;
}

int chunk_list_find(const Chunk_List* list, void* ptr){
	Heap_Chunk key = {
		.start = ptr
	};
	
	Heap_Chunk* result = bsearch(&key, list->chunks, list->count,
					sizeof(list->chunks[0]), chunk_start_compare);

	if(result != 0){
		assert(list->chunks <= result);
		return result - list->chunks;
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
	for(size_t i = 0; i < list->count; ++i){
		printf("\tstart: %p,\tsize: %zu\n",
			list->chunks[i].start, 
			list->chunks[i].size);
	} 
}

void chunk_list_merge(Chunk_List* dest, const Chunk_List * src){
	dest->count = 0;
	
	for(size_t i = 0; i < src->count; ++i){
		const Heap_Chunk chunk = src->chunks[i];

		if(dest->count > 0){
			Heap_Chunk *last = &dest->chunks[dest->count - 1];

			if(last->start + last->size == chunk.start){
				last->size += chunk.size;
				continue;
			}
		}
		chunk_list_insert(dest, chunk.start, chunk.size);
	}
}

void coalesce_free_chunks(void){
	Chunk_List merged = {0};
	chunk_list_merge(&merged, &dealloced_chunks);
	dealloced_chunks = merged;
}