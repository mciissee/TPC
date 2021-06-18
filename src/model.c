#include <stdio.h>
#include <stdlib.h>
#include "model.h"

Symb* symbs;
DataTypes currtype;
int size, scope;
FILE* output = NULL;

char TYPE_NAMES[][MAX_TYPES] = {
    "char", // char
    "int", // int
    "long", // long
    "void", // void
};

void model_create(char* out) {
    output = fopen(out, "w");
    if (!output) {
        fprintf(stderr, "'%s' not a valid output path", out);
        exit(EXIT_FAILURE);
    }

    symbs = calloc(MAX_SYMBOLS, sizeof(Symb));
    if (!symbs) {
        fprintf(stderr, "memory error\n");
        fclose(output);
        exit(EXIT_FAILURE);
    }
}
