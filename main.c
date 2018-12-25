#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

size_t stddef_max_align_t() {return alignof(max_align_t);}
size_t stdint_max_align_t();

#include <stdio.h>
int main(){
	puts(stddef_max_align_t() == stdint_max_align_t() ? "SUCCESS: Your platform does not exhibit this bug." : "FAILURE: Your platform exhibits this bug.");
	puts("Please report results back to https://github.com/rohlem/gcc-max_align_t-bug-repro/issues/1");
}