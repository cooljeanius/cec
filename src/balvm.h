#ifndef _BALVM_H
#  define _BALVM_H

extern int tick(void);

#  define MAX_THREADS		255
#  define MAX_STATES		255
#  define MAX_JOINS 		64
#  define MAX_SIGNALS		255
#  define MAX_STACK             32
#  define MAX_REGISTERS         255

#  define MAX_PROG_SIZE         2048
#  define NO_ERROR      	0
#  define FATAL_ERROR		1
#  define UNKNOWN_ERROR		2
#  define READ_ERROR 		3
#  define SIZE_ERROR 		4
#  define TV_READ_ERROR		5
#  define NO_MORE_SIGNALS	6

#  define INPUT_SIGNALS_START	0
#  define INPUT_SIGNALS_END	2
#  define OUTPUT_SIGNALS_START	3
#  define OUTPUT_SIGNALS_END	6

/* Number of bytes to skip at the beginning of the bytecode */

#  define BAL_PROGRAM_OFFSET    1

extern unsigned char *program;
extern unsigned char * threads[MAX_THREADS];
extern unsigned char states[MAX_STATES];
extern unsigned char joins[MAX_JOINS];
extern unsigned char signals[MAX_SIGNALS];
extern int stack[MAX_STACK];
extern int registers[MAX_REGISTERS];

#endif
