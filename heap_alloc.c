#include "mem_alloc.h"

char* heap = NULL;

size_t heap_alloced_amount = 0;
size_t heap_freed_amount = 0;

Chunk_List alloced_chunks = {0};
Chunk_List dealloced_chunks = {0};
Region_List mapped_regions = {0};

//Rounds size up to the nearest multiple of the system page size
static size_t align_to_page(size_t size){
	size_t page_size = sysconf(_SC_PAGESIZE);
	return (size + page_size - 1) / page_size * page_size;
}

//Requests a fresh region of memory from the OS via mmap
static void* request_heap_region(size_t size){
	size_t aligned_size = align_to_page(size);

	void* region = mmap(
        NULL,                          // let the kernel choose the address
        aligned_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
	
	if(region == MAP_FAILED)
		return NULL;
	
	assert(mapped_regions.count < REGIONS_CAP);
	mapped_regions.regions[mapped_regions.count].start = region;
	mapped_regions.regions[mapped_regions.count].size = aligned_size;
    mapped_regions.count += 1;

	chunk_list_insert(&dealloced_chunks, region, aligned_size);
	return region;
}


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
		//No free chunk big enough, so request a new region from the OS
		size_t request_size = size > CHUNK_REGION_SIZE ? size : CHUNK_REGION_SIZE;
		if(request_heap_region(request_size) == NULL){
            return NULL; // mmap failed, out of general memory
        }

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

	return NULL; // size is 0
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

void* heap_realloc(void* ptr, size_t size){
	if(ptr == NULL)
		return heap_alloc(size);

	if(size == 0){
		heap_free(ptr);
		return NULL;
	}

	int index = chunk_list_find(&alloced_chunks, ptr);
	assert(index >= 0);
	Heap_Chunk *chunk = &alloced_chunks.chunks[index];

	//if the given size is the same as the current size
	if(chunk->size == size)
		return ptr;
	
	//shrink the block
	if(size < chunk->size){
		size_t tail = chunk->size - size;
		chunk->size = size;
		chunk_list_insert(&dealloced_chunks, chunk->start + size, tail);

		coalesce_free_chunks();
		
		return ptr;
	}

	//expand the block
	if(size > chunk->size){
		void* new_block = heap_alloc(size);
		
		if(new_block == NULL)
			return NULL;
		
		memcpy(new_block, ptr, chunk->size);

		heap_free(ptr);

		return new_block;
	}

	return NULL;

}

void heap_free(void* ptr){
	if(ptr != NULL){
		const int index = chunk_list_find(&alloced_chunks, ptr); 
		assert(index >= 0);
		chunk_list_insert(&dealloced_chunks, alloced_chunks.chunks[index].start,
						alloced_chunks.chunks[index].size);
		chunk_list_remove(&alloced_chunks, (size_t)index);
	}
	
	coalesce_free_chunks();
}

