#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "utils.h"

#define CMDBUFLEN 128
int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[CMDBUFLEN];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);

    va_end(ap);

    return system(buf);
}
