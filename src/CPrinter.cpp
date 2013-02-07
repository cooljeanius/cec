#line 1665 "CPrinter.nw"
#include "CPrinter.hpp"
#include <stdio.h>

namespace CPrinter {
  
#line 71 "CPrinter.nw"
int Printer::nextLabel = 0;
#line 75 "CPrinter.nw"
Printer::Printer(std::ostream &o, Module &m, bool three_val)
   : o(o), m(m), do_threevalued(three_val)
{
  g = dynamic_cast<AST::GRCgraph*>(m.body);
  if (!g) throw IR::Error("Module is not in GRC format");

  // Enumerate selection tree and CFG nodes
  g->enumerate(cfgmap, stmap);

  // Enter C reserved words into the identifiers list to avoid collisions

  // Note: float and double aren't in this list because they are equivalent
  // to Esterel's types of the same name

  char *keywords[] = {
    "int", "break", "char", "continue", "if", "else",
    "struct", "for", "auto", "do", "extern", "while", "register", "switch",
    "static", "case", "goto", "default", "return", "entry", "sizeof", NULL
  };

  for (char **k = keywords ; *k != NULL ; k++) identifiers.insert(*k);
}
#line 109 "CPrinter.nw"
string Printer::uniqueID(string name)
{
  string newname = name;

  char buf[10];
  int version = 1;

  while (contains(identifiers, newname)) {
    sprintf(buf, "%d", version++);
    newname = name + '_' + buf;
  }
  
  identifiers.insert(newname);
  return newname;
}
#line 193 "CPrinter.nw"
Status Printer::visit(Enter &e)
{
  STexcl *exclusive = 0;
  STNode *n = e.st;

  for (;;) {
    assert(n);

    STNode *parent = n->parent;

    // If we hit a parallel first, this Enter is unnecessary; do not generate
    // any code
    if (dynamic_cast<STpar*>(parent) != NULL) return Status();

    exclusive = dynamic_cast<STexcl*>(parent);
    if (exclusive != NULL) break; // found the exclusive node
    n = parent;
  }

  assert(exclusive != NULL);

  // Locate node n among the children of "parent"

  vector<STNode*>::iterator i = exclusive->children.begin();
  while (*i != n && i != exclusive->children.end()) i++;

  assert(i != exclusive->children.end());

  int childnum = i - exclusive->children.begin();
  
  assert(childnum >= 0);

  assert(contains(stateVar, exclusive));
  o << stateVar[exclusive] << " = " << childnum;

  return Status();
}
#line 250 "CPrinter.nw"
Status Printer::visit(Terminate &t)
{
  // If we have something other than a single data successor or it is not
  // a Sync, return nothing.
  if (t.dataSuccessors.size() != 1 || t.code == 0) {
    o << "/* Vacuous terminate */";
    return Status();
  }

  Sync *s = dynamic_cast<Sync*>(t.dataSuccessors.front());
  if (s == NULL) return Status();

  if ( contains(terminationVar, s) )
    o << terminationVar[s] << " &= -(1 << " << t.code << ")";
  return Status();
}
#line 317 "CPrinter.nw"
Status Printer::visit(Emit &e)
{
  assert(e.signal);
  if (e.signal->type != NULL) {
    assert(contains(valueVar, e.signal));
    o << "(";
    if (e.value->type->name == "string") {      
      o << "strcpy(" << valueVar[e.signal] << ", ";
      printExpr(e.value);
      o << ")";
    } else {
      o << valueVar[e.signal] << " = ";
      printExpr(e.value);
    }
    o << "), (";
  }
  assert(contains(presenceVar, e.signal));
  if (e.unknown)
    o << presenceVar[e.signal] << "_unknown = 1";
  else
    o << presenceVar[e.signal] << " = 1";
  if (e.signal->type) o << ")";
  return Status();
}
#line 344 "CPrinter.nw"
Status Printer::visit(Exit &e)
{
  assert(e.trap);
  if (e.trap->type) {
    assert(contains(valueVar, e.trap));
    o << valueVar[e.trap] << " = ";
    printExpr(e.value);
    o << ", ";
  }
  assert(contains(presenceVar, e.trap));
  o << presenceVar[e.trap] << " = 1";
  return Status();
}
#line 421 "CPrinter.nw"
Status Printer::visit(StartCounter &s)
{
  assert(s.counter);
  assert(contains(counterVar, s.counter));
  o << counterVar[s.counter] << " = ";
  printExpr(s.count);
  return Status();
}
#line 441 "CPrinter.nw"
Status Printer::visit(CheckCounter &s)
{
  assert(s.counter);
  assert(s.predicate);
  assert(contains(counterVar, s.counter));
  // FIXME: Is this safe?
  if (dynamic_cast<LoadVariableExpression*>(s.predicate)) {
    o << "0 == --" << counterVar[s.counter];
  } else {
    o << "0 == (";
    printExpr(s.predicate);
    o << " ? --" << counterVar[s.counter] << " : "
      << counterVar[s.counter] << ")";
  }
  return Status();
}
#line 512 "CPrinter.nw"
Status Printer::visit(UnaryOp &op)
{
  o << '(';
  string s = op.op;
  if (s == "not") s = "!";
  o << s;
  assert(op.source);
  printExpr(op.source);
  o << ')';
  return Status();
}
#line 526 "CPrinter.nw"
Status Printer::visit(BinaryOp &op)
{
  o << '(';
  assert(op.source1);
  printExpr(op.source1);
  string s = op.op;
  if (s == "mod") s = "%";
  else if (s == "=") s = "==";
  else if (s == "<>") s = "!=";
  else if (s == "and") s = "&&";
  else if (s == "or") s = "||";
  o << ' ' << s << ' ';
  assert(op.source2);
  printExpr(op.source2);
  o << ')';
  return Status();
}
#line 557 "CPrinter.nw"
Status Printer::visit(Literal &l)
{
  assert(l.type);
  if ( l.type->name == "string" ) {
    o << '\"';
    for ( string::iterator i = l.value.begin() ; i != l.value.end() ; i++ ) {
      if (*i == '\"') o << '\\';
      o << *i;
    }
    o << '\"';
  } else {
    o << l.value;
  }
  return Status();
}
#line 585 "CPrinter.nw"
Status Printer::visit(FunctionCall &c)
{
  assert(c.callee);
  if (dynamic_cast<BuiltinFunctionSymbol*>(c.callee)) {
    o << '(';
    switch (c.arguments.size()) {
    case 1:
      if (c.callee->name == "not") {
	o << '!';
      } else {
	o << c.callee->name << ' ';
      }
      printExpr(c.arguments.front());
      break;
    case 2:
      printExpr(c.arguments.front());
      if ( c.callee->name == "and" ) o << " && ";
      else if (c.callee->name == "or" ) o << " || ";
      else if (c.callee->name == "=" ) o << " == ";
      else if (c.callee->name == "<>" ) o << " != ";
      else o << ' ' << c.callee->name << ' ';
      printExpr(c.arguments[1]);
      break;
    default:
      // Not one or two arguments.  What function is this?
      assert(0);
      break;
    }
    o << ')';
  } else {
    o << c.callee->name << '(';
    for ( vector<Expression*>::iterator i = c.arguments.begin() ;
	  i != c.arguments.end() ; i++ ) {
      printExpr(*i);
      if ( i != (c.arguments.end() - 1)) o << ", ";
    }
    o << ')';
  }
  return Status();
}
#line 634 "CPrinter.nw"
Status Printer::visit(ProcedureCall &c)
{
  assert(c.procedure);
  o << c.procedure->name << '(';
  bool needComma = false;
  for ( vector<VariableSymbol*>::iterator i = c.reference_args.begin() ;
	i != c.reference_args.end() ; i++ ) {
    assert(*i);
    if (needComma) o << ", ";
    o << '&' << (*i)->name;
    needComma = true;
  }
  for ( vector<Expression*>::iterator i = c.value_args.begin() ;
	i != c.value_args.end() ; i++ ) {
    if (needComma) o << ", ";
    printExpr(*i);
    needComma = true;
  }
  o << ")";
  return Status();
}
#line 667 "CPrinter.nw"
void Printer::printInclude(string basename)
{

  // Decide whether to #include "basename.h"
  // If there are any procedures, tasks, user-defined types, functions
  // or undefined constants, include it.
  bool needInclude = (m.procedures->size() != 0) || (m.tasks->size() != 0);

  if ( !needInclude )
    for ( SymbolTable::const_iterator i = m.types->begin() ;
	  i != m.types->end() ; i++ )
      if ( dynamic_cast<BuiltinTypeSymbol*>(*i) == NULL ) {
	needInclude = true;
	break;
      }

  if ( !needInclude )
    for ( SymbolTable::const_iterator i = m.constants->begin() ;
	  i != m.constants->end() ; i++ )
      if ( dynamic_cast<BuiltinConstantSymbol*>(*i) == NULL) {
	ConstantSymbol *cs = dynamic_cast<ConstantSymbol*>(*i);
	assert(cs);
	if (cs->initializer == NULL) {
	  needInclude = true;
	  break;
	}
      }

  if ( !needInclude )
    for ( SymbolTable::const_iterator i = m.functions->begin() ;
	  i != m.functions->end() ; i++ )
      if ( dynamic_cast<BuiltinFunctionSymbol*>(*i) == NULL) {
	needInclude = true;
	break;
      }

  if (needInclude)
    o << "#include \"" << basename << ".h\"\n";
}
#line 715 "CPrinter.nw"
void Printer::printDeclarations(string basename)
{

  // Although external types need no declarations, their names
  // are registered to check for later collisions

  o <<
    "#ifndef STRLEN"         "\n"
    "#  define STRLEN 81"    "\n"
    "#endif"                 "\n"
    "#define _true 1"        "\n"
    "#define _false 0"       "\n"
    "typedef unsigned char boolean;"  "\n"
    "typedef int integer;"   "\n"
    "typedef char* string;"  "\n"
    ;

  printInclude(basename);
 
  BuiltinConstantSymbol *truec =
    dynamic_cast<BuiltinConstantSymbol*>(m.constants->get(string("true")));
  assert(truec);
  variableVar[truec] =  uniqueID("_true");
  BuiltinConstantSymbol *falsec =
    dynamic_cast<BuiltinConstantSymbol*>(m.constants->get(string("false")));
  assert(falsec);
  variableVar[falsec] = uniqueID("_false");

  identifiers.insert("STRLEN");

  // Verify all exteral type names are OK

  assert(m.types);
  for ( SymbolTable::const_iterator i = m.types->begin() ;
	i != m.types->end() ; i++ ) {
    TypeSymbol *s = dynamic_cast<TypeSymbol*>(*i);
    assert(s);
    if (contains(identifiers, s->name))
      throw IR::Error("Name of external type \"" + s->name +
		      "\" already in use");
    uniqueID(s->name);
  }

  // Print input function declarations

  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->name != "tick" &&
	( s->kind == SignalSymbol::Input ||
	  s->kind == SignalSymbol::Inputoutput)) {
      assert(m.symbol);
      o << "void " << m.symbol->name << "_I_" << s->name << "(";
      if (s->type) {
	o << s->type->name;
      } else {
	o << "void";
      }
      o << ");\n";
    }
  }

  // Print declarations for the tick and reset functions

  o <<
    "int " << m.symbol->name << "(void);"        "\n"
    "int " << m.symbol->name << "_reset(void);"  "\n";    

  // External declarations (constants, functions, procedures)

  o << "#ifndef _NO_EXTERN_DEFINITIONS"    "\n";

  // Uninitialized constants

  o << "#  ifndef _NO_CONSTANT_DEFINITIONS"   "\n";
  assert(m.constants);
  for ( SymbolTable::const_iterator i = m.constants->begin() ;
	i != m.constants->end() ; i++ ) {
    ConstantSymbol *s = dynamic_cast<ConstantSymbol*>(*i);
    assert(s);
    if (!s->initializer) {
      o << "#    ifndef _" << s->name << "_DEFINED\n";
      o << "#      ifndef " << s->name << "\n";
      assert(s->type);
      if (contains(identifiers, s->name))
	throw IR::Error("Name of constant \"" + s->name + "\" already in use");
      string var = uniqueID(s->name);
      variableVar[s] = var;
      o << "extern " << s->type->name << " " << var << ";\n";
      o << "#      endif\n";
      o << "#    endif\n";
    }
  }
  o << "#  endif /* _NO_CONSTANT_DEFINITIONS */\n";

  // Functions

  o << "#  ifndef _NO_FUNCTION_DEFINITIONS"   "\n";
  assert(m.functions);
  for ( SymbolTable::const_iterator i = m.functions->begin() ;
	i != m.functions->end() ; i++ ) {
    FunctionSymbol *s = dynamic_cast<FunctionSymbol*>(*i);
    assert(s);
    if (dynamic_cast<BuiltinFunctionSymbol*>(*i) == NULL ) {
      o << "#    ifndef _" << s->name << "_DEFINED\n";
      o << "#      ifndef " << s->name << "\n";
      if (contains(identifiers, s->name))
	throw IR::Error("Name of function \"" + s->name + "\" already in use");
      uniqueID(s->name);
      assert(s->result);
      o << "extern " << s->result->name << " " << s->name << "(";
      if (s->arguments.empty()) {
	o << "void";
      } else {
	for ( vector<TypeSymbol*>::const_iterator j = s->arguments.begin() ;
	      j != s->arguments.end() ; j++ ) {
	  assert(*j);
	  o << (*j)->name;
	  if ( j != s->arguments.end() - 1) o << ", ";
	}
      }
      o << ");\n";
      o << "#      endif\n";
      o << "#    endif\n";
    }
  }
  o << "#  endif /* _NO_FUNCTION_DEFINITIONS */\n";

  // Procedures

  o << "#  ifndef _NO_PROCEDURE_DEFINITIONS"   "\n";
  assert(m.procedures);
  for ( SymbolTable::const_iterator i = m.procedures->begin() ;
	i != m.procedures->end() ; i++ ) {
    ProcedureSymbol *s = dynamic_cast<ProcedureSymbol*>(*i);
    assert(s);
    o << "#    ifndef _" << s->name << "_DEFINED\n";
    o << "#      ifndef " << s->name << "\n";
    if (contains(identifiers, s->name))
      throw IR::Error("Name of procedure \"" + s->name + "\" already in use");
    uniqueID(s->name);
    o << "extern void " << s->name << "(";
    for ( vector<TypeSymbol*>::const_iterator j =
	    s->reference_arguments.begin() ;
	  j != s->reference_arguments.end() ; j++ ) {
      assert(*j);
      o << (*j)->name << "*";
      if ( j != s->reference_arguments.end() - 1 ||
	   !s->value_arguments.empty() )
	o << ", ";
    }
    for ( vector<TypeSymbol*>::const_iterator j = s->value_arguments.begin() ;
	  j != s->value_arguments.end() ; j++ ) {
      assert(*j);
      o << (*j)->name;
      if ( j != s->value_arguments.end() - 1) o << ", ";
    }
    o << ");\n";
    o << "#      endif\n";
    o << "#    endif\n";
  }
  o << "#  endif /* _NO_PROCEDURE_DEFINITIONS */\n";

  o << "#endif /* _NO_EXTERN_DEFINITIONS */\n\n";

  // Initialized Constants

  for ( SymbolTable::const_iterator i = m.constants->begin() ;
	i != m.constants->end() ; i++ ) {
    ConstantSymbol *s = dynamic_cast<ConstantSymbol*>(*i);
    assert(s);
    if (s->initializer && dynamic_cast<BuiltinConstantSymbol*>(*i) == NULL) {
      assert(s->type);
      if (contains(identifiers, s->name))
	throw IR::Error("Name of constant \"" + s->name + "\" already in use");
      string var = uniqueID(s->name);
      variableVar[s] = var;
      o << "static " << s->type->name << " " << s->name << " = ";
      printExpr(s->initializer);
      o << ";\n";
    }
  }

  // Variables for signal declarations

#ifdef USE_STRUCTS_FOR_SIGNALS
  assert(m.signals);

  // Define a struct holding all boolean presence variables

  o << "static struct {\n";
  unsigned int n_signals = 0;
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->name != "tick") {
      // All signals, sensors included, have presence variables
      string var = uniqueID(s->name);
      o << "  unsigned int " << var << " : 1;\n";
      if (do_threevalued)
        o << "  unsigned int " << var << "_unknown : 1;\n";
      presenceVar[s] = string("_s.") + var;
      ++n_signals;
    }
  }
  o << "} _s = { ";
  for (unsigned int i = 0 ; i < n_signals ; i++) {
    o << " 0";
    if ( i < n_signals - 1 ) o << ", ";
  }
  o << " };\n";

  // Define value variables for each valued signal

  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->name == "tick") {
      // "tick" is a special built-in signal that is always present
      string var = uniqueID(s->name);
      presenceVar[s] = var;
      o << "#define " << var << " 1\n";
    }
    if (s->type) {
      // Has a type: need a value variable
      if (s->reincarnation) {
	// This is a reincarnation of an earlier signal: use its value variable
	// std::cerr << "Found reincarnation " << s->name << " of " << s->reincarnation->name << std::endl;
	assert(valueVar.find(s->reincarnation) != valueVar.end());
	valueVar[s] = valueVar[s->reincarnation];
      } else {
	string var = uniqueID(s->name + "_v");
	valueVar[s] = var;
	o << "static ";
	if (s->type->name == "string")
	  o << "char " << var << "[STRLEN]";
	else
	  o << s->type->name << " " << var;
	o << ";\n";
      }
    }
  }

#else

  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->name == "tick") {
      // "tick" is a special built-in signal that is always present
      string var = uniqueID(s->name);
      presenceVar[s] = var;
      o << "#define " << var << " 1\n";
    } else {
      // All signals, sensors included, have presence variables
      string var = uniqueID(s->name);
      presenceVar[s] = var;
      o << "static boolean " << var << " = _false;\n";
      if (do_threevalued)
        o << "static boolean " << var << "_unknown = _false;\n";
    }
    if (s->type) {
      // Has a type: need a value variable
      if (s->reincarnation) {
	// This is a reincarnation of an earlier signal: use its value variable
	assert(valueVar.find(s->reincarnation) != valueVar.end());
	valueVar[s] = valueVar[s->reincarnation];
      } else {
	string var = uniqueID(s->name + "_v");
	valueVar[s] = var;
	o << "static ";
	if (s->type->name == "string")
	  o << "char " << var << "[STRLEN]";
	else
	  o << s->type->name << " " << var;
	o << ";\n";
      }
    }
  }

#endif

  // Variable declarations

  assert(m.variables);
  for ( SymbolTable::const_iterator i = m.variables->begin() ;
	i != m.variables->end() ; i++ ) {
    VariableSymbol *s = dynamic_cast<VariableSymbol*>(*i);
    assert(s);
    string var = uniqueID(s->name);
    variableVar[s] = var;
    o << "static ";
    if ( s->type->name == "string" )
      o << "char " << var << "[STRLEN]";
    else
      o << s->type->name << ' ' << var;
    if ( s->initializer ) {
      o << " = ";
      printExpr(s->initializer);
    }
    o << ";\n";
  }

  // State variable declarations

#ifdef USE_STRUCTS_FOR_STATES

  o << "static struct {\n";
  for ( STmap::const_iterator i = stmap.begin() ; i != stmap.end() ; i++ ) {
    STexcl *e = dynamic_cast<STexcl*>((*i).first);
    if (e) {
      char buf[15];
      sprintf(buf, "_%d", stmap[e]);
      stateVar[e] = string("_state.") + string(buf);
      unsigned int bits = 1;
      while ( (1 << bits) < e->children.size() ) ++bits;
      o << "  unsigned int " << buf << " : " << bits << ";\n";
    }
  }
  o << "} _state = { ";
  bool needComma = false;
  for ( STmap::const_iterator i = stmap.begin() ; i != stmap.end() ; i++ ) {
    STexcl *e = dynamic_cast<STexcl*>((*i).first);
    if (e) {   
      // Initialization of states
      if (needComma) o << ", ";
      o << (e->children.size() - 1);
      needComma = true;
    }
  }
  o << " };\n";

#else

  for ( STmap::const_iterator i = stmap.begin() ; i != stmap.end() ; i++ ) {
    STexcl *e = dynamic_cast<STexcl*>((*i).first);
    if (e) {
      char buf[15];
      sprintf(buf, "_state_%d", stmap[e]);
      string var = uniqueID(buf);
      stateVar[e] = var;
      o << "static unsigned char " << var;
      // Initialization of state of selection-tree root:
      //   state = highest-numbered child
      if ( e == g->selection_tree )
	o << " = " << (e->children.size() - 1);
      o << ";\n";
    }
  }
#endif

  // Termination level variable declarations

    for ( CFGmap::const_iterator i = cfgmap.begin() ;
	  i != cfgmap.end() ; i++ ) {
    Sync *s = dynamic_cast<Sync*>((*i).first);
    if (s) {

      // Count the number of non-zero successors

      unsigned int successors = 0;
      for ( vector<GRCNode*>::iterator j = s->successors.begin() ;
	    j != s->successors.end() ; j++ )
	if (*j) ++successors;
      if (successors > 1) {
	// If there is more than one non-NULL successor, generate a variable
	char buf[15];
	sprintf(buf, "_term_%d", cfgmap[s]);
	string var = uniqueID(buf);
	terminationVar[s] = var;
	o << "static int " << var << ";\n";
      }
    }
  }

  // Counter declarations

  for ( vector<Counter*>::const_iterator i = m.counters.begin() ;
	i != m.counters.end() ; i++ ) {
    char buf[15];
    sprintf(buf, "_counter_%d", i-m.counters.begin() );
    string var = uniqueID(buf);
    counterVar[*i] = var;
    o << "static int " << var << ";\n";
  }


#ifdef PRINT_OUTPUT_FUNCTION_DECLARATIONS
  // Output function declarations

  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->kind == SignalSymbol::Output ||
	s->kind == SignalSymbol::Inputoutput) {
      string name = m.symbol->name + "_O_" + s->name;
      o << "#ifndef " << name << "\n"
	"extern void " << name << "(";
      if (s->type) o << s->type->name;
      else o << "void";
      o << ");\n"
	"#endif\n";
    }
  }
#endif

}
#line 1142 "CPrinter.nw"
void Printer::outputFunctions()
{
  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->kind == SignalSymbol::Output ||
	s->kind == SignalSymbol::Inputoutput) {
      assert(contains(presenceVar, s));
      o << "  if (" << presenceVar[s] << ") { ";
      o << m.symbol->name << "_O_" << s->name << "(";
      if (s->type) {
        assert(contains(valueVar, s));
	o << valueVar[s];
      }
      o << "); " << presenceVar[s] << " = 0; }\n";
    }
  }
}
#line 1173 "CPrinter.nw"
void Printer::resetInputs()
{
  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->name != "tick" &&
	( s->kind == SignalSymbol::Input ||
	  s->kind == SignalSymbol::Inputoutput ||
	  s->kind == SignalSymbol::Sensor )) {
      o << "  ";
      assert(contains(presenceVar, s));
      o << presenceVar[s] << " = 0;\n";
    }
  }  
}
#line 1201 "CPrinter.nw"
void Printer::ioDefinitions()
{
  // Print input signal function definitions

  assert(m.signals);
  for ( SymbolTable::const_iterator i = m.signals->begin() ;
	i != m.signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);

    if (s->name != "tick" &&
	( s->kind == SignalSymbol::Input ||
	  s->kind == SignalSymbol::Inputoutput)) {
      assert(contains(presenceVar, s));
      assert(m.symbol);
      o << "void " << m.symbol->name << "_I_" << s->name << "(";
      if (s->type) {
	o << s->type->name << " _v";
      } else {
	o << "void";
      }
      o << ") {\n"
	"  " << presenceVar[s] << " = 1;\n";
      if (s->type) {
	assert(contains(valueVar, s));
	if (s->type->name == "string") {
	  o << "  strcpy(" << valueVar[s] << ", _v);\n";
	} else {
	  // FIXME: This doesn't work with combine
	  o << "  " << valueVar[s] << " = _v;\n";
	}
      }
      o << "}\n";
    }
  }
}
#line 1258 "CPrinter.nw"
void Printer::printStructuredCode(GRCNode *exit_node, unsigned int indent)
{
  assert(exit_node);
  assert(exit_node->successors.size() == 0);

  // Number the nodes in a depth-first order

  nodes.clear();
  nodeNumber.clear();

  dfsVisit(exit_node);

  
#line 1320 "CPrinter.nw"
ridom.clear();

// Compute immediate dominators on the reverse graph

ridom[exit_node] = exit_node;
bool changed;
do {
  changed = false;
  for ( vector<GRCNode*>::reverse_iterator b = nodes.rbegin() + 1;
	b != nodes.rend() ; b++ ) {
    GRCNode *new_idom = NULL;
    for ( vector<GRCNode*>::iterator p = (*b)->successors.begin() ;
	  p != (*b)->successors.end() ; p++ ) {
      if ( ridom.find(*p) != ridom.end() ) {
	if ( new_idom == NULL )
	  new_idom = *p;
	else {
	  // Intersect
	  GRCNode *b1 = *p;
	  GRCNode *b2 = new_idom;
	  while (b1 != b2) {
	    while (nodeNumber[b1] < nodeNumber[b2]) b1 = ridom[b1];
	    while (nodeNumber[b2] < nodeNumber[b1]) b2 = ridom[b2];
	  }
	  new_idom = b1;
	}
      }
    }
    if ( ridom[*b] != new_idom ) {
      ridom[*b] = new_idom;
      // std::cerr << "idom of " << cfgmap[*b] << " is " << cfgmap[new_idom] << '\n';
      changed = true;
    }
  }
} while (changed);

#line 1272 "CPrinter.nw"
  GRCNode *entry_node = nodes.front();

  statementFor.clear();
  CStatement *root = synthesize(entry_node, nodes.back(), false);

  CStatement::printer = this;
  for ( ; root ; root = root->next ) {
    // std::cerr << "Printing node " << cfgmap[root->node] << "\n";
    root->print(indent);
  }

  delete root;
}
#line 1297 "CPrinter.nw"
void Printer::dfsVisit(GRCNode *n)
{
  if (!n || nodeNumber.find(n) != nodeNumber.end()) return;
  nodeNumber[n] = -1; // Mark as being visited, but do not know number yet

  for (vector<GRCNode*>::const_iterator i = n->predecessors.begin() ;
       i != n->predecessors.end() ; i++)
    dfsVisit(*i);

  nodeNumber[n] = nodes.size();
  nodes.push_back(n);

  // std::cerr << "Assigned node " << cfgmap[n] << " = " << nodeNumber[n] << '\n';
}
#line 1412 "CPrinter.nw"
Printer *CStatement::printer = 0;
#line 1416 "CPrinter.nw"
void CStatement::indent(unsigned int n)
{
  for (unsigned int i = 0 ; i < n ; i++) printer->o << "  ";
}
#line 1423 "CPrinter.nw"
void CStatement::begin(unsigned int i)
{
  if (!label.empty()) {
    indent(i > 0 ? i - 1 : i);
    printer->o << label << ":\n";
  }
  indent(i);
}
#line 1435 "CPrinter.nw"
void CStatement::print(unsigned int i)
{
  begin(i);
  printer->printExpr(node);
  printer->o << ";\n";
}
#line 1444 "CPrinter.nw"
void CIfElse::print(unsigned int i)
{
  begin(i);
  printer->o << "if (";
  printer->printExpr(node);
  printer->o << ") {\n";
  for ( CStatement *st = thenSt ; st ; st = st->next ) st->print(i+1);
  indent(i);
  printer->o << "}";
  if ( elseSt ) {
    printer->o << " else {\n";
    for ( CStatement *st = elseSt ; st ; st = st->next ) st->print(i+1);
    indent(i);
    printer->o << "}\n";
  } else {
    printer->o << "\n";
  }
}
#line 1465 "CPrinter.nw"
void CGoto::print(unsigned int i)
{
  begin(i);
  printer->o << "goto " << label << ";\n";
}
#line 1473 "CPrinter.nw"
void CBreak::print(unsigned int i)
{
  begin(i);
  printer->o << "break;\n";
}
#line 1481 "CPrinter.nw"
void CSwitch::print(unsigned int i)
{
  begin(i);
  printer->o << "switch (";
  printer->printExpr(node);
  printer->o << ") {\n";
  for ( CStatement *st = body ; st ; st = st->next ) st->print(i+1);
  indent(i);
  printer->o << "default: break;\n";
  indent(i);
  printer->o << "}\n";
}
#line 1496 "CPrinter.nw"
void CCase::print(unsigned int i)
{
  indent(i > 0 ? i - 1 : 0);
  printer->o << "case " << label << ":\n";
  assert(body);
  for ( CStatement *st = body ; st ; st = st->next ) st->print(i);
}
#line 1512 "CPrinter.nw"
CStatement *Printer::synthesize(GRCNode *node, GRCNode *final, bool needBreak)
{
  assert(node);
  assert(final);

  //std::cerr << "/* initial synthesize(" << cfgmap[node] << ", " << cfgmap[final] << ", " << (needBreak ? 't' : 'f') << ") */\n ";

#if 0
  std::cerr << "successors: ";
  for ( vector<GRCNode*>::const_iterator i = node->successors.begin() ;
	i != node->successors.end() ; i++ ) {
    if (*i) std::cerr << cfgmap[*i] << ' ';
    else std::cerr << "NULL ";
  }
  std::cerr << std::endl;
#endif

  if ( node == final )
    return needBreak ? new CBreak() : 0;

  if ( statementFor.find(node) != statementFor.end() ) {
    CStatement *target = statementFor[node];
    if (target->label.empty()) {
      char buf[20];
      // sprintf(buf, "L%d", nextLabel++);
      assert(cfgmap.find(node) != cfgmap.end());
      sprintf(buf, "N%d", cfgmap[node]);
      target->label = buf;
    }
    return new CGoto(target->label);
  }

  assert(ridom.find(node) != ridom.end());

  GRCNode *next =
    (node->successors.size() > 1) ? ridom[node] : node->successors.front();

  CStatement *nextStatement = next ? synthesize(next, final, needBreak) : 0;

//  std::cerr << "/* continue synthesize(" << cfgmap[node] << ", " << cfgmap[final] << ", " << (needBreak ? 't' : 'f') << ") */\n ";  

  CStatement *result = NULL;

  switch (node->successors.size()) {
  case 0:
  case 1:
    // std::cerr << "simple statement\n";
    result = new CStatement(node);
    break;
  case 2:
    {
      if (node->successors.front() && node->successors.back()) {
	//std::cerr << "if-then-else statement\n";
	CStatement *elsePart =
	  synthesize(node->successors.front(), next, false);
	CStatement *thenPart =
	  synthesize(node->successors.back(), next, false);
	result = new CIfElse(node, thenPart, elsePart);
      } else {
	// std::cerr << "Identified a node with two successors that became a simple statement" << std::endl;
	result = new CStatement(new Nop());
      }
    }
    break;
  default:
    // Three or more successors: a switch statement
    {
      // std::cerr << "switch statement\n";
      unsigned int nonzero_successors = 0;
      for ( vector<GRCNode*>::reverse_iterator i = node->successors.rbegin() ;
	    i != node->successors.rend() ; i++ )
	if (*i) ++nonzero_successors;
      if (nonzero_successors > 1) {
	CStatement *body = NULL;
	bool useSyncNumbering = dynamic_cast<Sync*>(node) != NULL;
	for ( vector<GRCNode*>::reverse_iterator i =
		node->successors.rbegin() ;
	      i != node->successors.rend() ; i++ )
	  if (*i) {	
	    CStatement *caseBody = synthesize(*i, next, true);
	    int caseLabel = node->successors.rend() - i - 1;
	    if (useSyncNumbering) caseLabel = (1 << caseLabel) - 1;
	    CStatement *thisCase = new CCase(caseLabel, caseBody);
	    thisCase->next = body;
	    body = thisCase;
	  }
	result = new CSwitch(node, body);
      } else {
	// std::cerr << "Identified a node with multiple successors that became a simple statement" << std::endl;
	result = new CStatement(new Nop());
      }
    }
    break;
  }

  assert(result);
  assert(result->next == NULL);
  result->next = nextStatement;

  // std::cerr << "done with " << cfgmap[node] << "\n";
  assert(result);
  statementFor[node] = result;
  if (labelFor.find(node) != labelFor.end())
    result->label = labelFor[node];
  return result;
}
#line 1670 "CPrinter.nw"
}
