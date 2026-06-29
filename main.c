#include "mem_alloc.h"

int main(){
	

	chunk_list_display_chunks(&alloced_chunks);
	printf("\n\n\nFreed:\n");
	chunk_list_display_chunks(&dealloced_chunks);

	return 0;
}
