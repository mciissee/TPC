#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "print.h"

int errcount;
int warncount;
int mainfound;
int lineno = 1;

void print_error(const char* format, ...) {
    fprintf(stderr, "%d: ", lineno);
    fprintf(stderr, RED "%s: " RESET, "error");
    va_list args;
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    fprintf(stderr, "\n");
    errcount++;
}

void print_message(const char* format, ...) {
    va_list args;
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
}

void print_warning(const char* format, ...) {
    fprintf(stderr, "%d: ", lineno);
    fprintf(stderr, YELLOW "%s: " RESET, "warning");
    va_list args;
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    fprintf(stderr, "\n");
}

void print_errors() {
    if (!mainfound) {
        print_error("missing main function");
    }

    if (warncount > 0)
	    fprintf(stderr,"%d warnings(s) generated\n", errcount);

    if (errcount > 0)
	    fprintf(stderr,"%d error(s) generated\n", errcount);
}
