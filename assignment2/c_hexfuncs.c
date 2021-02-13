// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
long hex_to_printable(long byteval) {
    if (byteval < 0 || byteval > 255) {
        return -1;
    }
    if (byteval >= 32 && byteval <= 126) {
        return byteval;
    }
    else {
        return 46;
    }
}