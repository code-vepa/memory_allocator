#include "mem_alloc.h"

int main(){
	for(int i = 1; i < 10; ++i){
		void * p = heap_alloc(i);
		if(i % 2 == 0){
			heap_free(p);
		}
	}
	
	chunk_list_display_chunks(&alloced_chunks);
	printf("\n\n\nFreed:\n");
	chunk_list_display_chunks(&dealloced_chunks);

	return 0;
}
