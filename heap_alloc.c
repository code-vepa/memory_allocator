#include <stdio.h>
#include <assert.h>

#define CAPACITY 640000
#define ALLOCED_CAP 1024


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

char heap[CAPACITY] = {0};
//Keeps track of how much of heap has been used so far
//Tracks heap_size <= CAPACITY (640k Bytes)
size_t heap_size = 0;

Heap_Chunk heap_alloced[ALLOCED_CAP] = {0};
//The number of allocated chunks
//Keeps track of heap_alloced_amount < 1024
size_t heap_alloced_amount = 0;


void* heap_alloc(size_t size){
	if(size <= 0) return NULL;
    assert(heap_size + size <= CAPACITY);
    
    void * result = heap + heap_size; 
    heap_size += size;
    const Heap_Chunk chunk = {
		.start = result,
		.size = size,
	};
	assert(heap_alloced_amount < ALLOCED_CAP);
	heap_alloced[heap_alloced_amount++] = chunk;

	return result;
}

void allocated_chunks_print(void){
	for(int i = 0; i < heap_alloced_amount; ++i){
		printf("	start: %p, size: %zu\n",
			heap_alloced[i].start, heap_alloced[i].size);
	} 
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
