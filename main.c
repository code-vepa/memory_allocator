#include "mem_alloc.h"

int main(void){
	
	void* p = heap_alloc(200);

    printf("Allocated chunks:\n");
    chunk_list_display_chunks(&alloced_chunks);

    printf("\nFree chunks:\n");
    chunk_list_display_chunks(&dealloced_chunks);

	return 0;
}
