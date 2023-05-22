// If C++ support is active (whch it is by default), then
// the __libc_init_array and __libc_fini_array
// functions will need these functions to be defined.
// mark them as weak so that they're user overridable.
void _fini() __attribute__((weak)); 
void _fini () {}
void _init() __attribute__((weak)); 
void _init() {}