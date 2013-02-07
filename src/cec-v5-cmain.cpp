#include "IR.hpp"
#include "AST.hpp"
#include "CPrinter.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace AST;

void do_esterel_v5_simulator(std::ostream &o, Module *mod,
			     unsigned int seed, int cycles,
			     string basename)
{
  string mn = mod->symbol->name;

  o <<
    "#  include <time.h>"       "\n"
    "#ifdef H8" "\n"
    "#  include <conio.h>" "\n"
    "#else" "\n"
    "#  include <stdio.h>"      "\n"
    "#  include <stdlib.h>"     "\n"
    "#endif" "\n"
    "typedef char boolean;"   "\n"
    "typedef int integer;"    "\n"
    "typedef char* string;"   "\n"
    "#  define STRLEN 81"       "\n"
    "char *argv0;"            "\n"
    "#  define MAIN" "\n"
    "\n";

  CPrinter::Printer p(o, *mod, false);
  p.printInclude(basename);

  // Generate code for variables that store the presence and value
  // of output signals and for functions (module_O_signal) that record
  // the emission of signals

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->kind == SignalSymbol::Output ||
	 ss->kind == SignalSymbol::Inputoutput ) {
      o << "boolean presence_" << ss->name << ";"    "\n";
      if (ss->type)
	o << ss->type->name << " value_" << ss->name << ";"   "\n";
      o << "void " << mn << "_O_" << ss->name << "(";
      if (ss->type) {
	// Idiosyncracy: the linkage of float-valued signal output routines
	// isn't defined in the V5-generated code.  Even though it
	// should be a float, it's promoted to a double, so this output
	// function is forced to also expect a double
	if ( ss->type->name == "float" )
	  o << "double";
	else
	  o << ss->type->name;
	o << " value";
      }
      o << ")"  "\n"
	"{"  "\n"
	"  presence_" << ss->name << " = 1;"  "\n";
      if (ss->type)
	o << "  value_" << ss->name << " = value;" "\n";
      o << "}" "\n"
	"\n";
    }
  }

  // Declare an external function for each input signal

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->name != "tick" &&
	 (ss->kind == SignalSymbol::Input ||
	  ss->kind == SignalSymbol::Inputoutput) ) {
      o << "extern void " << mn << "_I_" << ss->name << "(";
      if (ss->type) o << ss->type->name;
      o << ");\n";
    }
  }
  o << '\n';

  // Generate a function that reads a single line in the test vector file
  // and calls the corresponding input functions

  o <<
    "#ifndef H8" "\n"
    "int read_testline(FILE *fi)"   "\n"
    "{"                   "\n"
    "  char p;"           "\n"
    "  int c;"            "\n"
    "  int i;"            "\n"
    "  float f;"          "\n"
    "  double d;"         "\n"
    "  char s[STRLEN];"   "\n"
    ;
  
  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->name != "tick" &&
	 (ss->kind == SignalSymbol::Input ||
	  ss->kind == SignalSymbol::Inputoutput) ) {
      o <<
	"  if (fscanf(fi, \" %c\", &p) != 1) return -1;"   "\n"
	"  if (p == '1') {" "\n";
      if (ss->type) {
	char var;
	if (ss->type->name == "integer" || ss->type->name == "boolean") {
	  o << "    if (fscanf(fi, \"=%d\", &i) != 1) return -1;"   "\n";
	  var = 'i';
	} else if (ss->type->name == "string") {
	  o << "    if (fscanf(fi, \"=%80s\", s) != 1) return -1;"   "\n";
	  var = 's';
	} else if (ss->type->name == "double" || ss->type->name == "float") {
	  o << "    if (fscanf(fi, \"=%lf\", &d) != 1) return -1;"   "\n";
	  // o << "printf(\"got %g\\n\", d);\n";
	  var = 'd';
	} else {
	  throw IR::Error("Unrecognized input signal type " + ss->type->name);
	}
	o << "    " << mn << "_I_" << ss->name << "(" << var << ");\n";
      } else {
	o << "    " << mn << "_I_" << ss->name << "();\n";
      }
      o << "  }"  "\n";
    }
  }

  // Code that skips to the next line

  o <<
    "  while ( (c = fgetc(fi)) != '\\n')" "\n"
    "    if (c == EOF) return -1;"  "\n"
    "  return 0;"   "\n"
    "}"    "\n"
    "#endif" "\n"
    "\n";

  // Generate code that applies random stimulus to the inputs

  o <<
    "int apply_random_stimulus()"      "\n"
    "{"                                "\n";

  srandom(seed); // Initialize random number generator to ensure determinism

  for ( int c = 0 ; c < cycles ; c++ ) {
    for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	  i != mod->signals->end() ; i++ ) {
      SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
      assert(ss);
      if ( ss->name != "tick" &&
	   (ss->kind == SignalSymbol::Input ||
	    ss->kind == SignalSymbol::Inputoutput) ) {
	if (random() > RAND_MAX / 2) {
	  o << "  " << mn << "_I_" << ss->name << '(';
	  if (ss->type) {
	    if (ss->type->name == "integer") {
	      o << int(random());
	    } else if (ss->type->name == "boolean") {
	      o << ( (random() > RAND_MAX / 2) ? '1' : '0');
	    } else if (ss->type->name == "double") {
	      o << ( double(random()) / 32768.0 );
	    } else if (ss->type->name == "float") {
	      o << ( double(random()) / 32768.0 );
	    } else if (ss->type->name == "string") {
	      o << "\"string" << random() << '\"';
	    }
	  }
	  o << ");\n";
	}
      }
    }
    // Call the "tick" function and return if it has terminated
    o << "  if (!" << mn << "()) return " << c << ";\n";
  }

  o <<
    "  return " << cycles << ";"  "\n"
    "}"            "\n"
    "\n";
  
  o <<
    "#ifndef H8" "\n"
    "void Usage()"    "\n"
    "{"    "\n"
    "  fprintf(stderr, \"Usage: %s [-c <cycles>] [-t <testvector file>] [-b]\\n\", argv0);"    "\n"
    "  exit(-1);"    "\n"
    "}"    "\n"
    "#endif"    "\n"
    "\n"
    "extern int " << mn << "();"    "\n"
    "\n"
    "int main(int argc, char *argv[])"    "\n"
    "{"    "\n"
    "  int active, ntick, maxtick = -1;"    "\n"
    "  int benchtick = 0;"                  "\n"
    "#ifdef H8" "\n"
    "time_t start, finish;" "\n"
    "benchtick = 1000;" "\n"
    "#else" "\n"
    "  char *fname = NULL;"    "\n"
    "  FILE *f = NULL;"    "\n"
    "  clock_t start, finish;"   "\n"
    "  double seconds;"          "\n"
    "#endif"
    "\n"
    "#ifndef H8" "\n"
    "  argv0 = argv[0];"    "\n"
    "  ntick = 0;"    "\n"
    "  --argc; ++argv;"    "\n"
    "  while (argc > 0 && argv[0][0]==\'-\') {"   "\n"
    "    switch(argv[0][1]){"    "\n"
    "    case 'c':"  "\n"
    "      --argc; ++argv; if (!argc) Usage();"   "\n"
    "      maxtick = atoi(argv[0]);"      "\n"
    "      break;"    "\n"
    "    case 't':"   "\n"
    "      --argc; ++argv; if (!argc) Usage();"   "\n"
    "      fname = argv[0]; break;"    "\n"
    "    case 'b':"     "\n"
    "      benchtick = 1000;"            "\n"
    "      break;"                                "\n"
    "    default: Usage();"    "\n"
    "    }"    "\n"
    "    --argc; ++argv;"    "\n"
    "  }"    "\n"
    "#endif" "\n"
    "\n" <<
    mn << "_reset();\n"
    "#ifdef H8" "\n"
    "  cputs(\"str\");" "\n"
    "  start = get_system_up_time();" "\n"
    "#else" "\n"
    "  if (benchtick) {"            "\n"
    "    do {" 					    "\n"
    "      benchtick *= 10;"                        "\n"
    "      start = clock();"                        "\n"
    "#endif" "\n"
    "      for (ntick = 0 ; ntick < benchtick ; ) {"  "\n"
    "        active = apply_random_stimulus();"       "\n"
    "        ntick += active;"    "\n"
    "        if (active < " << cycles << ") break;"   "\n"
    "      }"            "\n"
    "#ifdef H8" "\n"
    "  finish = get_system_up_time();" "\n"
    "  time_t diff = finish - start;" "\n"
    "  cputw(diff);" "\n"
    "#else" "\n"
    "    finish = clock();"    "\n"
    "    seconds = (double) ((finish - start) / (double) CLOCKS_PER_SEC);" "\n"
    "   } while (seconds < 1.0);" "\n" 
    "    printf(\"" << mn << " %d cycles %g seconds %g average\\n\", ntick, \n"
    "           seconds, seconds / (double) benchtick);"  "\n"
    "  } else {"                    "\n"
    "    if (fname)"    "\n"
    "      if ((f = fopen(fname,\"r\")) == NULL) {"    "\n"
    "        fprintf(stderr,\"Cannot open testvector file `%s'. Exiting\\n\", fname);"    "\n"
    "        exit(-2);"    "\n"
    "      }"    "\n"
    "\n"
    "    do {"    "\n"
    "      if ( f != NULL) if (read_testline(f) == -1) break;"    "\n";

  // Generate code that resets output signal presence variables

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->kind == SignalSymbol::Output ||
	 ss->kind == SignalSymbol::Inputoutput )
      o << "      presence_" << ss->name << " = 0;\n";
  }

  o << "      active = " << mn << "();\n";

  o << "      printf(\"%4d \", ntick);\n";

  // Generate code that prints the presence/absence status of each signal

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->kind == SignalSymbol::Output ||
	 ss->kind == SignalSymbol::Inputoutput ) {
      
      o << "      printf(\"" << ss->name;
      if (ss->kind == SignalSymbol::Inputoutput) o << "_IO_O";
      o << "=\");\n";
      o << "      if (presence_" << ss->name;
      o << ") printf(\"1";
      if ( ss->type ) {
	if ( ss->type->name == "integer" || ss->type->name == "boolean" )
	  o << " (%d) \", value_" << ss->name << ");\n";
	else if ( ss->type->name == "string" )
	  o << " (%s) \", value_" << ss->name << ");\n";
	else if ( ss->type->name == "float" || ss->type->name == "double" )
	  o << " (%g) \", value_" << ss->name << ");\n";
	else
	  o << " <unprintable> \");\n";
      } else {
	  o << " \");\n";
      }
      o << "      else printf(\"0 \");\n";
    }
  }

  o <<
    "      printf(\"\\n\");"    "\n"
    "      ntick++;"    "\n"
    "    } while (active && ntick != maxtick);"    "\n"
    "    if (f!=NULL) fclose(f);"    "\n"
    "  }"                            "\n"
    "#endif" "\n"
    "  return 0;"    "\n"
    "}"    "\n";
}

struct Usage {};

int main(int argc, char* argv[])
{
  int cycles = 100;
  unsigned int seed = 1;
  
  try {
    std::string basename;

    argc--, argv++;  // Skip program name
    
    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'B':
        argc--, argv++;
        if (argc == 0) throw Usage();
        basename = argv[0];
        break;
      case 'h':
      default:
        throw Usage();
        break;
      }
      argc--, argv++;
    }
    
    if ( argc > 0 ) throw Usage();   

    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;

    Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    AST::Module *m = mods->modules.front();
    assert(m);

    if (basename.empty()) basename = m->symbol->name;
          
    SymbolTable *sig = dynamic_cast<AST::SymbolTable*>(m->signals);
    assert(sig);
      
    do_esterel_v5_simulator(std::cout, m, seed, cycles, basename);

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (Usage &) {
    std::cerr << "Usage: cec-v5-cmain [-B basename]" << std::endl;
    exit(-1);
  }

  return 0;
}
