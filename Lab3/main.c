#include <stdio.h>
#include "mm.h"
#include "memlib.h"

int main() {
	mem_init();
	
	mm_init();
	printf("test\n");
	void* ptr = mm_malloc(50);
	mm_free(ptr);
	return 0;
}
