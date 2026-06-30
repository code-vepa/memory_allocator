#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define CAPACITY 640000
#define ALLOCED_CAP 1024
#define DEALLOCED_CAP 1024
#define REGIONS_CAP 256
#define CHUNK_REGION_SIZE (640 * 1024)

/*
*********************************************************
Allocated chunk of memory in the heap. Members: start pointer
Which points at the beginning of the allocated memory
Size of the allocated memory of that particular chunk
*********************************************************
*/

typedef struct{
	char* start;
	size_t size;
} Heap_Chunk;


typedef struct{
	size_t count;
	Heap_Chunk chunks[ALLOCED_CAP];
} Chunk_List;


typedef struct{
	void* start;
	size_t size;
} Mapped_Region;


typedef struct{
	size_t count;
	Mapped_Region regions[REGIONS_CAP];
} Region_List;

extern size_t heap_alloced_amount; //The number of allocated chunks
extern size_t heap_freed_amount; //The number of the freed chunks

extern Chunk_List alloced_chunks;
extern Chunk_List dealloced_chunks;
extern Region_List mapped_regions;


int chunk_start_compare(const void *ptr1, const void *ptr2);
int chunk_list_find(const Chunk_List* list, void* ptr);
void chunk_list_insert(Chunk_List * list, void* ptr, size_t size);
void chunk_list_remove(Chunk_List* list, size_t index);
void chunk_list_display_chunks(const Chunk_List* list);
void chunk_list_merge(Chunk_List * dest, const Chunk_List * src);

void coalesce_free_chunks(void);

void* heap_alloc(size_t size);
void* heap_realloc(void* ptr, size_t size);
void* heap_calloc(size_t count, size_t size);

void heap_free(void *ptr);

#endif