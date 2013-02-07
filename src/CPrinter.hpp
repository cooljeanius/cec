#line 1635 "CPrinter.nw"
#ifndef _CPRINTER_HPP
#  define _CPRINTER_HPP

#  define USE_STRUCTS_FOR_SIGNALS
/* #  define USE_STRUCTS_FOR_STATES */

#  include "AST.hpp"
#  include <iostream>
#  include <cassert>
#  include <set>
#  include <vector>
#  include <map>

namespace CPrinter {
  using namespace AST;
  using std::set;
  using std::vector;
  using std::map;

  
#line 1623 "CPrinter.nw"
template <class T> bool contains(set<T> &s, T o) {
   return s.find(o) != s.end();
}

template <class T, class U> bool contains(map<T, U> &m, T o) {
  return m.find(o) != m.end();
}

#line 1656 "CPrinter.nw"
  
#line 1360 "CPrinter.nw"
class Printer;

struct CStatement {
  static Printer *printer;

  GRCNode *node;
  CStatement *next;
  string label;

  CStatement(GRCNode *node) : node(node), next(0) {}
  virtual ~CStatement() { delete next; }
  virtual void print(unsigned int = 0);
  void indent(unsigned int);
  void begin(unsigned int);
};

struct CIfElse : CStatement {
  CStatement *thenSt;
  CStatement *elseSt;
  CIfElse(GRCNode *node, CStatement *thenSt, CStatement *elseSt)
    : CStatement(node), thenSt(thenSt), elseSt(elseSt) {}
  virtual ~CIfElse() { delete thenSt; delete elseSt; }
  void print(unsigned int = 0);
};

struct CGoto : CStatement {
  string label;
  CGoto(string label) : CStatement(NULL), label(label) {}
  void print(unsigned int = 0);
};

struct CBreak : CStatement {
  CBreak() : CStatement(NULL) {}
  void print(unsigned int = 0);
};

struct CSwitch : CStatement {
  CStatement *body;
  CSwitch(GRCNode *node, CStatement *body) : CStatement(node), body(body) {}
  virtual ~CSwitch() { delete body; }
  void print(unsigned int = 0);
};

struct CCase : CStatement {
  int label;
  CStatement *body;
  CCase(int label, CStatement *body) : CStatement(NULL), label(label), body(body) {}
  void print(unsigned int = 0);
};

#line 1658 "CPrinter.nw"
  
#line 22 "CPrinter.nw"
class Printer : public Visitor {
  typedef map<GRCNode *, int> CFGmap;
  typedef map<STNode *, int> STmap;
  CFGmap cfgmap;
  STmap stmap;

  vector<GRCNode*> nodes;
  CFGmap nodeNumber;
  map<GRCNode*, GRCNode*> ridom;

  map<GRCNode *, CStatement*> statementFor;

  static int nextLabel;
public:
  std::ostream &o;
  Module &m;
  bool do_threevalued;
  GRCgraph *g;

  map<GRCNode *, string> labelFor;

  set<string> identifiers; // All C identifiers for avoiding name collisions

  // C identifiers for various objects

  typedef map<Counter *, string> CounterNames;
  CounterNames counterVar;

  typedef map<SignalSymbol *, string> SignalNames;
  SignalNames presenceVar;
  SignalNames valueVar;

  typedef map<STexcl *, string> StateNames;
  StateNames stateVar;

  typedef map<Sync *, string> TerminationNames;
  TerminationNames terminationVar;

  typedef map<VariableSymbol *, string> VariableNames;
  VariableNames variableVar;

  Printer(std::ostream &, Module &, bool);
  virtual ~Printer() {}

  
#line 105 "CPrinter.nw"
string uniqueID(string);
#line 135 "CPrinter.nw"
void printExpr(ASTNode *n) { n->welcome(*this); }
#line 144 "CPrinter.nw"
Status visit(Test &t) { printExpr(t.predicate); return Status(); }
Status visit(Action &a) { printExpr(a.body); return Status(); }
#line 153 "CPrinter.nw"
Status visit(EnterGRC&) { o << "1 /* EnterGRC */"; return Status(); }
Status visit(ExitGRC&) { o << "/* ExitGRC */"; return Status(); }
Status visit(STSuspend&) { o << "/* STSuspend */"; return Status(); }
#line 164 "CPrinter.nw"
Status visit(Nop& n) { o << n.body; return Status(); }
#line 172 "CPrinter.nw"
Status visit(Switch &s) {
  STexcl *e = dynamic_cast<STexcl*>(s.st);
  assert(e);
  assert(contains(stateVar, e));
  o << stateVar[e];
  return Status();
}
#line 189 "CPrinter.nw"
Status visit(Enter &);
#line 246 "CPrinter.nw"
Status visit(Terminate &);
#line 283 "CPrinter.nw"
Status visit(Sync &s) {
  if ( contains(terminationVar, &s) )
    o << '~' << terminationVar[&s];
  return Status();
}
#line 295 "CPrinter.nw"
Status visit(Fork &f) {
  if (f.sync && contains(terminationVar, f.sync))
    o << terminationVar[f.sync] << " = -1";
  return Status();
}
#line 312 "CPrinter.nw"
Status visit(Emit &);
Status visit(Exit &);
#line 364 "CPrinter.nw"
Status visit(DefineSignal &d)
{
  assert(contains(presenceVar, d.signal));
  o << presenceVar[d.signal] << " = 0";
  if (d.signal->initializer && d.is_surface) {
     o << ", ";
     assert(contains(valueVar, d.signal));
     if (d.signal->initializer->type->name == "string") {
      o << "strcpy(" << valueVar[d.signal] << ", ";
      printExpr(d.signal->initializer);
      o << ");";
     } else {
      o << valueVar[d.signal] << " = ";
      printExpr(d.signal->initializer);
     }
  }
  o << ';';
  return Status();
}
#line 388 "CPrinter.nw"
Status visit(Assign &a) {
  assert(a.variable->type);
  assert(contains(variableVar, a.variable));

  if (a.variable->type->name == "string") {
    // Use strcpy for strings
    o << "strcpy(" << variableVar[a.variable] << ", ";
    printExpr(a.value);
    o << ")";
  } else if ( dynamic_cast<BuiltinTypeSymbol*>(a.variable->type) ) {
    // Use assignment for other built-in types
    o << variableVar[a.variable] << " = ";
    printExpr(a.value);
  } else {
    // Call _<typename>(&lvalue, rvalue) for user-defined types
    o << '_' << a.variable->type->name
      << "(&" << variableVar[a.variable] << ", ";
    printExpr(a.value);
    o << ')';
  }
  return Status();
}
#line 417 "CPrinter.nw"
Status visit(StartCounter &);
#line 437 "CPrinter.nw"
Status visit(CheckCounter &);
#line 464 "CPrinter.nw"
Status visit(LoadSignalExpression &e) {
  assert(contains(presenceVar, e.signal));
  o << presenceVar[e.signal];
  return Status();
}
#line 483 "CPrinter.nw"
Status visit(LoadSignalValueExpression &e) {
  assert(e.signal);
  assert(contains(valueVar, e.signal));
  assert(contains(presenceVar, e.signal));
  if (e.signal->kind == SignalSymbol::Sensor) {
    o << "( " << presenceVar[e.signal] << " ? " << valueVar[e.signal]
      << " : (" << presenceVar[e.signal] << " = 1,"
      << valueVar[e.signal] << " = "
      << m.symbol->name << "_S_" << e.signal->name << "()) )";
  } else {
    o << valueVar[e.signal];
  }
  return Status();
}
#line 502 "CPrinter.nw"
Status visit(LoadVariableExpression &e) {
  assert(contains(variableVar, e.variable));
  o << variableVar[e.variable];
  return Status();
}
#line 546 "CPrinter.nw"
Status visit(UnaryOp &);
Status visit(BinaryOp &);
#line 553 "CPrinter.nw"
Status visit(Literal &);
#line 581 "CPrinter.nw"
Status visit(FunctionCall &);
#line 630 "CPrinter.nw"
Status visit(ProcedureCall &);
#line 663 "CPrinter.nw"
virtual void printInclude(string);
#line 711 "CPrinter.nw"
virtual void printDeclarations(string);
#line 1138 "CPrinter.nw"
virtual void outputFunctions();
#line 1169 "CPrinter.nw"
virtual void resetInputs();
#line 1197 "CPrinter.nw"
virtual void ioDefinitions();
#line 1254 "CPrinter.nw"
void printStructuredCode(GRCNode *, unsigned int = 0);
#line 1293 "CPrinter.nw"
void dfsVisit(GRCNode*);
#line 1508 "CPrinter.nw"
CStatement *synthesize(GRCNode*, GRCNode*, bool);
#line 67 "CPrinter.nw"
};
#line 1659 "CPrinter.nw"
}

#endif
