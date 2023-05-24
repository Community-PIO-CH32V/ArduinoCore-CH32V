#include "api/itoa.h"
/* String(unsigned long) needs the ultoa function defined */
/* toolchain doesnt have it, but unsigned int == unsigned long here.. */
static_assert(sizeof(unsigned long) == sizeof(unsigned));

extern "C" char* ultoa(unsigned long value, char *string, int radix) {
    return utoa(value, string, radix);
}