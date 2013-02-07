/* The Esterel Virtual Machine  */

#ifdef DEBUG
#  include <stdio.h>
#endif

#define MAX_THREADS	255
#define MAX_STATES	255
#define MAX_JOINS 	64
#define MAX_SIGNALS	255
#define MAX_STACK       32
#define MAX_REGISTERS   255

#define NO_ERROR        0
#define UNKNOWN_ERROR   1

/* Number of bytes to skip at the beginning of the bytecode */

#  define BAL_PROGRAM_OFFSET    1

#define EMIT 0
#define RST 1
#define SET 2
#define STRT 3
#define TICK 4
#define JMP 5
#define BST 6
#define BCC 7
#define BNZ 8
#define SWC 9
#define TRM 11
#define LDI 12
#define LDS 13
#define LDR 14
#define STR 15
#define ADD 16
#define SUB 17
#define MUL 18
#define DIV 19
#define MOD 20
#define CMP 21
#define NEQ 22
#define LT 23
#define LEQ 24
#define GT 25
#define GEQ 26
#define AND 27
#define OR 28
#define NEG 29
#define NOT 30
#define CDEC 31

#define WORD(offset) (pc[(offset)] << 8 | pc[(offset)+1])
#define BINOP(op) arg1 = (arg1 op arg2)
#define UNARY(op) sp[-1] = op sp[-1];

extern unsigned char *program;
unsigned char *threads[MAX_THREADS]; /* Thread program counters */
unsigned char states[MAX_STATES];     /* Inter-cycle state variables */
unsigned char joins[MAX_JOINS];       /* Completion codes */
unsigned char signals[MAX_SIGNALS];   /* Signal status variables */
int stack[MAX_STACK];                 /* Arithmetic stack */
int registers[MAX_REGISTERS];         /* Variables, signal values, etc. */

int tick() {
  int myth = 0;
  unsigned char *pc = threads[myth];
  int arg1, arg2;

  unsigned char tmp;
  int *sp = stack;
  for(;;) {
#ifdef DEBUG
    printf("%.4x %.2x\n", pc - program, *pc);
#endif
    switch (*pc++) {
    case EMIT: signals[*pc++] = 1; break;
    case RST:  signals[*pc++] = 0; break;
    case SET:  states[pc[0]] = pc[1]; pc += 2; break;
    case STRT: threads[pc[0]] = program + WORD(1); pc += 3; break;
    case TICK: return NO_ERROR;
    case JMP:  pc = program + WORD(0); break;
    case BST:  tmp = states[pc[0]]; pc = program + WORD(2*tmp+1); break;
    case BCC:  tmp = joins[pc[0]]; pc = program + WORD(2*tmp+1); break;
    case BNZ:  if (*(--sp)) pc = program + WORD(0); else pc += 2; break;
    case SWC:  tmp = pc[0]; threads[myth] = ++pc; pc = threads[myth = tmp];
      break;

    case TRM:  if (joins[pc[0]] < pc[1]) joins[pc[0]] = pc[1]; pc += 2; break;
    case LDI:  *(sp++) = WORD(0); pc += 2; break;
    case LDS:  *(sp++) = signals[*pc++]; break;
    case LDR:  *(sp++) = registers[*pc++]; break;
    case STR:  registers[*pc++] = (*(--sp)); break;
      	    	
      /* Binary operators */
    case ADD: case SUB: case MUL: case DIV: case MOD:
    case CMP: case NEQ: case LT: case LEQ: case GT: case GEQ:
    case AND: case OR:
      --sp;
      arg1 = sp[-1];
      arg2 = sp[0];
      switch (pc[-1]) {
      default: ; /* never taken */
      case ADD:  BINOP(+); break;
      case SUB:  BINOP(-); break;
      case MUL:  BINOP(*); break;
      case DIV:  BINOP(/); break;
      case MOD:  BINOP(%); break;
      case CMP:  BINOP(==); break;
      case NEQ:  BINOP(-); break;
      case LT:   BINOP(<); break;
      case LEQ:  BINOP(<=); break;
      case GT:   BINOP(>); break;
      case GEQ:  BINOP(>=); break;
      case AND:  BINOP(&&); break;
      case OR:   BINOP(||); break;
      }
      sp[-1] = arg1;
      break;
    case NEG:  UNARY(-); break;
    case NOT:  UNARY(!); break;
    case CDEC:
      sp[-1] = 0 == (sp[-1] ? (--registers[*pc++]) : registers[*pc++]);
      break;

    default:
#ifdef DEBUG
      printf("Unrecognized instruction %d\n", pc[-1]);
#endif
      return UNKNOWN_ERROR;
    }
  }
}

