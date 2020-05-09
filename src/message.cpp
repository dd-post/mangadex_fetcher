#include "message.h"

#include <stdlib.h>
#include <stdio.h>

void pquit(int ret, const char* format, ...) {
    va_list vp;
    va_start(vp, format);

    vprintf(format, vp);
    va_end(vp);

    exit(ret);
}
