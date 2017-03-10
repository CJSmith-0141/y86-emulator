#include "y86emul.h"
#include "Stack.h"
#include "Instructions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void EMULATE(struct cpu* CPU,struct byte* Memory,int size){
	
	while(CPU->status == 0){
		if(CPU->PC > size){
			CPU->status = 2;
			continue;
		}

		
		if(Memory[CPU->PC].upper == 0 || 
		   Memory[CPU->PC].upper == 1 || 
		   Memory[CPU->PC].upper == 9){/*ONE BYTE INSTRUCTIONS*/
					
					struct smallInstruction* instruction = malloc(sizeof(struct smallInstruction));
					
					instruction->opcode0 = Memory[CPU->PC].upper;
					instruction->opcode1 = Memory[CPU->PC].lower;
				
		
						
					
					small_Instruction(CPU, Memory, instruction);
					
					
					free(instruction);
			
		}else if(Memory[CPU->PC].upper == 2  || 
				 Memory[CPU->PC].upper == 6  || 
				 Memory[CPU->PC].upper == 10 || 
				 Memory[CPU->PC].upper == 11   ){/*TWO BYTE INSTRUCTIONS*/
			
					struct arithmeticInstruction* instruction = malloc(sizeof(struct arithmeticInstruction));
					
					instruction->opcode0 = Memory[CPU->PC+0].upper;
					instruction->opcode1 = Memory[CPU->PC+0].lower;
				
					
					instruction->regcode0 = Memory[CPU->PC+1].upper;
					instruction->regcode1 = Memory[CPU->PC+1].lower;
		
					arithmetic_Instruction(CPU, Memory, instruction);
		
					free(instruction);
		
		}else if(Memory[CPU->PC].upper == 7  || 
				 Memory[CPU->PC].upper == 8    ){/*FIVE BYTE INSTRUCTIONS*/
		
					struct noregInstruction* instruction = malloc(sizeof(struct noregInstruction));
					
					
					instruction->instr.opcode0 = Memory[CPU->PC+0].upper;
					instruction->instr.opcode1 = Memory[CPU->PC+0].lower;
					
					
					
					instruction->constant       =           (Memory[CPU->PC+1].lower      )
														  + (Memory[CPU->PC+1].upper <<  4) 
												  
														  + (Memory[CPU->PC+2].lower <<  8) 
														  + (Memory[CPU->PC+2].upper << 12) 
												  
														  + (Memory[CPU->PC+3].lower << 16) 
														  + (Memory[CPU->PC+3].upper << 20) 
												  
														  + (Memory[CPU->PC+4].lower << 24) 
														  + (Memory[CPU->PC+4].upper << 28); 
					
					noreg_Instruction(CPU, Memory, instruction);
					
					free(instruction);
		
		
		
		
		
		}else if(Memory[CPU->PC].upper == 3  || 
				 Memory[CPU->PC].upper == 4  || 
				 Memory[CPU->PC].upper == 5  || 
				 Memory[CPU->PC].upper == 12 || 
				 Memory[CPU->PC].upper == 13)  {/*SIX BYTE INSTRUCTIONS*/
			
						struct bigInstruction* instruction = malloc(sizeof(struct bigInstruction));
					
						instruction->instr.opcode0 = Memory[CPU->PC+0].upper;
						instruction->instr.opcode1 = Memory[CPU->PC+0].lower;
					
						
						instruction->instr.regcode0 = Memory[CPU->PC+1].upper;
						instruction->instr.regcode1 = Memory[CPU->PC+1].lower;
					
						instruction->constant       =       (Memory[CPU->PC+2].lower      )
														  + (Memory[CPU->PC+2].upper <<  4) 
												  
														  + (Memory[CPU->PC+3].lower <<  8) 
														  + (Memory[CPU->PC+3].upper << 12) 
												  
														  + (Memory[CPU->PC+4].lower << 16) 
														  + (Memory[CPU->PC+4].upper << 20) 
												  
														  + (Memory[CPU->PC+5].lower << 24) 
														  + (Memory[CPU->PC+5].upper << 28); 
					
						big_Instruction(CPU, Memory, instruction);
					
						free(instruction);  	
		
		
		}else{
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			CPU->status = 3;
			
		}
		
	
	
	}
	
	if(CPU->status ==1) printf("\n\nPROGRAM EXITED NORMALLY\n\n");
	if(CPU->status ==2) printf("\n\nADDRESS ERROR\n\n");
	if(CPU->status ==2) printf("\n\nINSTRUCTION ERROR\n\n");
	
	return; 
	
	
}

void big_Instruction(struct cpu* _cpu, struct byte* Memory, struct        bigInstruction* Move){
	int index;
	int temp;
	char tempchar;
	
	if(_cpu == NULL || Move == NULL){
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			_cpu->status = 3;
			return;
	}
	
	
	if(Move->instr.opcode0 == 3){/*INTEGER TO REGISTER MOVE*/
		
		_cpu->Registers[Move->instr.regcode1] = Move->constant;
		
		_cpu->PC += 6;
		return;
		
	}else if(Move->instr.opcode0 == 4){/*REGISTER TO MEMORY MOVE*/
		index = Move->constant + _cpu->Registers[Move->instr.regcode1];
		temp =  _cpu->Registers[Move->instr.regcode0];
			
			
			Memory[index+3].upper = (temp & 0xf0000000) >> 28;
			Memory[index+3].lower = (temp & 0x0f000000) >> 24;
			
			Memory[index+2].upper = (temp & 0x00f00000) >> 20;
			Memory[index+2].lower = (temp & 0x000f0000) >> 16;
			
			Memory[index+1].upper = (temp & 0x0000f000) >> 12;
			Memory[index+1].lower = (temp & 0x00000f00) >>  8;
			
			Memory[index+0].upper = (temp & 0x000000f0) >>  4;
			Memory[index+0].lower = (temp & 0x0000000f)      ;
		
		_cpu->PC += 6;
		return;
		
	}else if(Move->instr.opcode0 == 5){/*MEMORY TO REGISTER MOVE*/
		
		index = Move->constant + _cpu->Registers[Move->instr.regcode1];
		
		_cpu->Registers[Move->instr.regcode0] = (Memory[index+0].lower      )
		                                      + (Memory[index+0].upper <<  4) 
		                                      
		                                      + (Memory[index+1].lower <<  8) 
		                                      + (Memory[index+1].upper << 12) 
		                                      
		                                      + (Memory[index+2].lower << 16) 
		                                      + (Memory[index+2].upper << 20) 
		                                      
		                                      + (Memory[index+3].lower << 24) 
		                                      + (Memory[index+3].upper << 28); 
				
		_cpu->PC += 6;
		return;
		
	}else if(Move->instr.opcode0 == 12){/*READ*/
		
		if(Move->instr.opcode1 == 0){/*READ BYTE*/
			
			index = Move->constant + _cpu->Registers[Move->instr.regcode0];
			_cpu->ZF = false;
			if (EOF == scanf("%c",&tempchar))
				_cpu->ZF = true;
						
			Memory[index+0].upper = (temp & 0x000000f0) >>  4;
			Memory[index+0].lower = (temp & 0x0000000f)      ;
		
			_cpu->PC += 6;
			return;
		
		}else if(Move->instr.opcode1 == 1){/*READ WORD*/
			
			index = Move->constant + _cpu->Registers[Move->instr.regcode0];
			
			_cpu->ZF = false;
			if (EOF == scanf("%d",&temp))
				_cpu->ZF = true;
						
			Memory[index+3].upper = (temp & 0xf0000000) >> 28;
			Memory[index+3].lower = (temp & 0x0f000000) >> 24;
			
			Memory[index+2].upper = (temp & 0x00f00000) >> 20;
			Memory[index+2].lower = (temp & 0x000f0000) >> 16;
			
			Memory[index+1].upper = (temp & 0x0000f000) >> 12;
			Memory[index+1].lower = (temp & 0x00000f00) >>  8;
			
			Memory[index+0].upper = (temp & 0x000000f0) >>  4;
			Memory[index+0].lower = (temp & 0x0000000f)      ;
			
			_cpu->PC += 6;
			return;
		
		}else{
			
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			_cpu->status = 3;
			return;
		
		}

	}else if(Move->instr.opcode0 == 13){/*WRITE*/
		
		if(Move->instr.opcode1 == 0){/*WRITE BYTE*/
			index = Move->constant + _cpu->Registers[Move->instr.regcode0];
			temp = (Memory[index+0].lower      )
				 + (Memory[index+0].upper <<  4);
				 
			printf("%c",temp);
			_cpu->PC += 6;
			return;	
		}else if(Move->instr.opcode1 == 1){/*WRITE WORD*/
		index = Move->constant + _cpu->Registers[Move->instr.regcode0];
		 temp = (Memory[index+0].lower      )
		      + (Memory[index+0].upper <<  4) 
		                                      
		      + (Memory[index+1].lower <<  8) 
		      + (Memory[index+1].upper << 12) 
		                                      
		      + (Memory[index+2].lower << 16) 
		      + (Memory[index+2].upper << 20) 
		                                      
		      + (Memory[index+3].lower << 24) 
		      + (Memory[index+3].upper << 28);
		      
		    printf("%d",temp);

			_cpu->PC += 6;
			 
			return;  
		}else{
			
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			_cpu->status = 3;
			return;
		
		}
		
	}else{
		fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
		_cpu->status = 3;
		return;		
	}	

 return;
}
void arithmetic_Instruction(struct cpu* _cpu, struct byte* Memory, struct arithmeticInstruction* OP){
	
	int temp;
	int index;
	
	if(_cpu == NULL || OP == NULL){
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");
			_cpu->status = 3;
			return;	
	}       
	
	
	if(OP[0].opcode0 == 2){/*REGISTER TO REGISTER MOVE COMMAND*/
		
		_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode0];
		_cpu->PC += 2;
		return;
	}else if(OP[0].opcode0 == 6){/*ARITHMETIC AND LOGICAL REGISTER OPERATIONS*/
		
		
		if      (OP[0].opcode1 == 0){/*ADDITION*/
			_cpu->ZF=false;
			_cpu->SF=false;
			_cpu->OF=false;
			if( (_cpu->Registers[OP[0].regcode0] + _cpu->Registers[OP[0].regcode1])==0)_cpu->ZF=true;
			if( (_cpu->Registers[OP[0].regcode0] + _cpu->Registers[OP[0].regcode1]) <0)_cpu->SF=true;
			
			
			if(((_cpu->Registers[OP[0].regcode1] + _cpu->Registers[OP[0].regcode0])>0) && (_cpu->Registers[OP[0].regcode1]<0) && (_cpu->Registers[OP[0].regcode0]<0))_cpu->OF=true;/*(a>0 && b>0 && (a+b)<0) || (a<0 && b<0 && (a+b)>0)*/
			if(((_cpu->Registers[OP[0].regcode1] + _cpu->Registers[OP[0].regcode0])<0) && (_cpu->Registers[OP[0].regcode1]>0) && (_cpu->Registers[OP[0].regcode0]>0))_cpu->OF=true;
			
			_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode0] + _cpu->Registers[OP[0].regcode1]; 
			_cpu->PC += 2;
			return;
		}
		
		else if (OP[0].opcode1 == 1){/*SUBTRACTION*/
			_cpu->ZF=false;
			_cpu->SF=false;
			_cpu->OF=false;
			
			if( (_cpu->Registers[OP[0].regcode1] - _cpu->Registers[OP[0].regcode0])==0)_cpu->ZF=true;
			
			if( (_cpu->Registers[OP[0].regcode1] - _cpu->Registers[OP[0].regcode0]) <0)_cpu->SF=true;
			
			
			if(((_cpu->Registers[OP[0].regcode1] - _cpu->Registers[OP[0].regcode0])<0) && (_cpu->Registers[OP[0].regcode1]>0) && (_cpu->Registers[OP[0].regcode0]<0))_cpu->OF=true;/*(b>0 && a<0 && (b-a)<0) || (b<0 && a>0 && (b-a)>0)*/
			if(((_cpu->Registers[OP[0].regcode1] - _cpu->Registers[OP[0].regcode0])>0) && (_cpu->Registers[OP[0].regcode1]<0) && (_cpu->Registers[OP[0].regcode0]>0))_cpu->OF=true;																					
																								
																								
			_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode1] - _cpu->Registers[OP[0].regcode0]; 
			_cpu->PC += 2;
			return;
			
			
		}
		
		else if (OP[0].opcode1 == 2){/*BITWISE AND*/
			_cpu->ZF=false;
			_cpu->SF=false;
			
			
			if( (_cpu->Registers[OP[0].regcode0] & _cpu->Registers[OP[0].regcode1])==0)_cpu->ZF=true;
			if( (_cpu->Registers[OP[0].regcode0] & _cpu->Registers[OP[0].regcode1]) <0)_cpu->SF=true;
			_cpu->OF = false;
			_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode0] & _cpu->Registers[OP[0].regcode1]; 
			_cpu->PC += 2;
			return;
		}
		else if (OP[0].opcode1 == 3){/*BITWISE EXCLUSIVE OR*/
			_cpu->ZF=false;
			_cpu->SF=false;
			
			
			if( (_cpu->Registers[OP[0].regcode0] ^ _cpu->Registers[OP[0].regcode1])==0)_cpu->ZF=true;
			if( (_cpu->Registers[OP[0].regcode0] ^ _cpu->Registers[OP[0].regcode1]) <0)_cpu->SF=true;
			_cpu->OF = false;
			_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode0] ^ _cpu->Registers[OP[0].regcode1]; 
			_cpu->PC += 2;
			return;
		}else if (OP[0].opcode1 == 4){/*MULTIPLICATION*/
			_cpu->ZF=false;
			_cpu->SF=false;
			_cpu->OF=false;
			if( (_cpu->Registers[OP[0].regcode0] * _cpu->Registers[OP[0].regcode1])==0)_cpu->ZF=true;
			if( (_cpu->Registers[OP[0].regcode0] * _cpu->Registers[OP[0].regcode1]) <0)_cpu->SF=true;
			
			if(((_cpu->Registers[OP[0].regcode1] * _cpu->Registers[OP[0].regcode0])<0) && (_cpu->Registers[OP[0].regcode1]>0) && (_cpu->Registers[OP[0].regcode0]>0))_cpu->OF=true;																					
			if(((_cpu->Registers[OP[0].regcode1] * _cpu->Registers[OP[0].regcode0])<0) && (_cpu->Registers[OP[0].regcode1]<0) && (_cpu->Registers[OP[0].regcode0]<0))_cpu->OF=true;																					
			if(((_cpu->Registers[OP[0].regcode1] * _cpu->Registers[OP[0].regcode0])>0) && (_cpu->Registers[OP[0].regcode1]<0) && (_cpu->Registers[OP[0].regcode0]>0))_cpu->OF=true;																					
			if(((_cpu->Registers[OP[0].regcode1] * _cpu->Registers[OP[0].regcode0])>0) && (_cpu->Registers[OP[0].regcode1]>0) && (_cpu->Registers[OP[0].regcode0]<0))_cpu->OF=true;																					
			
			
																									   /*(a>0 && b>0 && (a*b)<0) || 
																										 (a<0 && b<0 && (a*b)<0) || 
																										 (a<0 && b>0 && (a*b)>0) || 
																										 (a>0 && b<0 && (a*b)>0)    */
			_cpu->Registers[OP[0].regcode1] = _cpu->Registers[OP[0].regcode0] * _cpu->Registers[OP[0].regcode1]; 
			_cpu->PC += 2;
			return;
		}else{
			
			fprintf(stderr,"YOU HAVE A MALFORMED ARITHMETIC INSTRUCTION");
			_cpu->status = 3;
			return;
		}
	}else if(OP[0].opcode0 == 10){/*PUSH*/
		
		temp = _cpu->Registers[OP[0].regcode0];
		
		_cpu->Registers[4] -= 4;
		
		index = _cpu->Registers[4];
		
		Memory[index+3].upper = (temp & 0xf0000000) >> 28;
		Memory[index+3].lower = (temp & 0x0f000000) >> 24;
			
		Memory[index+2].upper = (temp & 0x00f00000) >> 20;
		Memory[index+2].lower = (temp & 0x000f0000) >> 16;
			
		Memory[index+1].upper = (temp & 0x0000f000) >> 12;
		Memory[index+1].lower = (temp & 0x00000f00) >>  8;
			
		Memory[index+0].upper = (temp & 0x000000f0) >>  4;
		Memory[index+0].lower = (temp & 0x0000000f)      ;
		
		
		
		_cpu->PC += 2;
		return;
		
	}else if(OP[0].opcode0 == 11){/*POP*/
		
		
		
		index = _cpu->Registers[4];
		
		_cpu->Registers[OP[0].regcode0] = 		(Memory[index+0].lower      )
		                                      + (Memory[index+0].upper <<  4)
		                                      
		                                      + (Memory[index+1].lower <<  8) 
		                                      + (Memory[index+1].upper << 12) 
		                                      
		                                      + (Memory[index+2].lower << 16) 
		                                      + (Memory[index+2].upper << 20) 
		                                      
		                                      + (Memory[index+3].lower << 24) 
		                                      + (Memory[index+3].upper << 28);
		
		_cpu->Registers[4] += 4;
		_cpu->PC += 2;
		return;
		
	}else{
		
		fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS, THERE IS AN ARITHMETIC OPERATION THAT DOn'T WORK SON\n");
		_cpu->status = 3;
		return;
	}
	
	
	

	return;

}
void small_Instruction(struct cpu* _cpu, struct byte* Memory, struct smallInstruction* bite){
	int index;
	
	
	if(_cpu == NULL || bite == NULL){
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			_cpu->status = 3;
			return;
	}
	if(bite[0].opcode0 == 0){/*NO OP*/
		
		_cpu->status = 0;
		_cpu->PC += 1;
		return;
				
	}else if(bite[0].opcode0 == 1){/*HALT*/

		_cpu->status = 1;
		_cpu->PC += 1;
		return;
		
	}else if(bite[0].opcode0 == 9){/*RETURN*/
		
		
		index = _cpu->Registers[4];
		
		_cpu->PC = 		(Memory[index+0].lower      )
		              + (Memory[index+0].upper <<  4) 
		                                      
		              + (Memory[index+1].lower <<  8) 
		              + (Memory[index+1].upper << 12) 
		                                      
		              + (Memory[index+2].lower << 16) 
		              + (Memory[index+2].upper << 20) 
		                                      
		              + (Memory[index+3].lower << 24) 
		              + (Memory[index+3].upper << 28);
		
		 _cpu->Registers[4] += 4;

		
		return;
		
	}else{
		
		fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
		_cpu->status = 3;
		return;
	}


}
void noreg_Instruction(struct cpu* _cpu, struct byte* Memory, struct noregInstruction* JP){
	int temp;
	int index;
	
	if(_cpu == NULL || JP == NULL){
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");	
			_cpu->status = 3;
			return;
	}
	
	if(JP->instr.opcode0 == 7){/*VARIOUS JUMPS*/
		
		
		if(JP->instr.opcode1==0){/*JMP*/
			_cpu->PC = JP->constant; 
			return; 
			
		}else if(JP->instr.opcode1==1){/*JLE*/
			if ((_cpu->SF ^ _cpu->OF) || (_cpu->ZF)) {
		
			
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;   
			}
			
		}else if(JP->instr.opcode1==2){/*JL*/
			if(_cpu->SF ^ _cpu->OF){
							
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;
			}
			
		}else if(JP->instr.opcode1==3){/*JE*/
			if(_cpu->ZF){
							
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;
			}
			
		}else if(JP->instr.opcode1==4){/*JNE*/
			if(!(_cpu->ZF)){
						
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;
			}
			
		}else if(JP->instr.opcode1==5){/*JGE*/
			if(!(_cpu->SF ^ _cpu->OF)){
						
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;
			}
			
		}else if(JP->instr.opcode1==6){/*JG*/
			if(!(_cpu->SF ^ _cpu->OF) && !(_cpu->ZF)){
					
				_cpu->PC = JP->constant;
				return;
				
			}else{
				_cpu->PC += 5;
				return;
			}
			
		}else{
			fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");
			_cpu->status = 3;
			return;
		}
	}else if(JP->instr.opcode0 == 8){/*CALL*/
		
		temp = _cpu->PC+5;
		
		
		
		index = _cpu->Registers[4] -4;
		
		Memory[index+3].upper = (temp & 0xf0000000) >> 28;
		Memory[index+3].lower = (temp & 0x0f000000) >> 24;
			
		Memory[index+2].upper = (temp & 0x00f00000) >> 20;
		Memory[index+2].lower = (temp & 0x000f0000) >> 16;
			
		Memory[index+1].upper = (temp & 0x0000f000) >> 12;
		Memory[index+1].lower = (temp & 0x00000f00) >>  8;
			
		Memory[index+0].upper = (temp & 0x000000f0) >>  4;
		Memory[index+0].lower = (temp & 0x0000000f)      ;
		
		index = _cpu->PC;
		
		temp =  		(Memory[index+1].lower      )
		              + (Memory[index+1].upper <<  4) 
		                                      
		              + (Memory[index+2].lower <<  8) 
		              + (Memory[index+2].upper << 12) 
		                                      
		              + (Memory[index+3].lower << 16) 
		              + (Memory[index+3].upper << 20) 
		                                      
		              + (Memory[index+4].lower << 24) 
		              + (Memory[index+4].upper << 28);
		              
		 _cpu->Registers[4] -= 4;
		 _cpu->PC = temp;
		
				
		return;
	}else{
		
		fprintf(stderr, "SOMTHING WENT TERRIBLY WRONG WITH THE CONSTRUCTION OF YOUR INSTRUCTIONS\n");
		_cpu->status = 3;
		return;
	}
	return;
}

