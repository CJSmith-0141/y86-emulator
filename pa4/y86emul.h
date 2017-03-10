#ifndef y86emul
#define y86emul

#include "Instructions.h"
#include "Stack.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void TEST_PRINT(struct byte* memory, int size);
int DECstr_to_int(char* input);
int HEXchar_to_int(char i);
int main(int argc, char **argv);
char * Stringify(const char* input);
int findSize(char* file);
int HEXstr_to_int(char* input);
char * strdup (const char *s);

#endif
