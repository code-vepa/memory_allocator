#include "mem_alloc.h"

int main(){
	for(int i = 0; i < 26; ++i){
		void * p = heap_alloc(i);

	}
	
	chunk_list_display_chunks(&alloced_chunks);

	return 0;
}
