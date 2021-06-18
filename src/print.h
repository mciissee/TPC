#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "model.h"

extern int errcount;
extern int warncount;
extern int mainfound;
extern int lineno;

void print_error(const char* format, ...);
void print_message(const char* format, ...);
void print_warning(const char* format, ...);
void print_errors();

#endif