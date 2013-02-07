/*

  Generate a wrapper for the virtual machine

  Usage: cec-vm-wrapper < myfile.opt.xml > myfile.vm-wrapper.c

*/

#include "IR.hpp"
#include "AST.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>
#include "CPrinter.hpp"

using namespace std;
using namespace AST;

void create_vm_wrapper(std::ostream &o, Module *mod, string basename)
{
  string mn = mod->symbol->name;

  o <<
    "\n"
    "typedef int integer;" "\n"
    "typedef int boolean;" "\n";

  CPrinter::Printer p(o, *mod, false);
  p.printInclude(basename);

  o << '\n';

  // Generate code for the _reset function

  o <<
    "\n"
    "int " << mn << "_reset() {" "\n"
    "" "\n"
    "  int i;" "\n"
    "  for(i = MAX_SIGNALS ; --i >= 0 ; ) signals[i] = 0x00;" "\n"
    "  threads[0] = program + BAL_PROGRAM_OFFSET;" "\n"
    "  for(i = MAX_JOINS ; --i >= 0 ; ) joins[i] = 0x00;" "\n"
    "  for(i = MAX_STATES ; --i > 0 ; ) states[i] = 0x00;" "\n"
    "  states[0] = program[0];" "\n";

  // Generate assignments to uninitialized constants, assumed to
  // be taken from an included .h file

  int var_num = 0;
  for ( SymbolTable::const_iterator i = mod->constants->begin();
	i != mod->constants->end() ; i++ ) {
    ConstantSymbol *cs = dynamic_cast<ConstantSymbol*>(*i);
    if(cs && !cs->initializer) {
      o << "  registers[" << var_num << "] = " << cs->name << ";\n";
      var_num++;
    }
  }

  o <<
    "  return 1;" "\n"
    "}" "\n";

  // Declare a function that sets a flag in the signals array
  // for each input signal
  
  int signal_number = 0;
  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; ++i ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->name != "tick" ) {
      if (ss->kind == SignalSymbol::Input) {
	o << "void " << mn << "_I_" << ss->name << "(";
	if (ss->type) o << ss->type->name << " v";
	o << ") { signals[" << signal_number << "] = 1; ";
	if (ss->type) o << " registers[" << var_num++ << "] = v;";
	o << "}\n";
	++signal_number;
      }
    }
  }
  o << '\n';

  int first_inputoutput_signal = signal_number;
  int first_inputoutput_register = var_num;

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; ++i ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if (ss->kind == SignalSymbol::Inputoutput) {
      o << "void " << mn << "_I_" << ss->name << "(";
      if (ss->type) o << ss->type->name << " v";
      o << ") { signals[" << signal_number << "] = 1; ";
      if (ss->type) o << " registers[" << var_num++ << "] = v;";
      o << "}\n";
      ++signal_number;
    }
  }

  signal_number = first_inputoutput_signal;
  var_num = first_inputoutput_register;

  // Define the main "tick" function whose name follows the module name

  o <<
    "int " << mn << "() {" "\n"
    "" "\n"
    "  int i;" "\n"
    "  tick();" "\n"
    "\n";

  // Generate a test for each output signal

  for ( SymbolTable::const_iterator i = mod->signals->begin() ;
	i != mod->signals->end() ; ++i ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if ( ss->name != "tick" ) {
      if (ss->kind == SignalSymbol::Output ||
	  ss->kind == SignalSymbol::Inputoutput) {
	o << "  if (signals[" << signal_number << "]) "
	  << mn << "_O_" << ss->name << "(";
	  if (ss->type) o <<  "registers[" << var_num++ << "]"; // valued signals
	o << ");\n";
	++signal_number;
      }
    }
  }
  o << '\n';

  o <<
    "  for(i = MAX_SIGNALS ; --i >= 0 ; ) signals[i] = 0x00;" "\n"
    "  threads[0] = program + BAL_PROGRAM_OFFSET;" "\n"
    "  for(i = MAX_JOINS ; --i >= 0 ; ) joins[i] = 0x00;" "\n";

  // Check whether the program will ever terminate

  GRCgraph *g = dynamic_cast<AST::GRCgraph*>(mod->body);
  STexcl *excl = dynamic_cast<STexcl*>(g->selection_tree);
  assert(excl);
  assert(!excl->children.empty());
  STleaf *terminal_leaf = dynamic_cast<STleaf*>(excl->children.front());
  if (terminal_leaf && terminal_leaf->isfinal())
    o << "  return states[0] != 0;\n";
  else
    o << "  return 1;\n";

  o <<
    "}" "\n";
}

struct Usage {};

int main(int argc, char* argv[])
{
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

    create_vm_wrapper(std::cout, m, basename);

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (Usage &) {
    std::cerr << "Usage: cec-vm-wrapper [-B basename]" << std::endl;
    exit(-1);
  }
  return 0;
}
