#include <stdio.h>
#include <assert.h>

#define CAPACITY 640000
#define ALLOCED_CAP 1024
#define DEALLOCED_CAP 1024


/*
*********************************************************
Allocated chunk of memory in the heap. Members: start pointer
Which points at the beginning of the allocated memory
Size of the allocated memory of that particular chunk
*********************************************************
*/

typedef struct{
	void* start;
	size_t size;
} Heap_Chunk;

typedef struct{
	size_t count;
	Heap_Chunk chunks[ALLOCED_CAP];
} Chunk_List;

int chunk_list_find(const Chunk_List* list, void* ptr){
	if(ptr == NULL) return -1;
	
	//for(size_t i = 0; i < alloced_chunks.count; ++i){
		//if(ptr == alloced_chunks.chunks->start[i]){
			
		
	//}

	return -1;
}

int chunk_list_insert(Chunk_List * list, void* ptr, size_t size){
	assert(list->count < ALLOCED_CAP);
	list->chunks[list->count].start = ptr;
	list->chunks[list->count].size = size;

	for(size_t i = list->count; i > 0 
			&& list->chunks[i].start < list->chunks[i].start;
			--i){
				const Heap_Chunk temp = list->chunks[i];
				list->chunks[i] = list->chunks[i - 1];
				list->chunks[i - 1] = temp;
	}

	
}

int chunk_list_remove(Chunk_List* list, size_t index){
	//assert(0 && "TODO: chunk_list_remove is not implemented")
	return 1;
}

void chunk_list_display_chunks(const Chunk_List* list){
	for(int i = 0; i < list->count; ++i){
		printf("\tstart: %p,\tsize: %zu\n",
			list->chunks[i].start, 
			list->chunks[i].size);
	} 
}

char heap[CAPACITY] = {0};
//Keeps track of how much of heap has been used so far
//Tracks heap_size <= CAPACITY (640k Bytes)
size_t heap_size = 0;

size_t heap_alloced_amount = 0; //The number of allocated chunks
size_t heap_freed_amount = 0; //The number of the freed chunks

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
 
}


int main(){
	char * root = heap_alloc(64);
	
	char * root2 = heap_alloc(6214);
	char * root3 = heap_alloc(63124);


	for(int i = 0; i < 26; ++i){
		root[i] = i + 'A';
	}
	
	allocated_chunks_print();


	return 0;
}
