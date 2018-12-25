#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

size_t stdint_max_align_t() {return alignof(max_align_t);}
