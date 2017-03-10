/* 
 *       CONNOR JAMES SMITH
 * 		 COMPUTER SCIENCE 211
 *       PROGRAMING ASSIGNMENT #4
 *       DUE: WEDNESDAY, MAY 1st, 2013
 * 
 * */

#include "Instructions.h"
#include "y86emul.h"
#include "Stack.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void TEST_PRINT(struct byte* Memory, int size){
	
	short int j;
	int i;
	
		for(i = 0;i<size;i++){
			j =  (Memory[i].upper << 4) + Memory[i].lower;
			printf("%x\t %x | %x | %x\n", i, Memory[i].upper, Memory[i].lower,j);
			
		}
		
	
}

int HEXchar_to_int(char i){
	
	
	if(i >= '0' && i <= '9'){
				i = i-'0';
	}else if(i >= 'a' && i <= 'f'){
				i = (i - 'a') + 10;
	}else{
		i= ' ';
	}
	
	return i;
	
}

char * strdup (const char *s) {
	char *d = malloc (strlen (s) + 1);
			if (d == NULL) return NULL;
				strcpy (d,s);
	return d;
}

int intPow(int b, int e){
	
	int output = 1;
	
	int count;
		
		for(count = 0; count<e;count++){
			output = output * b; 
		}
		
	return output;
	
}

int DECstr_to_int(char* input){
	
	char * inputCPY = strdup(input);
	
	int Length = strlen(inputCPY);
	
	
	
	int i;
	int output;
	int k = 0;
	
	free(inputCPY);
		
		for(i= Length-1 ; i>=0 ; i--){
			if(input[i] >= '0' && input[i] <= '9'){
				input[i] = input[i]-'0';
			}	  
		}
		
		for(i=Length-1;i>=0;i--){
			
			output = output + (input[i]*intPow(10,k));
			k++; 
		}
		
		
	
	
	
	return output;
	
}
int HEXstr_to_int(char* input){
	
	char * inputCPY = strdup(input);
	
	int Length = strlen(inputCPY);
	
	
	
	int i;
	int output;
	int k = 0;
	
	free(inputCPY);
		
		for(i= Length-1 ; i>=0 ; i--){
			if(input[i] >= '0' && input[i] <= '9'){
				input[i] = input[i]-'0';
			}else if(input[i] >= 'a' && input[i]<='f'){
				input[i] = (input[i]-'a') + 10;
			}	  
		}
		
		for(i=Length-1;i>=0;i--){
			
			output = output + (input[i]*intPow(16,k));
			k++; 
		}
		
		
	
	
	
	return output;
	
}



char * Stringify(const char* input){
		
		char * buffer = 0;
		long length;
			FILE * f = fopen (input, "rb");

				if (f) {
					fseek (f, 0, SEEK_END);
					length = ftell (f);
					fseek (f, 0, SEEK_SET);
					buffer = malloc (length);

						if (buffer) {
							fread (buffer, 1, length, f);
						}

					fclose (f);
				} else {
					printf("That file does not exist.\n");
				}

			if (buffer) {
				return buffer;
			} else {
				printf("There is something wrong with your program (perhaps it's empty). \n");
				return "";
			}
}
		
int findSize(char* file){
	
	int output; 
	char * duplicate = strdup(file);
	char * token = strtok(duplicate,"\n\t");
	char * dupToken;
		
		
		while(token){
		
			dupToken = strdup(token);
			
				if(strcmp(dupToken,".size")==0){
					free(dupToken);
					token = strtok(NULL, "\n\t");
					dupToken = strdup(token);
					output = HEXstr_to_int(dupToken);
					
					
				}else{
						token = strtok(NULL, "\n\t");
				}
				
			free(dupToken);
		}
	
	free(token);
	return output;
	
}	
	



int main(int argc, char **argv){
	char* ingest;
	int size;
	struct byte* Memory;
	struct cpu*  CPU;
		if(argc!=2){
			
			fprintf(stderr, "There is an issue with the ammount of arguments that you have, please reconsider.");
		}
	
	ingest = Stringify(argv[1]);
	
	size = findSize(ingest);
	
	
	
	CPU = Initialize_CPU(ingest,size);
	
	Memory = create_Program_Memory(ingest, size ,CPU); 
	
	EMULATE(CPU,Memory,size);
	/*TEST_PRINT(Memory,size);*/
	
	free(CPU);
	free(Memory);
	free(ingest);

	return 0;
	
}
