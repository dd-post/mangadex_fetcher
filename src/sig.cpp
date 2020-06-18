#include "sig.h"
#include <stdio.h>

void catch_sigint(int signo) {
    printf("\nCaught SIGINT. Cleaning up...\n");

    if (!current_file.empty()) {
        remove(current_file.c_str());
    }

    exit(1);
}
