#include "Instructions.h"
#include "Stack.h"
#include "y86emul.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>




struct cpu* Initialize_CPU(char * ingest,int size){
	
	struct cpu* CPU = malloc(sizeof(struct cpu));
	
	CPU->Registers[0] = 0;
	CPU->Registers[1] = 0;
	CPU->Registers[2] = 0; 
	CPU->Registers[3] = 0;
	CPU->Registers[4] = 0;
	CPU->Registers[5] = 0;
	CPU->Registers[6] = 0;
	CPU->Registers[7] = 0;
	
	CPU->ZF = false;
	CPU->SF = false;
	CPU->OF = false;
	
	
	CPU->status = 0;
	
	CPU->PC = 0;
	
	return CPU;
}


struct byte* create_Program_Memory(char* ingest, int size, struct cpu* CPU){
	
	unsigned int strLength;
	
	unsigned int UPPERindex;
	unsigned int index;
	unsigned int subIndex;
	
	char* cpy = strdup(ingest);
	
	char * Token = strtok(cpy,"\n\t");
	 
	struct byte* Memory = malloc(size * sizeof(struct byte));
	
		for(index = 0 ; index<size ; index++){
			Memory[index].upper = 0;
			Memory[index].lower = 0;
		} 
		
	while(Token){
		
		/*
		 * 
		 * This has already been taken care of, so it just advances the token for continuities sake. 
		 * DONE
		 * */
		if(strcmp(Token,".size")==0){
			Token = strtok(NULL,"\n\t");
		}
		
		/*
		 * 
		 * This deals with the input of a string into memory, where each character is one byte. 
		 * DONE
		 * */
		else if(strcmp(Token,".string")==0){
			Token = strtok(NULL,"\n\t");
			
				index = HEXstr_to_int(Token);
			
			Token = strtok(NULL,"\n\t");
			
				strLength = strlen(Token);
				
					if(!(Token[0] == '"' && Token[strLength-1] == '"')){
						fprintf(stderr,"YOU HAVE A MALFORMED .string DIRECTIVE\n");
					}	
					for(subIndex=1;subIndex<strLength-1;subIndex++){
						UPPERindex = (unsigned int)Token[subIndex];
						
							Memory[index].upper = (UPPERindex & 0xf0) >> 4;
					
							Memory[index].lower = (UPPERindex & 0x0f);
							
							index++; 
					
				
					}
		}
		
		/*
		 * 
		 * This deals with inputing a 4-byte integer into memory, one byte at a time
		 * DONE
		 * */
		else if(strcmp(Token,".long")==0){
			Token = strtok(NULL,"\n\t");
			
				index = HEXstr_to_int(Token);
			
			Token = strtok(NULL,"\n\t");
				
				UPPERindex = DECstr_to_int(Token);
			
			Memory[index+3].upper = (UPPERindex & 0xf0000000) >> 28;
			Memory[index+3].lower = (UPPERindex & 0x0f000000) >> 24;
			
			Memory[index+2].upper = (UPPERindex & 0x00f00000) >> 20;
			Memory[index+2].lower = (UPPERindex & 0x000f0000) >> 16;
			
			Memory[index+1].upper = (UPPERindex & 0x0000f000) >> 12;
			Memory[index+1].lower = (UPPERindex & 0x00000f00) >> 8;
			
			Memory[index+0].upper = (UPPERindex & 0x000000f0) >> 4;
			Memory[index+0].lower = (UPPERindex & 0x0000000f)     ;	
				
		}
		
		/*
		 * 
		 * Sets the stated block of memory to be Zero, or unallocated
		 * DONE
		 * */
		else if(strcmp(Token,".bss")==0){
			
			
			Token = strtok(NULL,"\n\t");
				
				index = HEXstr_to_int(Token);
			
			Token = strtok(NULL,"\n\t");
				
				UPPERindex = index + DECstr_to_int(Token);
					for(index = index; index<=UPPERindex;index++){
						Memory[index].lower = 0;
						Memory[index].upper = 0;
					}
			
		}
		
		
		/*
		 * 
		 * The .byte Directive takes exactly one byte of information and stores it into memory at the hex address specified
		 * DONE
		 * */
		
		else if(strcmp(Token,".byte")==0){
			Token = strtok(NULL,"\n\t");
			
				index = HEXstr_to_int(Token);
			
			Token = strtok(NULL,"\n\t");
			
				strLength = strlen(Token);
			
				if(strLength==2){ /*More then two characters in this particular Directive represents a fatal error, and needs to be checked for.*/
				
					Memory[index].upper = HEXchar_to_int(Token[0]);
					
					Memory[index].lower = HEXchar_to_int(Token[1]);
					
				}else{
					fprintf(stderr,"THERE IS A MALFORMED .byte DIRECTIVE IN THE SOURCE FILE\n");
				}
		}
		
		/*
		 * 
		 * Populates the memory with integer representations of the instruction string. 
		 * DONE
		 * */
		 
		else if(strcmp(Token,".text")==0){
			
			
			Token = strtok(NULL,"\n\t");
			
				index = HEXstr_to_int(Token);
				CPU->PC = index;
			
			Token = strtok(NULL,"\n\t");
				
							
				for(subIndex=0;subIndex<strlen(Token);subIndex+=2){
					
					Memory[index].upper = HEXchar_to_int(Token[subIndex]);
					Memory[index].lower = HEXchar_to_int(Token[subIndex+1]);
					 
					index++; 
					
				}
			
			
		}else{
			
			fprintf(stderr,"THERE IS AN ISSUE WITH THE INPUT PROGRAM AND IT'S FORMATING, PLEASE TRY AGAIN.\n");
		
		}
		
		Token = strtok(NULL,"\n\t");
	}
	
			
	
	
	
	
	return Memory;
	

}


