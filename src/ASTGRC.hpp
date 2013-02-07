#line 3197 "ASTGRC.nw"
#ifndef _ASTGRC_HPP
#  define _ASTGRC_HPP

#  include "AST.hpp"
#  include <assert.h>
#  include <stack>
#  include <map>
#  include <set>
#  include <stdio.h>

namespace ASTGRC {
  using namespace IR;
  using namespace AST;
  using std::map;
  using std::set;

  class GrcSynth;

  
#line 29 "ASTGRC.nw"
class CompletionCodes : public Visitor {
  int maxOverModule; // Maximum code for this
  map<Abort*, int> codeOfAbort; // Code for each weak abort
  map<SignalSymbol*, int> codeOfTrapSymbol; // Code for each trap symbol
  map<Trap*, int> codeOfTrap; // Code for each trap statement
public:

  void alsoMax(AST::ASTNode *n, int &m) {
     int max = recurse(n);
     if (max > m) m = max;
  }

  int recurse(AST::ASTNode *n) {
     if (n) return n->welcome(*this).i;
     else return 0;
  }

  
#line 53 "ASTGRC.nw"
CompletionCodes(Module *m)
{
  assert(m);
  assert(m->body);
  maxOverModule = recurse(m->body);
  if (maxOverModule <= 1) maxOverModule = 1;
}

virtual ~CompletionCodes() {}
#line 67 "ASTGRC.nw"
  int max() const { return maxOverModule; }

  int operator [] (Abort *a) {
    assert(codeOfAbort.find(a) != codeOfAbort.end());
    return codeOfAbort[a];
  }

  int operator [] (SignalSymbol *ts) {
    assert(codeOfTrapSymbol.find(ts) != codeOfTrapSymbol.end());
    return codeOfTrapSymbol[ts];
  }

  int operator [] (Trap *t) {
    assert(codeOfTrap.find(t) != codeOfTrap.end());
    return codeOfTrap[t];
  }
#line 88 "ASTGRC.nw"
Status visit(Signal &s) { return recurse(s.body); }
Status visit(Var &s) { return recurse(s.body); }
Status visit(Loop &s) { return recurse(s.body); }
Status visit(Repeat &s) { return recurse(s.body); }
Status visit(Every &s) { return recurse(s.body); }
Status visit(Suspend &s) { return recurse(s.body); }
Status visit(PredicatedStatement &s) { return recurse(s.body); }
#line 98 "ASTGRC.nw"
Status visit(StatementList &l) {
  int max = 1;
  for (vector<Statement*>::iterator i = l.statements.begin() ;
       i != l.statements.end() ; i++ ) alsoMax(*i, max);
  return max;
}
#line 107 "ASTGRC.nw"
Status visit(ParallelStatementList &l) {
  int max = 1;
  for (vector<Statement*>::iterator i = l.threads.begin() ;
       i != l.threads.end() ; i++ ) alsoMax(*i, max);
  return max;
}
#line 116 "ASTGRC.nw"
Status visit(IfThenElse& n) {
  int max = 1;
  alsoMax(n.then_part, max);
  alsoMax(n.else_part, max);
  return max;
}
#line 129 "ASTGRC.nw"
Status visit(Emit&) { return Status(0); }
Status visit(Assign&) { return Status(0); }
Status visit(ProcedureCall&) { return Status(0); }
Status visit(TaskCall&) { return Status(0); }
Status visit(Exec&) { return Status(0); }
Status visit(Exit&) { return Status(0); }
Status visit(Run&) { return Status(0); }
Status visit(Pause&) { return Status(0); }
#line 145 "ASTGRC.nw"
Status visit(Abort &s) {
  int max = 1;
  alsoMax(s.body, max);
  for (vector<PredicatedStatement*>::iterator i = s.cases.begin() ;
       i != s.cases.end() ; i++ ) alsoMax(*i, max);
  if (s.is_weak) {
    int code = max + 1;
    codeOfAbort[&s] = code;
    assert(code >= 2);
    max += 1 + s.cases.size();
  }
  return max;
}
#line 165 "ASTGRC.nw"
Status visit(Trap &s) {
  int max = 1;
  alsoMax(s.body, max);

  // FIXME: is this the right order?  Should the predicates be
  // considered before or after the code is assigned?
 
  for (vector<PredicatedStatement*>::iterator i = s.handlers.begin() ;
       i != s.handlers.end() ; i++ ) alsoMax(*i, max);

  max++; // Allocate an exit level for this trap statement

  codeOfTrap[&s] = max;

  assert(s.symbols);
  for (SymbolTable::const_iterator i = s.symbols->begin() ; i !=
       s.symbols->end() ; i++) {
    SignalSymbol *ts = dynamic_cast<SignalSymbol*>(*i);
    assert(ts);
    assert(ts->kind == SignalSymbol::Trap);
    codeOfTrapSymbol[ts] = max;
  }

  return max;
}
#line 47 "ASTGRC.nw"
};

#line 3217 "ASTGRC.nw"
  
#line 211 "ASTGRC.nw"
class Cloner : public Visitor {
public:
  template <class T> T* operator() (T* n) {
    if (!n) return NULL;
    T* result = dynamic_cast<T*>(n->welcome(*this).n);
    assert(result);
    return result;
  }

  
#line 350 "ASTGRC.nw"
SignalSymbol *cloneLocalSignal(SignalSymbol *s, SymbolTable *st) {
  assert(s);
  assert(newsig.find(s) == newsig.end()); // Should not already be there
  assert(st);

  string name = s->name;
  int next = 1;
  while (st->contains(name)) {
    char buf[10];
    sprintf(buf, "%d", next++);
    name = s->name + '_' + buf;
  }
  SignalSymbol::kinds kind =
    (s->kind == SignalSymbol::Trap) ? SignalSymbol::Trap : SignalSymbol::Local;
  SignalSymbol *reincarnation = 0;
  if (master_signal.find(s) != master_signal.end()) {
    reincarnation = master_signal[s];
    assert(reincarnation);
  }
  SignalSymbol *result =
    new SignalSymbol(name, s->type, kind, clone(s->combine),
                     clone(s->initializer), reincarnation);
  if (!reincarnation)
    master_signal[s] = result;
  assert(master_signal.find(s) != master_signal.end());
  // std::cerr << "cloning " << s->name << std::endl;
  st->enter(result);
  newsig[s] = result;
  return result;
}
#line 385 "ASTGRC.nw"
void sameSig(SignalSymbol *s) {
  assert(s);
  assert(newsig.find(s) == newsig.end());
  newsig[s] = s;
}
#line 395 "ASTGRC.nw"
void clearSig(SignalSymbol *orig) {
  assert(orig);
  map<SignalSymbol*, SignalSymbol*>::iterator i = newsig.find(orig);
  assert(i != newsig.end());
  newsig.erase(i);
}
#line 424 "ASTGRC.nw"
map<VariableSymbol*, VariableSymbol*> newvar;
#line 428 "ASTGRC.nw"
VariableSymbol *hoistLocalVariable(VariableSymbol *s, SymbolTable *st) {
  assert(s);
  assert(newvar.find(s) == newvar.end()); // should not be remapped yet
  assert(st);

  // Add a suffix to the name to make it unique, if necessary

  string name = s->name;
  int next = 1;
  while (st->contains(name)) {
    char buf[10];
    sprintf(buf, "%d", next++);
    name = s->name + '_' + buf;
  }

  VariableSymbol *result =
    new VariableSymbol(name, s->type, clone(s->initializer));
  st->enter(result);
  newvar[s] = result;
  return result;
}
#line 452 "ASTGRC.nw"
void sameVar(VariableSymbol *s) {
  assert(s);
  assert(newvar.find(s) == newvar.end());
  newvar[s] = s;
}

#line 222 "ASTGRC.nw"
  virtual ~Cloner() {}

protected:
  template <class T> T* clone(T* n) { return (*this)(n); }

  /* For each signal symbol in the AST's symbol tables,
     the master signal symbol in the expanded graph.  Used to
     set up the reincarnation field of the cloned signals. */
  map<SignalSymbol *, SignalSymbol *> master_signal;

  
#line 239 "ASTGRC.nw"
Status visit(Emit &s) {
  return new Emit(clone(s.signal), clone(s.value));
}

Status visit(Exit &s) {
  return new Exit(clone(s.trap), clone(s.value));
}

Status visit(Assign &s) {
  return new Assign(clone(s.variable), clone(s.value));
}
#line 255 "ASTGRC.nw"
Status visit(Literal &s) { return new Literal(s.value, s.type); }

Status visit(LoadVariableExpression &s) {
  return new LoadVariableExpression(clone(s.variable));
}

Status visit(LoadSignalExpression &s) {
  return new LoadSignalExpression(s.type, clone(s.signal));
}

Status visit(LoadSignalValueExpression &s) {
  return new LoadSignalValueExpression(clone(s.signal));
}
#line 273 "ASTGRC.nw"
Status visit(UnaryOp &s) {
  return new UnaryOp(s.type, s.op, clone(s.source));
}

Status visit(BinaryOp &s) {
  return new BinaryOp(s.type, s.op, clone(s.source1), clone(s.source2));
}
#line 285 "ASTGRC.nw"
Status visit(FunctionCall &s) {
  FunctionCall *c = new FunctionCall(clone(s.callee));
  for (vector<Expression*>::const_iterator i = s.arguments.begin() ;
       i != s.arguments.end() ; i++) {
    assert(*i);
    c->arguments.push_back(clone(*i));
  }
  return c;
}
#line 299 "ASTGRC.nw"
Status visit(ProcedureCall &s) {
  ProcedureCall *c = new ProcedureCall(clone(s.procedure));
  for (vector<VariableSymbol*>::const_iterator i = s.reference_args.begin() ;
       i != s.reference_args.end() ; i++) {
    assert(*i);
    c->reference_args.push_back(clone(*i));
  }
  for (vector<Expression*>::const_iterator i = s.value_args.begin() ;
       i != s.value_args.end() ; i++) {
    assert(*i);
    c->value_args.push_back(clone(*i));
  }

  return c;
}
#line 319 "ASTGRC.nw"
Status visit(CheckCounter &s) {
  return new CheckCounter(s.type, s.counter, clone(s.predicate));
}
#line 329 "ASTGRC.nw"
Status visit(ConstantSymbol &s) { return &s; }
Status visit(BuiltinConstantSymbol &s) { return &s; }
Status visit(BuiltinSignalSymbol &s) { return &s; }
Status visit(FunctionSymbol &s) { return &s; }
Status visit(ProcedureSymbol &s) { return &s; }
Status visit(BuiltinFunctionSymbol &s) { return &s; }
Status visit(Counter &s) { return &s; }
#line 343 "ASTGRC.nw"
map<SignalSymbol*, SignalSymbol*> newsig;
#line 412 "ASTGRC.nw"
Status visit(SignalSymbol &s) {
  assert(newsig.find(&s) != newsig.end()); // should be there
  return newsig[&s];
}
#line 462 "ASTGRC.nw"
Status visit(VariableSymbol &s) {
  assert(newvar.find(&s) != newvar.end()); // should be there
  return newvar[&s];
}
#line 233 "ASTGRC.nw"
};

#line 3219 "ASTGRC.nw"
  
#line 511 "ASTGRC.nw"
struct Context {
  int size;
  std::stack<GRCNode**> continuations;

  Context(int sz) : size(sz) {
    assert(sz >= 2); // Must at least have termination at levels 0 and 1
    continuations.push(new GRCNode*[size]);
    for (int i = 0 ; i < size ; i++ ) continuations.top()[i] = 0;
  }
  ~Context() {}

  void push(Context &c) {
    GRCNode **parent = c.continuations.top();
    continuations.push(new GRCNode*[size]);
    GRCNode **child = continuations.top();
    for ( int i = 0 ; i < size ; i++ ) child[i] = parent[i];
  }

  void push() { push(*this); }

  void pop() {
    delete [] continuations.top();
    continuations.pop();
  }

  GRCNode *& operator ()(int k) {
    assert(k >= 0);
    assert(k < size);
    return continuations.top()[k];
  }
};
#line 3220 "ASTGRC.nw"
  
#line 710 "ASTGRC.nw"
class GrcWalker : public Visitor {
protected:
  Context &context;
  GrcSynth &environment;
  Cloner &clone;
public:
  GrcWalker(Context &, GrcSynth &);

  GRCNode *synthesize(ASTNode *n) {
    assert(n);
    n->welcome(*this);
    assert(context(0));
    return context(0);
  }

  GRCNode *recurse(ASTNode *n) {
    context.push();
    GRCNode *nn = synthesize(n);
    context.pop();
    return nn;
  }

  static GRCNode* push_onto(GRCNode *& b, GRCNode* n) {
    *n >> b;
    b = n;
    return b;
  }

  STNode *stnode(const ASTNode &);
};
#line 3221 "ASTGRC.nw"
  
#line 785 "ASTGRC.nw"
class Surface : public GrcWalker {
public:
  Surface(Context &c, GrcSynth &e) : GrcWalker(c, e) {}
  
#line 827 "ASTGRC.nw"
Status visit(Pause &);
#line 866 "ASTGRC.nw"
Status visit(Exit &);
#line 893 "ASTGRC.nw"
Status visit(Emit &s) {
  push_onto(context(0), new Action(clone(&s)));
  return Status();
}
#line 916 "ASTGRC.nw"
Status visit(Assign &s) {
  Action *a = new Action(clone(&s));
  *a >> context(0);
  context(0) = a;
  return Status();
}
#line 951 "ASTGRC.nw"
Status visit(IfThenElse &);
#line 1012 "ASTGRC.nw"
Status visit(StatementList &);
#line 1077 "ASTGRC.nw"
Status visit(Loop &);
#line 1127 "ASTGRC.nw"
Status visit(Every &);
#line 1227 "ASTGRC.nw"
Status visit(Repeat &);
#line 1408 "ASTGRC.nw"
Status visit(Suspend &);
#line 1611 "ASTGRC.nw"
Status visit(Abort &);
#line 1648 "ASTGRC.nw"
Status visit(ParallelStatementList &);
#line 1951 "ASTGRC.nw"
Status visit(Trap &);
#line 2020 "ASTGRC.nw"
Status visit(Signal &);
#line 2076 "ASTGRC.nw"
Status visit(Var &);
#line 2092 "ASTGRC.nw"
Status visit(Exec &) { return Status(); }
#line 2106 "ASTGRC.nw"
Status visit(ProcedureCall &s) {
  push_onto(context(0), new Action(clone(&s)));
  return Status();
}
#line 789 "ASTGRC.nw"
};
#line 3222 "ASTGRC.nw"
  
#line 793 "ASTGRC.nw"
class Depth : public GrcWalker {
public:
  Depth(Context &c, GrcSynth &e) : GrcWalker(c, e) {}
  
#line 843 "ASTGRC.nw"
Status visit(Pause &);
#line 879 "ASTGRC.nw"
Status visit(Exit &) { return Status(); }
#line 900 "ASTGRC.nw"
Status visit(Emit &) { return Status(); }
#line 925 "ASTGRC.nw"
Status visit(Assign &) { return Status(); }
#line 972 "ASTGRC.nw"
Status visit(IfThenElse &);
#line 1030 "ASTGRC.nw"
Status visit(StatementList &);
#line 1090 "ASTGRC.nw"
Status visit(Loop &);
#line 1164 "ASTGRC.nw"
Status visit(Every &);
#line 1243 "ASTGRC.nw"
Status visit(Repeat &);
#line 1412 "ASTGRC.nw"
Status visit(Suspend &);
#line 1615 "ASTGRC.nw"
Status visit(Abort &);
#line 1699 "ASTGRC.nw"
Status visit(ParallelStatementList &);
#line 1955 "ASTGRC.nw"
Status visit(Trap &);
#line 2024 "ASTGRC.nw"
Status visit(Signal &);
#line 2080 "ASTGRC.nw"
Status visit(Var &);
#line 2096 "ASTGRC.nw"
Status visit(Exec &) { return Status(); }
#line 2113 "ASTGRC.nw"
Status visit(ProcedureCall &) { return Status(); }
#line 797 "ASTGRC.nw"
};
#line 3223 "ASTGRC.nw"
  
#line 758 "ASTGRC.nw"
class SelTree : public Visitor {
protected:
  GrcSynth &environment;
public:
  SelTree(GrcSynth &e): environment(e) {}

  STNode *synthesize(ASTNode *n) {
    assert(n);
    STNode *result = dynamic_cast<STNode*>(n->welcome(*this).n);
    assert(result);
    return result;
  }

  void setNode(const ASTNode &, STNode *);
    
  
#line 813 "ASTGRC.nw"
Status visit(Pause &);
#line 860 "ASTGRC.nw"
Status visit(Exit &s) {
  return Status(new STref());
}
#line 885 "ASTGRC.nw"
Status visit(Emit &) {
  return Status(new STref());
}
#line 910 "ASTGRC.nw"
Status visit(Assign &s) {
  return Status(new STref());
}
#line 933 "ASTGRC.nw"
Status visit(IfThenElse &);
#line 992 "ASTGRC.nw"
Status visit(StatementList &s);
#line 1064 "ASTGRC.nw"
Status visit(Loop &s);
#line 1108 "ASTGRC.nw"
Status visit(Every &);
#line 1214 "ASTGRC.nw"
Status visit(Repeat &);
#line 1404 "ASTGRC.nw"
Status visit(Suspend &);
#line 1607 "ASTGRC.nw"
Status visit(Abort &);
#line 1623 "ASTGRC.nw"
Status visit(ParallelStatementList &);
#line 1947 "ASTGRC.nw"
Status visit(Trap &);
#line 2016 "ASTGRC.nw"
Status visit(Signal &);
#line 2072 "ASTGRC.nw"
Status visit(Var &);
#line 2088 "ASTGRC.nw"
Status visit(Exec &) { return Status(new STref()); }
#line 2102 "ASTGRC.nw"
Status visit(ProcedureCall &) { return Status(new STref()); }
#line 774 "ASTGRC.nw"
};
#line 3224 "ASTGRC.nw"
  
#line 555 "ASTGRC.nw"
struct GrcSynth {
  Module *module;
  CompletionCodes &code;

  Cloner clone;

  Context surface_context;
  Context depth_context;

  Surface surface;
  Depth depth;
  SelTree seltree;

  map<const ASTNode*, STNode*> ast2st;

  BuiltinTypeSymbol *integer_type;
  BuiltinTypeSymbol *boolean_type;
  BuiltinConstantSymbol *true_symbol;

  
#line 582 "ASTGRC.nw"
GrcSynth(Module *, CompletionCodes &);
#line 623 "ASTGRC.nw"
GRCgraph *synthesize()
{
  assert(module->body);

  // Set up initial and terminal states in the selection tree

  STexcl *stroot = new STexcl();
  STleaf *boot = new STleaf();
  STleaf *finished = new STleaf();
  finished->setfinal();

  // Set up the root of the GRC

  EnterGRC *engrc = new EnterGRC();
  ExitGRC *exgrc = new ExitGRC();

  Enter *enfinished = new Enter(finished);
  Switch *top_switch = new Switch(stroot);

  *engrc >> exgrc >> top_switch;
  *enfinished >> exgrc;

  enfinished->st = finished;

  Terminate *term0 = new Terminate(0, 0);
  *term0 >> enfinished;
  Terminate *term1 = new Terminate(1, 0);
  *term1 >> exgrc;

  // Set up the context for the surface and the depth: point to term0 and 1

  surface_context(0) = depth_context(0) = term0;
  surface_context(1) = depth_context(1) = term1;

  // Build the selection tree and create the selection tree root

  STNode *synt_seltree = seltree.synthesize(module->body);
  *stroot >> finished >> synt_seltree >> boot;

  // Build the surface and the depth

  GRCNode *synt_surface = surface.synthesize(module->body);
  GRCNode *synt_depth = depth.synthesize(module->body);

  // Add DefineSignal statements for every output signal.  This clears
  // their presence and initializes their values if an initializer was given

  for (SymbolTable::const_iterator i = module->signals->begin() ;
       i != module->signals->end() ; i++) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    if (ss->kind == SignalSymbol::Output) {
      DefineSignal *ds = new DefineSignal(ss, true);
      *ds >> synt_surface;
      synt_surface = ds;
    }
  }

  *top_switch >> enfinished >> synt_depth >> synt_surface;

  GRCgraph *result = new GRCgraph(stroot, engrc);

  return result;
}
#line 575 "ASTGRC.nw"
};
#line 3225 "ASTGRC.nw"
  
#line 2122 "ASTGRC.nw"
class RecursiveSynth : public Visitor {
public:
  Module *module;
  CompletionCodes &code;

  Cloner clone;

  Context context;

  BuiltinTypeSymbol *boolean_type;
  BuiltinConstantSymbol *true_symbol;

  // The visitors set these pointers when they return

  STNode *stnode;
  GRCNode *surface;
  GRCNode *depth;

  void synthesize(ASTNode *n) {
    assert(n);

    stnode = NULL; // Assignments not strictly necessary: for safety
    surface = depth = NULL;

    n->welcome(*this);
    assert(stnode);
    assert(surface);
    assert(depth);
  }

  // Run n then b, replacing b
  static void run_before(GRCNode *& b, GRCNode *n) { *n >> b; b = n; }

  
#line 2161 "ASTGRC.nw"
RecursiveSynth(Module *, CompletionCodes &);
#line 2193 "ASTGRC.nw"
GRCgraph *synthesize();
#line 2259 "ASTGRC.nw"
map<GRCNode *, bool> visiting;
void visit(GRCNode *);
#line 2283 "ASTGRC.nw"
Status visit(Pause &);
#line 2303 "ASTGRC.nw"
Status visit(Exit &);
#line 2321 "ASTGRC.nw"
Status visit(Emit &);
#line 2338 "ASTGRC.nw"
Status visit(Assign &);
#line 2355 "ASTGRC.nw"
Status visit(IfThenElse &);
#line 2397 "ASTGRC.nw"
Status visit(StatementList &);
#line 2439 "ASTGRC.nw"
Status visit(Loop &);
#line 2466 "ASTGRC.nw"
Status visit(Every &);
#line 2530 "ASTGRC.nw"
Status visit(Repeat &);
#line 2562 "ASTGRC.nw"
Status visit(Suspend &);
#line 2637 "ASTGRC.nw"
Status visit(Abort &);
#line 2727 "ASTGRC.nw"
Status visit(ParallelStatementList &);
#line 2801 "ASTGRC.nw"
Status visit(Trap &);
#line 2879 "ASTGRC.nw"
Status visit(Signal &);
Status visit(Var &);
#line 2924 "ASTGRC.nw"
Status visit(ProcedureCall &);
#line 2943 "ASTGRC.nw"
Status visit(Exec &);
#line 2156 "ASTGRC.nw"
  virtual ~RecursiveSynth() {}
};

#line 3227 "ASTGRC.nw"
  
#line 2963 "ASTGRC.nw"
class Dependencies : public Visitor {
protected:
  set<GRCNode *> visited;
  GRCNode *current;

public:
  struct SignalNodes {
    set<GRCNode *> writers;
    set<GRCNode *> readers;
  };

  map<SignalSymbol *, SignalNodes> dependencies;

  
#line 3030 "ASTGRC.nw"
void dfs(GRCNode *);
#line 3047 "ASTGRC.nw"
Status visit(Action &);
#line 3051 "ASTGRC.nw"
Status visit(Emit &e) {
  dependencies[e.signal].writers.insert(current);
  if (e.value) e.value->welcome(*this);
  return Status();
}
#line 3059 "ASTGRC.nw"
Status visit(Exit &e) {
  dependencies[e.trap].writers.insert(current);
  if (e.value) e.value->welcome(*this);
  return Status();
}
#line 3067 "ASTGRC.nw"
Status visit(Assign &a) {
  a.value->welcome(*this);
  return Status();
}
#line 3074 "ASTGRC.nw"
Status visit(ProcedureCall &c) {
  for ( vector<Expression*>::const_iterator i = c.value_args.begin() ;
        i != c.value_args.end() ; i++ )
    (*i)->welcome(*this);
  return Status();
}
#line 3085 "ASTGRC.nw"
Status visit(Pause &) { return Status(); }
Status visit(StartCounter &) { return Status(); }
#line 3094 "ASTGRC.nw"
Status visit(DefineSignal &d) {
  dependencies[d.signal].writers.insert(current);
  return Status();
}
#line 3105 "ASTGRC.nw"
Status visit(Test &t) { t.predicate->welcome(*this); return Status(); }
#line 3111 "ASTGRC.nw"
Status visit(LoadSignalExpression &e) {
  dependencies[e.signal].readers.insert(current);
  return Status();
}

Status visit(LoadSignalValueExpression &e) {
  dependencies[e.signal].readers.insert(current);
  return Status();
}

Status visit(BinaryOp &e) {
  e.source1->welcome(*this);
  e.source2->welcome(*this);
  return Status();
}

Status visit(UnaryOp &e) {
  e.source->welcome(*this);
  return Status();
}

Status visit(CheckCounter &e) {
  e.predicate->welcome(*this);
  return Status();
}

Status visit(Delay &d) {
  d.predicate->welcome(*this);
  return Status();
}

Status visit(FunctionCall &c) {
    for ( vector<Expression*>::const_iterator i = c.arguments.begin() ;
        i != c.arguments.end() ; i++ )
    (*i)->welcome(*this);
  return Status();
}
#line 3153 "ASTGRC.nw"
Status visit(Literal &) { return Status(); }
Status visit(LoadVariableExpression &) { return Status(); }
#line 3176 "ASTGRC.nw"
Status visit(Sync &);
#line 3184 "ASTGRC.nw"
Status visit(EnterGRC &) { return Status(); }
Status visit(ExitGRC &) { return Status(); }
Status visit(Nop &) { return Status(); }
Status visit(Switch &) { return Status(); }
Status visit(STSuspend &) { return Status(); }
Status visit(Fork &) { return Status(); }
Status visit(Terminate &) { return Status(); }
Status visit(Enter &) { return Status(); }
#line 2977 "ASTGRC.nw"
  Dependencies() {}
  virtual ~Dependencies() {}

  static void compute(GRCNode *);
};
#line 3228 "ASTGRC.nw"
}
#endif
