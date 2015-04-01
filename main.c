/* ***************************************************************************
 * This file is part of virtual_mlv.
 *
 * virtual_mlv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * virtual_mlv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with virtual_mlv.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 *  Authors: S. Lombardy, N. Bedon, C. Morvan, W. Hay
 *
 *************************************************************************** */

#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* From loadprog.c. */
extern int loadprog(FILE *);
extern void print_load_error(int);

static void print_version(void);
static void print_help(const char *);

int main(int argc, char *argv[]) {
    int opt;
    FILE *input = NULL;
    /* Has to be static to be known at compile-time. */
    static int exec = 1;
    const struct option longopts[] = {
        {"debug", no_argument, &debug, 1},
        {"help", no_argument, NULL, 'h'},
        {"noexec", no_argument, &exec, 0},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };
    
    /* Parse command line options. */
    while ((opt = getopt_long_only(argc, argv, "hv", longopts, NULL)) != -1) {
        switch (opt) {
            case 0:
                /* An option that sets a flag does nothing else. */
                break;
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            case 'v':
                print_version();
                exit(EXIT_SUCCESS);
            case '?':
                break;
            default:
                abort();
        }
    }
    input = (optind == argc) ? stdin : fopen(argv[optind], "r");
    if (!input) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    /* Using the fuction generated from loadprog.y. */
    if (!loadprog(input)) {
        if (debug) {
            print_code_segment(prog, prog_length);
        }
        if (exec && !vm_select_input()) {
            vm_execute();
        }
    }
    vm_halt();
    if (input != stdin && fclose(input) == EOF) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

/**
 * Prints the version number and the authors list.
 */
static void print_version(void) {
    printf(
"virtual_mlv 2.0\n"
"The virtual machine of the compilation course of the Université Paris-Est\n"
"Marne-la-Vallée (UPEM).\n"
"Written by Sylvain Lombardy, Nicolas Bedon, Christophe Morvan, Gaël Fuhs,\n"
"Wuthy Hay, Quentin Campos and Jefferson Mangue.\n");
    printf("\n");
    printf(
"This is free software; see the source for copying conditions.  There is NO\n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
    );
}

/**
 * Prints the builtin help.
 * @param execname The name of the executable (typically @p argv[0]).
 */
static void print_help(const char *execname) {
    printf("usage: %s [OPTIONS ...] [FILE]\n\n", execname);
    printf("Option summary:\n");
    printf("  -debug: prints the VM code segment\n");
    printf("  -help: displays the builtin help and stops\n");
    printf("  -noexec: does not run the code (not very useful without the "
           "debug option)\n");
    printf("  -version: prints the software version number and stops\n");
}
