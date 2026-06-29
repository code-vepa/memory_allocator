#include "mem_alloc.h"

int main(){
	

	char* cptr;
	cptr = (char*) heap_alloc(20);
	
	scanf("%19s", cptr);
	printf("Hello, %s!\n", cptr);
	heap_free(cptr);


	chunk_list_display_chunks(&alloced_chunks);
	printf("\n\n\nFreed:\n");
	chunk_list_display_chunks(&dealloced_chunks);

	return 0;
}
