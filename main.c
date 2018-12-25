#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

size_t stddef_max_align_t() {return alignof(max_align_t);}
size_t stdint_max_align_t();

#include <stdio.h>
int main(){
	return puts(stddef_max_align_t() == stdint_max_align_t() ? "SUCCESS" : "FAILURE");
}