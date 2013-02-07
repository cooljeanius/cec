#line 1661 "ExpandModules.nw"
#ifndef _DISMANTLE_HPP
#  define _DISMANTLE_HPP

#  include "AST.hpp"
#  include <assert.h>
#  include <sstream>
#  include <set>
#  include <map>

namespace ExpandModules {
  using namespace IR;
  using namespace AST;
  using std::set;
  using std::map;

  
#line 123 "ExpandModules.nw"
class FindRoots : public Visitor {
  Modules *modules;
public:
  set<Module*> roots;
  
#line 132 "ExpandModules.nw"
FindRoots(Modules *m) {
  assert(m);
  modules = m;
}

virtual ~FindRoots() {}
#line 146 "ExpandModules.nw"
const set<Module*> *find();
#line 172 "ExpandModules.nw"
void recurse(ASTNode* n) { if (n) n->welcome(*this); }
#line 182 "ExpandModules.nw"
Status visit(Run &);
#line 206 "ExpandModules.nw"
Status visit(Nothing &) { return Status(); }
Status visit(Pause &) { return Status(); }
Status visit(Halt &) { return Status(); }
Status visit(Exit &) { return Status(); }
Status visit(Emit &) { return Status(); }
Status visit(Sustain &) { return Status(); }
Status visit(Assign &) { return Status(); }
Status visit(ProcedureCall &) { return Status(); }
#line 223 "ExpandModules.nw"
Status visit(Module &m) { recurse(m.body); return Status(); }
Status visit(Loop &s) { recurse(s.body); return Status(); }
Status visit(Repeat &s) { recurse(s.body); return Status(); }
Status visit(Signal &s) { recurse(s.body); return Status(); }
Status visit(Var &s) { recurse(s.body); return Status(); }
Status visit(LoopEach &s) { recurse(s.body); return Status(); }
Status visit(Every &s) { recurse(s.body); return Status(); }
Status visit(Suspend &s) { recurse(s.body); return Status(); }
Status visit(DoUpto &s) { recurse(s.body); return Status(); }
#line 235 "ExpandModules.nw"
Status visit(DoWatching &s) {
  recurse(s.body);
  recurse(s.timeout);
  return Status();
}
#line 243 "ExpandModules.nw"
Status visit(Trap &s) {
  recurse(s.body);
  for (vector<PredicatedStatement*>::const_iterator i = s.handlers.begin() ;
       i != s.handlers.end() ; i++ ) {
    assert(*i);   
    recurse((*i)->body);
  }
  return Status();
}
#line 257 "ExpandModules.nw"
Status visit(StatementList &l) {
  for (vector<Statement*>::iterator i = l.statements.begin() ;
       i != l.statements.end() ; i++ )
    recurse(*i);
  return Status();
}
#line 266 "ExpandModules.nw"
Status visit(ParallelStatementList &l) {
  for (vector<Statement*>::iterator i = l.threads.begin() ;
       i != l.threads.end() ; i++ )
    recurse(*i);
  return Status();
}
#line 277 "ExpandModules.nw"
Status visit(IfThenElse &s) {
  recurse(s.then_part);
  recurse(s.else_part);
  return Status();
}
#line 285 "ExpandModules.nw"
Status visit(Exec &s) {
  for (vector<TaskCall*>::iterator i = s.calls.begin() ;
       i != s.calls.end() ; i++ ) {
    assert(*i);    
    recurse((*i)->body);
  }
  return Status();
}
#line 301 "ExpandModules.nw"
void visitCase(CaseStatement *s) {
  for ( vector<PredicatedStatement*>::const_iterator i = s->cases.begin() ;
	i != s->cases.end() ; i++ ) {
    assert(*i);
    recurse((*i)->body);
  }
  recurse(s->default_stmt);
}
#line 312 "ExpandModules.nw"
Status visit(Present &s) { visitCase( (CaseStatement*) &s); return Status(); }
Status visit(If &s) { visitCase( (CaseStatement*) &s); return Status(); }
Status visit(Await &s) { visitCase( (CaseStatement*) &s); return Status(); }
#line 318 "ExpandModules.nw"
Status visit(Abort &s) {
  visitCase( (CaseStatement*) &s);
  recurse(s.body);
  return Status();
}
#line 128 "ExpandModules.nw"
};
#line 1677 "ExpandModules.nw"
  class Copier;
  
#line 1583 "ExpandModules.nw"
class ExpressionCopier {
  Expression *theExpr;
  Copier *theCopier;
public:
  ExpressionCopier(Expression*, Copier*);
  Expression *copy();
};
#line 1679 "ExpandModules.nw"
  
#line 346 "ExpandModules.nw"
class Copier : public Visitor {
  map<const Symbol*, Symbol*> formalToActual;
  map<const SymbolTable*, SymbolTable*> copiedSymbolTableMap;
  map<const ConstantSymbol*, ExpressionCopier*> newConstantExpression;
  map<const Counter*, Counter*> copiedCounterMap;
  Modules *oldModules;
  Modules *newModules;
  Module *moduleBeingCopied;
  Module *newModule;
public:
  
#line 361 "ExpandModules.nw"
Copier(Modules *om, Modules *nm)
  : oldModules(om), newModules(nm), moduleBeingCopied(0), newModule(0) {
  assert(nm);
  assert(om);
}

Copier(Copier *c) {
  assert(c);
  oldModules = c->oldModules;
  newModules = c->newModules;
  newModule = c->newModule;
  moduleBeingCopied = c->moduleBeingCopied;
}

virtual ~Copier() {}
#line 383 "ExpandModules.nw"
template <class T> T* copy(T* n) {
  T* result = n ? dynamic_cast<T*>(n->welcome(*this).n) : 0;
  assert(result || !n);
  return result;
}
#line 397 "ExpandModules.nw"
template <class T> T* actualSymbol(const T* s) {
  if (!s) return 0;
  // std::cerr << "actualSymbol " << s->name << std::endl;
  map<const Symbol*, Symbol*>::const_iterator i = formalToActual.find(s);
  assert(i != formalToActual.end());        // Should be in the map
  assert((*i).second);                       // Should be non-NULL
  T* result = dynamic_cast<T*>((*i).second);
  assert(result);                            // Should be the same type
  return result;
}
#line 412 "ExpandModules.nw"
SymbolTable *newSymbolTable(const SymbolTable *s) {
  assert(s);
  map<const SymbolTable*, SymbolTable*>::const_iterator i =
     copiedSymbolTableMap.find(s);
  assert(i != copiedSymbolTableMap.end());
  SymbolTable *result = (*i).second;
  assert(result);
  return result;
}
#line 424 "ExpandModules.nw"
Counter *newCounter(const Counter *c) {
  if (c) {
    assert(copiedCounterMap.find(c) != copiedCounterMap.end());
    return copiedCounterMap[c];
  }
  return 0;
}
#line 441 "ExpandModules.nw"
void copySymbolTable(const SymbolTable *, SymbolTable *);
#line 472 "ExpandModules.nw"
Module *copyModule(Module *);
#line 517 "ExpandModules.nw"
Status visit(Exclusion &e) {
  Exclusion *result = new Exclusion();
  for ( vector<SignalSymbol*>::const_iterator i = e.signals.begin() ;
	i != e.signals.end() ; i++ ) {
    assert(*i);
    result->signals.push_back(actualSymbol(*i));
  }
  return result;
}
#line 529 "ExpandModules.nw"
Status visit(Implication &e) {
  return new Implication(actualSymbol(e.predicate),
			 actualSymbol(e.implication));
}
#line 543 "ExpandModules.nw"
Status visit(Run &);
#line 1203 "ExpandModules.nw"
Status visit(TypeSymbol &s) { return new TypeSymbol(s.name); }

Status visit(BuiltinTypeSymbol &s) { return new BuiltinTypeSymbol(s.name); }

Status visit(ConstantSymbol &s) {
  return new ConstantSymbol(s.name, actualSymbol(s.type), copy(s.initializer));
}

Status visit(BuiltinConstantSymbol &s) {
  return new BuiltinConstantSymbol(s.name, actualSymbol(s.type), 
                                   copy(s.initializer));
}
#line 1218 "ExpandModules.nw"
Status visit(FunctionSymbol&);
Status visit(BuiltinFunctionSymbol&);
Status visit(ProcedureSymbol&);
Status visit(TaskSymbol&);
#line 1286 "ExpandModules.nw"
Status visit(SignalSymbol &s)
{
  // std::cerr << "Copy signal " << s.name << std::endl;
  return
    new SignalSymbol( s.name, actualSymbol(s.type), (SignalSymbol::kinds) s.kind,
		      actualSymbol(s.combine), copy(s.initializer), actualSymbol(s.reincarnation) );
}

Status visit(BuiltinSignalSymbol &s)
{
  return
    new BuiltinSignalSymbol( s.name, actualSymbol(s.type),
	                     (SignalSymbol::kinds) s.kind,
			     actualSymbol(s.combine));
}
#line 1304 "ExpandModules.nw"
Status visit(VariableSymbol &s) {
  return new VariableSymbol(s.name, actualSymbol(s.type), copy(s.initializer));
}
#line 1314 "ExpandModules.nw"
Status visit(Nothing &) { return new Nothing(); }
Status visit(Pause &) { return new Pause(); }
Status visit(Halt &) { return new Halt(); }
#line 1322 "ExpandModules.nw"
Status visit(Emit &e) {
  return new Emit(actualSymbol(e.signal), copy(e.value));
}

Status visit(Sustain &s) {
  return new Sustain(actualSymbol(s.signal), copy(s.value));
}
#line 1334 "ExpandModules.nw"
Status visit(Assign &a) {
  return new Assign(actualSymbol(a.variable), copy(a.value));
}
#line 1340 "ExpandModules.nw"
Status visit(Exit &e) { return new Exit(actualSymbol(e.trap), copy(e.value)); }
#line 1344 "ExpandModules.nw"
Status visit(ProcedureCall &);
#line 1370 "ExpandModules.nw"
Status visit(StatementList&);
Status visit(ParallelStatementList&);
#line 1399 "ExpandModules.nw"
Status visit(IfThenElse &s) {
  return new IfThenElse(copy(s.predicate), copy(s.then_part),
			copy(s.else_part));
}
#line 1406 "ExpandModules.nw"
Status visit(Loop &s) { return new Loop(copy(s.body)); }
#line 1410 "ExpandModules.nw"
Status visit(Repeat &s) {
  return new Repeat(copy(s.body), copy(s.count), s.is_positive,
                    newCounter(s.counter));
}
#line 1417 "ExpandModules.nw"
Status visit(LoopEach &s) {
  return new LoopEach( copy(s.body), copy(s.predicate));
}
#line 1423 "ExpandModules.nw"
Status visit(Every &s) {
  return new Every( copy(s.body), copy(s.predicate));
}
#line 1429 "ExpandModules.nw"
Status visit(Suspend &s) {
  return new Suspend( copy(s.body), copy(s.predicate));
}
#line 1435 "ExpandModules.nw"
Status visit(DoWatching &s) {
  return new DoWatching( copy(s.body), copy(s.predicate), copy(s.timeout));
}
#line 1441 "ExpandModules.nw"
Status visit(DoUpto &s) {
  return new DoUpto( copy(s.body), copy(s.predicate));
}
#line 1447 "ExpandModules.nw"
Status visit(Exec &);
#line 1483 "ExpandModules.nw"
void copyCases(const CaseStatement &, CaseStatement *);
#line 1501 "ExpandModules.nw"
Status visit(Abort &s) {
  Abort *result = new Abort(copy(s.body), s.is_weak);
  copyCases(s, result);
  return result;
}
#line 1509 "ExpandModules.nw"
Status visit(Await &s) {
  Await *result = new Await();
  copyCases(s, result);
  return result;
}
#line 1517 "ExpandModules.nw"
Status visit(Present &s) {
  Present *result = new Present();
  copyCases(s, result);
  return result;
}

Status visit(If &s) {
  If *result = new If();
  copyCases(s, result);
  return result;
}
#line 1533 "ExpandModules.nw"
Status visit(Var &s) {
  Var *result = new Var();
  result->symbols = new SymbolTable();
  copySymbolTable(s.symbols, result->symbols);
  result->body = copy(s.body);
  return result;
}
#line 1543 "ExpandModules.nw"
Status visit(Signal &s) {
  Signal *result = new Signal();
  result->symbols = new SymbolTable();
  copySymbolTable(s.symbols, result->symbols);
  result->body = copy(s.body);
  return result;
}
#line 1553 "ExpandModules.nw"
Status visit(Trap &s) {
  Trap *result = new Trap();
  result->symbols = new SymbolTable();
  copySymbolTable(s.symbols, result->symbols);
  result->body = copy(s.body);
  for ( vector<PredicatedStatement *>::const_iterator i = s.handlers.begin() ;
        i != s.handlers.end() ; i++ ) {
    assert(*i);
    result->newHandler( copy((*i)->predicate), copy((*i)->body) );
  }
  return result;
}
#line 1573 "ExpandModules.nw"
Status visit(Literal &l) { return new Literal(l.value, actualSymbol(l.type)); }
#line 1600 "ExpandModules.nw"
Status visit(LoadVariableExpression &e) {
  // std::cerr << "copying variable load " << e.variable->name << std::endl;
  ConstantSymbol *cs = dynamic_cast<ConstantSymbol*>(e.variable);
  if (cs) {
    map<const ConstantSymbol*, ExpressionCopier*>::iterator i =
  	newConstantExpression.find(cs);
    if (i != newConstantExpression.end())
      return (*i).second->copy();
  }
  return new LoadVariableExpression(actualSymbol(e.variable));
}
#line 1614 "ExpandModules.nw"
Status visit(LoadSignalExpression &e) {
  return new LoadSignalExpression(e.type, actualSymbol(e.signal));
}

Status visit(LoadSignalValueExpression &e) {
  return new LoadSignalValueExpression(actualSymbol(e.signal));
}
#line 1624 "ExpandModules.nw"
Status visit(Delay &d) {
  return new Delay(actualSymbol(d.type), copy(d.predicate),
		   copy(d.count), d.is_immediate, newCounter(d.counter));
}
#line 1631 "ExpandModules.nw"
Status visit(UnaryOp &o) {
  return new UnaryOp(actualSymbol(o.type), o.op, copy(o.source));
}

Status visit(BinaryOp &o) {
  return new BinaryOp(actualSymbol(o.type), o.op,
		      copy(o.source1), copy(o.source2));
}
#line 1642 "ExpandModules.nw"
Status visit(FunctionCall &);
#line 357 "ExpandModules.nw"
};
#line 1680 "ExpandModules.nw"
  
#line 90 "ExpandModules.nw"
Modules *find_roots_and_rewrite(Modules *);
#line 1681 "ExpandModules.nw"
}
#endif
