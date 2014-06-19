#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i = 1;
    FILE *input;
    if (argc >= 2 && strcmp(argv[1], "-debug") == 0) {
        debug = 1;
        i++;
    }
    if (argc > i + 1) {
        fprintf(stderr, "usage: %s [-debug] [<fichier>]\n", argv[0]);
        return 1;
    }
    input = (argc == i + 1) ? fopen(argv[i], "r") : stdin;
    /* Using the fuction generated from loadprog.y */
    if (loadprog(argv[i], input)) {
        exit(EXIT_FAILURE);
    }
    if (input != stdin) {
        fclose(input);
    }
    if (debug) {
        printf("Segment de code\n");
        for (i = 0; i < prog_length; i++) {
            printf("%d  ", prog[i]);
        }
        printf("\n");
    }
    printf("Execution started:\n");
    vm_execute();
    vm_halt();
    exit(EXIT_SUCCESS);
}
