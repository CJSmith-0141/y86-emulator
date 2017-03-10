#ifndef Instructions
#define Instructions

#include "y86emul.h"
#include "Stack.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void EMULATE(struct cpu* CPU,struct byte* Memory,int size);
void        big_Instruction(struct cpu* _cpu, struct byte* Memory, struct        bigInstruction* Move);
void arithmetic_Instruction(struct cpu* _cpu, struct byte* Memory, struct arithmeticInstruction*   OP);
void      small_Instruction(struct cpu* _cpu, struct byte* Memory, struct      smallInstruction* bite);
void      noreg_Instruction(struct cpu* _cpu, struct byte* Memory, struct      noregInstruction*   JP);




#endif 
