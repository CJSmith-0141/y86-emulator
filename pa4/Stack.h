#ifndef Stack
#define Stack

#include "Stack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct cpu* Initialize_CPU(char * ingest,int size);
struct byte* create_Program_Memory(char* ingest, int size, struct cpu* CPU);

typedef struct byte{
	unsigned int lower : 4;
	unsigned int upper : 4;
}byte; 

typedef struct smallInstruction{
	unsigned int    opcode0  : 4;
	unsigned int    opcode1  : 4;
}smallInstruction;

typedef struct arithmeticInstruction{
	unsigned int    opcode0  : 4;
	unsigned int    opcode1  : 4;
	unsigned int   regcode0  : 4;
	unsigned int   regcode1  : 4;
}arithmeticInstruction;

typedef struct bigInstruction{
	struct arithmeticInstruction                  instr;
	unsigned int                               constant;  
}bigInstruction;

typedef struct noregInstruction{
	struct smallInstruction                  instr;
	unsigned int                          constant;  
}noregInstruction;

typedef struct cpu{
	
	
	/*
	 * 
	 * The following is a c int representation of all the registers and condition flags that will need to be updated as the program is emulated. 
	 * 
	 * the numbers next to each "register" are the hex codes that an instruction uses to find and use each register. the 8th register is treated as "no register" 
	 * 
	 * 
	 * */
	 /*0*//*eax*/
	 /*1*//*ecx*/
	 /*2*//*edx*/
	 /*3*//*ebx*/
	 /*4*//*esp*/
	 /*5*//*ebp*/
	 /*6*//*esi*/
	 /*7*//*edi*/
	 /*8*//*NO REG*/
	 
	int Registers[8];
	
	bool OF; /* Overflow Flag*/
	bool ZF; /* Zero     Flag*/
	bool SF; /* Sign     Flag*/
	
	int  PC; /* Program Counter */
	int status; /*
							* This unsigned char is the programs status,
							*	0 is AOK = A O K, Everything is fine, continue with the program
							* 	1 is HLT = is HALT, stops the program normally 
							* 	2 is ADR = ADDRESS ERROR, error in addressing 
							* 	3 is INS = INSTRUCTION ERROR, error in instruction construction
							*/
	
							 
	
}cpu;




#endif


