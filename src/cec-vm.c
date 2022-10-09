#include <stdio.h>
#include "balvm.h"
#include "balvm-instructions.h"

unsigned char *program;
extern unsigned char* threads[MAX_THREADS];
extern unsigned char states[MAX_STATES];
extern unsigned char joins[MAX_JOINS];
extern unsigned char signals[MAX_SIGNALS];
extern int stack[MAX_STACK];
extern int registers[MAX_REGISTERS];
unsigned char *testVectors;

char readProgram(char* filename);
void writeSignals(int tick, FILE * pFile);
void initializeVars();
void debugProgram();
char readTestVectors(char* filename);
char initializeVarsForTick();

int main(int argc, char* argv[]){
  if(argc < 4){
    printf("usage: desk_vm input.bal input.tv output.simout\n");
    return 1;
  }
  // initialize program
  initializeVars();
  if(readProgram(argv[1]) != NO_ERROR) return 1;
  debugProgram();
  printf("checkpoint 1\n");
  
   // Open the TV file containing the test vectors
  FILE * testFile;
  testFile = fopen(argv[2],"r");
  if(testFile == NULL){
    printf("Test Vectors File open failure!\n");
    return READ_ERROR;
  }
 
  // create simout file
  FILE * pFile;
  pFile = fopen(argv[3], "w+");
  if(pFile == NULL){
    printf("File open failue\n");
    return READ_ERROR;
  }
  
  // execute program
  int tickct = 1;
  char val;
  int tickerror = 0;
  char signalInitError;
  do {
    /*
    printf("All the signals: \n");
    int i;
    for(i = 0; i < MAX_SIGNALS; i++){
       printf("signals[%d]: %x\n", i, signals[i]);
    }
    */

    signalInitError = initializeVarsForTick(testFile);
    if(signalInitError){
	printf("\nNo more signals found to simulate\n");
	return signalInitError;
    }
    tickerror = tick();
    writeSignals(tickct, pFile);
    if(!tickerror){
      tickct++;
      /*
      printf("y to continue: ");
      val = (char)getchar();
      if (val != '\n') (char)getchar();
      */
      val = 'n'; // FIXME
    }
  } while(val == 'y' && !tickerror);

  // close simout file
  fclose(pFile);
  printf("checkpoint 2\n");

  return 0;
}

char readProgram(char* filename){
  FILE * pFile;
  long lSize;
  
  pFile = fopen(filename, "r+");
  if(pFile == NULL){
    printf("File open failue\n");
    return READ_ERROR;
  }

  fseek(pFile, 0, SEEK_END);
  lSize = ftell(pFile);
  rewind(pFile);

//  if(lSize > MAX_PROG_SIZE){
//    printf("Max size exceeded failure. Size is %ld\n", lSize);
//    return SIZE_ERROR;
//  }

  fread(program, 1, lSize, pFile);
  fclose(pFile);

  states[0] = program[0]; /* Initial state is first byte in the file */
  return NO_ERROR;
}

void writeSignals(int tick, FILE * pFile){
  int i;
  fprintf(pFile, " %i ", tick);
  for(i = OUTPUT_SIGNALS_START; i < OUTPUT_SIGNALS_END; i++){
    fprintf(pFile, "%d=%i", i, signals[i]);
  }
  fprintf(pFile, "\n");
}

void initializeVars(){
  int i;
  for(i = 0; i < MAX_PROG_SIZE; i++){
    program[i] = 0x00;
  }
  for(i = 0; i < MAX_THREADS; i++){
    threads[i] = program + BAL_PROGRAM_OFFSET;
  }
  for(i = 0; i < MAX_JOINS; i++){
    joins[i] = 0x00;
  }
  for(i = 0; i < MAX_SIGNALS; i++){
    signals[i] = 0x00;
  }
  for(i = 0; i < MAX_STATES; i++){
    states[i] = 0x00;
  }
}

void debugProgram(){
  printf("In debug!\n");
  FILE * pFile;
  pFile = fopen("byteout.prog", "w+");
  int byt = 0;
  unsigned char * pc = program;
  for(;;){
    char opcode = *pc;
    fprintf(pFile, "%i\t", byt);
    switch(opcode & 0x1f){
    case NOP:
      fprintf(pFile, "NOP\n");
      break;
    case EXIT:
      fprintf(pFile, "EXIT\n");
      break;
    case END:
      fprintf(pFile, "END\n");
      break;
    case EMT:
      fprintf(pFile, "EMT ");
      ++pc;
      ++byt;
      fprintf(pFile, "%i\n", (*pc & 0xFF));
      break;
    case SWC:
      fprintf(pFile, "SWC ");
      ++pc;
      ++byt;
      fprintf(pFile, "%i\n", (*pc & 0xFF));
      break;
    case JMP:
      fprintf(pFile, "JMP ");
      ++pc;
      unsigned char high = *pc;
      ++pc;
      unsigned char low = *pc;      
      int address = (high << 8) + low;
      fprintf(pFile, "%i\n", address);
      byt += 2;
      break;
    case STHR:
      fprintf(pFile, "STHR ");
      ++pc;
      fprintf(pFile, "%i ", (*pc & 0xFF));
      ++pc;
      high = *pc;
      ++pc;
      low = *pc;      
      address = (high << 8) + low;
      fprintf(pFile, "%i\n", address);
      byt += 3;
      break;
    case TRM:
      fprintf(pFile, "TRM ");
      // &= -( 1<< a)
      ++pc;
      fprintf(pFile, "%i ", (*pc & 0xFF));
      ++pc;
      fprintf(pFile, "%i\n", (*pc & 0xFF));
      byt += 2;
      break;
    case TWB:
      fprintf(pFile, "TWB ");
      int type = (*pc & 0x60);
      type = type >> 5;
      fprintf(pFile, "%i ", type);
      ++pc;
      fprintf(pFile, "%i ", (*pc & 0xFF));
      ++pc;
      high = *pc;
      ++pc;
      low = *pc;      
      address = (high << 8) + low;
      fprintf(pFile, "%i\n", address);
      byt += 3;
      break;
    case SSIG:
      fprintf(pFile, "SSIG ");
      char val = (*pc & 0x20);
      val = val >> 5;
      ++pc;
      ++byt;
      fprintf(pFile, "%i ", (*pc & 0xFF));
      fprintf(pFile, "%i\n", val);
      break;
    case SSTT:
      fprintf(pFile, "SSTT ");
      ++pc;
      fprintf(pFile, "%i ", (*pc & 0xFF));
      ++pc;
      fprintf(pFile, "%i\n", (*pc & 0xFF));
      byt += 2;
      break;
    case STPT:
      fprintf(pFile, "STPT\n");
      break;
    case MWB:
      fprintf(pFile, "MWB ");
      val = *pc & 0x20;
      val = val >> 5;
      fprintf(pFile, "%i ", val);
      ++pc; 
      int num = *pc & 0xFF;
      ++pc;
      byt += 2;
      fprintf(pFile, "%i ", num);
      fprintf(pFile, "%i ", *pc & 0xFF);
      int i;
      for(i = 0; i < num; i++){
	++pc;
	high = *pc;
	++pc;
	low = *pc;      
	address = (high << 8) + low;
	fprintf(pFile, "%i ", address);
	byt += 2;
      }
      fprintf(pFile, "\n");
      break;
    default:
      fprintf(pFile, "default case\n");
      return;
    }
    ++pc;
    ++byt;
  }
  fclose(pFile);
}

char initializeVarsForTick(FILE* testFile){
	char c;
	int i;

	// Initialize all signals to zero
 	for(i = 0; i < MAX_SIGNALS; i++){
    		signals[i] = 0x00;
  	}

	for(i = INPUT_SIGNALS_START; i <= INPUT_SIGNALS_END ; i++){
		fscanf(testFile, " %c", &c);
		if(c == '1'){
			signals[i] = 0x01;
		}
	}
	// Move until the end of line
	while ( (c = fgetc(testFile)) != '\n'){
		if(c == EOF){
			return NO_MORE_SIGNALS;
		}
	}

        for(i = 0; i < MAX_THREADS; i++){
		// Changed to accommodate removal of SWCU
		// and NR blocks
		if(i > 0)
			threads[i] = NULL;
		else
			threads[i] = program;
  	}

	for(i = 0; i < MAX_JOINS; i++){
    		joins[i] = 0x00;
  	}

	return NO_ERROR;
}


