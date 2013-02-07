#line 3233 "ASTGRC.nw"
#include <cstdio>
#include "ASTGRC.hpp"

namespace ASTGRC {
  
#line 586 "ASTGRC.nw"
GrcSynth::GrcSynth(Module *m, CompletionCodes &c)
  : module(m), code(c),
    surface_context(code.max() + 1),
    depth_context(code.max() +1),
    surface(surface_context, *this), depth(depth_context, *this),
    seltree(*this)
{
  assert(m);
  assert(m->types);
  integer_type = dynamic_cast<BuiltinTypeSymbol*>(m->types->get("integer"));
  assert(integer_type);
  boolean_type = dynamic_cast<BuiltinTypeSymbol*>(m->types->get("boolean"));
  assert(boolean_type);
  true_symbol = dynamic_cast<BuiltinConstantSymbol*>(m->constants->get("true"));
  assert(true_symbol);

  for ( SymbolTable::const_iterator i = m->signals->begin() ;
        i != m->signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    clone.sameSig(s);
  }

  for ( SymbolTable::const_iterator i = m->variables->begin() ;
        i != m->variables->end() ; i++ ) {
    VariableSymbol *s = dynamic_cast<VariableSymbol*>(*i);
    assert(s);
    clone.sameVar(s);
  }
}
#line 3238 "ASTGRC.nw"
  
#line 743 "ASTGRC.nw"
GrcWalker::GrcWalker(Context &c, GrcSynth &e)
    : context(c), environment(e), clone(e.clone) {}
#line 748 "ASTGRC.nw"
STNode *GrcWalker::stnode(const ASTNode &n) {
  assert(environment.ast2st.find(&n) != environment.ast2st.end() );
  return environment.ast2st[&n];
}
#line 3239 "ASTGRC.nw"
  
#line 831 "ASTGRC.nw"
Status Surface::visit(Pause &s) {
  Enter *en = new Enter(stnode(s));
  assert(en->st);
  *en >> context(1);
  context(0) = en;
  return Status();
}
#line 870 "ASTGRC.nw"
Status Surface::visit(Exit &s) {
  assert(s.trap);
  context(0) = context(environment.code[s.trap]);
  push_onto(context(0), new Action(clone(&s)));
  return Status();
}
#line 955 "ASTGRC.nw"
Status Surface::visit(IfThenElse &s) {
  Enter *en;
  assert(s.predicate);
  Test *t = new Test(stnode(s), clone(s.predicate));
  *t >> ( (s.else_part != 0) ? recurse(s.else_part) : context(0))
     >> ( (s.then_part != 0) ? recurse(s.then_part) : context(0));
  context(0) = t;
  en = new Enter(stnode(s));
  push_onto(context(0), en);
  return Status();
}
#line 1016 "ASTGRC.nw"
Status Surface::visit(StatementList &s) {

  for ( vector<Statement*>::reverse_iterator i = s.statements.rbegin() ;
	i != s.statements.rend() ; i++ ) {
    assert(*i);
    context(0) = synthesize(*i);
  }

  push_onto(context(0), new Enter(stnode(s)));
  return Status();
}
#line 1081 "ASTGRC.nw"
Status Surface::visit(Loop &s) {
  context(0) = synthesize(s.body);
  Enter *en = new Enter(stnode(s));
  push_onto(context(0), en);
  return Status();
}
#line 1131 "ASTGRC.nw"
Status Surface::visit(Every &s) {

  STNode *halt = stnode(s)->children[0]->children[0];
  Enter *enhalt = new Enter(halt);
  GRCNode *start;

  *enhalt >> context(1);

  Delay *d = dynamic_cast<Delay*>(s.predicate);
  if (d) {
    if (d->is_immediate) {
      context(0) = enhalt;
      Test *tst = new Test(stnode(s), clone(d->predicate));
      *tst >> enhalt >> synthesize(s.body);
      start = tst;

    } else {
      // A counted every
      assert(d->counter);
      StartCounter *scnt = new StartCounter(d->counter, clone(d->count));
      start = new Action(scnt);
      *start >> enhalt;
    }
  } else start = enhalt;
	
  push_onto(start, new Enter(stnode(s)));
  context(0) = start;

  return Status();
}
#line 1231 "ASTGRC.nw"
Status Surface::visit(Repeat &s) {
  context(0) = synthesize(s.body);
  assert(s.counter);
  StartCounter *stcnt = new StartCounter(s.counter, clone(s.count));
  push_onto(context(0), new Action(stcnt));
  Enter *en = new Enter(stnode(s));
  push_onto(context(0), en);
  return Status();
}
#line 1301 "ASTGRC.nw"
Status Surface::visit(Suspend &s)
{
  assert(s.body);
  GRCNode *start = recurse(s.body);

  assert(stnode(s)->children.size() >= 1); // Should have at least one child
  STNode *bodytree = stnode(s)->children.front();
  assert(bodytree); // First child is STref for the body

  // Put an Enter for the suspend's body just before the code for the body
  push_onto(start, new Enter(bodytree));
   
  Delay *d = dynamic_cast<Delay*>(s.predicate);
  if (d) {
    if (d->is_immediate) {

      // An immediate predicate (e.g., suspend .. when immediate A)
      STNode *imleaf = stnode(s)->children.back();
      Enter *enimleaf = new Enter(imleaf);
      *enimleaf >> context(1); // Enter the immediate additional leaf
      Test *tst = new Test(bodytree, clone(d->predicate));
      *tst >> start >> enimleaf;
      start = tst;

    } else {

      // A counted suspend (e.g., suspend .. when 5 A)
      assert(d->counter);
      StartCounter *scnt = new StartCounter(d->counter, clone(d->count));
      push_onto(start, new Action(scnt));

    }
  }

  // Put an Enter for the suspend statement itself at the beginning
  push_onto(start, new Enter(stnode(s)));

  context(0) = start;
  return Status();
}
#line 1462 "ASTGRC.nw"
Status Surface::visit(Abort &s) {
  if (s.is_weak) throw IR::Error("weak abort.  Did the dismantler run?");

  // Synthesize the surface of the body

  context.push();
  assert(s.body);
  GRCNode *start = recurse(s.body);
  context.pop();

  // Add an enter node for STref node under the abort

  assert(stnode(s)->children.size() >= 1); // Should be at least one child
  assert(stnode(s)->children.front()); // First child is STref for this body

  // The selection tree node for the body of the abort
  STNode *bodytree = stnode(s)->children.front();

  push_onto(start, new Enter(bodytree));

  // Add a check for each immediate predicate and "initialize counter"
  // for each counted predicate
   
  for ( vector<PredicatedStatement*>::reverse_iterator i = s.cases.rbegin() ;
	i != s.cases.rend() ; i++ ) {
    assert(*i);
    assert((*i)->predicate);
    Delay *d = dynamic_cast<Delay*>((*i)->predicate);
    if (d) {
      if (d->is_immediate) {

	// An immediate predicate: add a test an a handler

	assert(d->counter == NULL); // immediate delays shoudn't be counted
	assert(d->predicate);
	// FIXME: does the Test need this reference to the abort STref node?
	Test *tst = new Test( bodytree, clone(d->predicate) );
	assert(tst->st);
	*tst >> start
	  // If the predicate has a body, send control there
	     >> ( ((*i)->body) ? recurse((*i)->body)
		  : context(0) );
	start = tst;

      } else {

	// A counted predicate: add code that initializes the counter

        assert(d->counter);
	push_onto(start, new Action(new StartCounter(d->counter,
						     clone(d->count))));
	  
      }
    }
  }
 
  // Topmost node in the surface is an enter for the whole abort
  push_onto(start, new Enter(stnode(s)));

  context(0) = start;

  return Status();
}
#line 1652 "ASTGRC.nw"
Status Surface::visit(ParallelStatementList &s) {
  Sync *sync = new Sync(stnode(s));
  Fork *fork = new Fork(sync);
  Terminate *t;
  int nthr;

  GRCNode **outer = context.continuations.top();
  assert(outer);
  context.push();

  // Create a new terminate for every possible exit level
  // and link each from the sync node
  
  // Synthesize each thread's surface
  for ( vector<Statement*>::iterator i = s.threads.begin() ;i != s.threads.end() ; i++ ) {
    assert(*i);
    nthr=i-s.threads.begin();

    for(int tl=0; tl<context.size; tl++){
	    t = new Terminate(tl, nthr);
	    *t >> sync;
	    context(tl)=t;

	    if(tl != 1) {
	      Enter *en = new Enter( stnode(s)->children[nthr]->children[0] );
	      assert(en->st);
	      push_onto(context(tl), en);
            }
    }
   
    *fork >> recurse(*i); // it links thread to terminates, but each thread should have its own "enter"
  }

  // Connect the sync with outer context nodes

  for ( int i = 0 ; i < context.size ; i++ ){
	*sync >> outer[i];
  }

  context.pop();
  context(0) = fork;
  push_onto(context(0), new Enter(stnode(s)));
  return Status();
}
#line 1810 "ASTGRC.nw"
Status Surface::visit(Trap &s) {

  assert(s.symbols);
  assert(s.symbols->begin() != s.symbols->end());
  SignalSymbol *ts = dynamic_cast<SignalSymbol*>((*(s.symbols->begin())));
  assert(ts);

  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    clone.cloneLocalSignal(ss, environment.module->signals);
  }

  int level = environment.code[ts];
  assert(level > 1); // Should have been assigned by the completion codes class

  // Esterel permits multiple handlers, but the dismantler should
  // have removed them
  if (s.handlers.size() > 1)
    throw IR::Error("Multiple trap handler.  Did the dismantler run?");

  // Surface for the handler is either a normal termination or
  // the handler for the body
  assert( s.handlers.empty() || s.handlers.front() );
  GRCNode *handlerSurface =
     s.handlers.empty() ? context(0) : recurse(s.handlers.front()->body);

  // Synthesize the body
  context.push();

  assert(handlerSurface);
  context(level) = handlerSurface;

  assert(s.body);
  GRCNode *surface = synthesize(s.body);

  context.pop();

  assert(stnode(s));
  assert(stnode(s)->children.front());
  push_onto(surface, new Enter(stnode(s)->children.front()));

  // Add "DefineSignal" nodes for each of the traps

  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    push_onto(surface, new DefineSignal(clone(ss), true));
    clone.clearSig(ss);
  }

  push_onto(surface, new Enter(stnode(s)));

  context(0) = surface;

  return Status();
}
#line 1973 "ASTGRC.nw"
Status Surface::visit(Signal &s) {
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
        i != s.symbols->end() ; i++ ) {
    SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
    assert(sig);
    clone.cloneLocalSignal(sig, environment.module->signals);
  }

  context(0) = synthesize(s.body);

  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
        i!= s.symbols->end() ; i++ ) {
    SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
    assert(sig);
    push_onto(context(0), new DefineSignal(clone(sig), true));
    clone.clearSig(sig);
  }
  return Status();
}
#line 2042 "ASTGRC.nw"
Status Surface::visit(Var &s) {
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
	i != s.symbols->end() ; i++ ) {
    VariableSymbol *vs = dynamic_cast<VariableSymbol*>(*i);
    assert(vs);
    clone.hoistLocalVariable(vs, environment.module->variables);
  }
  s.symbols->clear(); // Make sure we do not do this again

  context(0) = synthesize(s.body);
  return Status();
}
#line 3240 "ASTGRC.nw"
  
#line 847 "ASTGRC.nw"
Status Depth::visit(Pause &s) {
  return Status();
}
#line 976 "ASTGRC.nw"
Status Depth::visit(IfThenElse &s) {
  Switch *sw = new Switch(stnode(s));
  *sw >> ( (s.else_part != 0) ? recurse(s.else_part) : context(0))
      >> ( (s.then_part != 0) ? recurse(s.then_part) : context(0));
  context(0) = sw;
  return Status();
}
#line 1036 "ASTGRC.nw"
Status Depth::visit(StatementList &s) {
  Switch *sw;
  if (!s.statements.empty()) {
    sw = new Switch(stnode(s));
    environment.surface_context.push(context);
    vector<Statement*>::reverse_iterator final = s.statements.rend();
    final--;
    for ( vector<Statement*>::reverse_iterator i = s.statements.rbegin() ;
	  i != s.statements.rend() ; i++ ) {
      assert(*i);
      *sw >> synthesize(*i); // Build the depth
      // Build the surface
      if (i != final ) context(0) = environment.surface.synthesize(*i);
    }
    environment.surface_context.pop();
    context(0) = sw;
  }
  return Status();
}
#line 1094 "ASTGRC.nw"
Status Depth::visit(Loop &s) {
  environment.surface_context.push(context);
  // Synthesize the surface
  context(0) = environment.surface.synthesize(s.body);
  // Synthesize the depth
  context(0) = synthesize(s.body);
  environment.surface_context.pop();
  return Status();
}
#line 1168 "ASTGRC.nw"
Status Depth::visit(Every &s) {

  Delay *d = dynamic_cast<Delay*>(s.predicate);
 
  Expression *pred =
    (d != NULL) ?
      ( d->is_immediate ?
	clone(d->predicate) :
	new CheckCounter(environment.boolean_type, d->counter, clone(d->predicate))
      ) :
    clone(s.predicate);

   Test *tst = new Test(stnode(s), pred);
   Enter *enhalt = new Enter(stnode(s)->children[0]->children[0]);
   *enhalt >> context(1);
   context(0) = enhalt;   

   Switch *sw = new Switch(stnode(s)->children[0]);
   *sw >> enhalt >> recurse(s.body);
   *tst >> sw;

   environment.surface_context.push(context);
   GRCNode *restart =  environment.surface.synthesize(s.body);
   environment.surface_context.pop();

   if(d && !d->is_immediate) {
      assert(d->counter);
      push_onto(restart, new Action(new StartCounter(d->counter,
						     clone(d->count))));
   }
   *tst >> restart;

   Enter *hold = new Enter(stnode(s));
   *hold >> tst;
   context(0) = hold;

  return Status();
}
#line 1247 "ASTGRC.nw"
Status Depth::visit(Repeat &s) {

  // std::cerr<<"depth visit\n";

  // Synthesize the surface
  environment.surface_context.push(context);
  GRCNode *restart = environment.surface.synthesize(s.body);
  environment.surface_context.pop();

  Test *tst = new Test(stnode(s),
     new CheckCounter(environment.boolean_type, s.counter,
	 new LoadVariableExpression(environment.true_symbol)));
  *tst >> restart >> context(0);
  //Synthesize the depth
  context(0) = tst;
  context(0) = synthesize(s.body);
 
  // std::cerr<<"visit ok\n";

  return Status();
}
#line 1344 "ASTGRC.nw"
Status Depth::visit(Suspend &s) {

  assert(s.predicate);
  assert(s.body);
  assert(stnode(s));

  assert(stnode(s)->children.size() >= 1); // Should have at least one child
  STNode *bodytree = stnode(s)->children.front();
  assert(bodytree); // First child is STref for the body

  Switch *swimm = new Switch(stnode(s));

  GRCNode *start = synthesize(s.body);

  Delay *d = dynamic_cast<Delay*>(s.predicate);
  Expression *pred =
    (d != NULL) ?
      ( d->is_immediate ?
	clone(d->predicate) :
	new CheckCounter(environment.boolean_type, d->counter,
	                 clone(d->predicate))
      ) :
    clone(s.predicate);

  // the depth test: body is already started
  
  Test *t = new Test(bodytree, pred);
  STSuspend *sts = new STSuspend(bodytree);
  *sts >> context(1);
  *t >> start >> sts;
  Enter *hold = new Enter(bodytree);
  *hold >> t;
  *swimm >> hold;

  // If the predicate is immediate then it's possible that the surface
  // still needs to start in the depth of the suspend (i.e., when
  // it was suspended in the first cycle)
  //
  // This section builds that portion of the code

  if (d && d->is_immediate) {
    assert(stnode(s)->children.size() == 2); // build in selection tree
    Enter *enimleaf = new Enter( stnode(s)->children.back() );
    Enter *en = new Enter( bodytree );
    *enimleaf >> context(1);
    t = new Test(NULL, clone(pred));
    environment.surface_context.push(context);
    start = environment.surface.synthesize(s.body);
    environment.surface_context.pop();
    push_onto(start, en);
    *t >> start >>enimleaf;
    *swimm >> t;
  }
 
  context(0) = swimm;
  return Status();
}
#line 1534 "ASTGRC.nw"
Status Depth::visit(Abort &s) {
  if (s.is_weak) throw IR::Error("weak abort.  Did the dismantler run?");

  // Synthesize the depth of the body

  context.push();
  assert(s.body);
  GRCNode *resume = recurse(s.body);//
  context.pop();

  // The selection tree node for the body of the abort

  STNode *bodytree = stnode(s)->children.front();
  push_onto(resume, new Enter(bodytree));

  // Add a check for each predicate that branches to the surface of
  // the handler.  Also add the depth of each handler

  for ( vector<PredicatedStatement*>::reverse_iterator i = s.cases.rbegin() ;
	i != s.cases.rend() ; i++ ) {
    assert(*i);
    assert((*i)->predicate);

    // Get the predicate expression: either the simple predicate,
    // the predicate of an immediate, or a counter check for counted
    // predicates

    Delay *d = dynamic_cast<Delay*>((*i)->predicate);
    Expression *pred =
      d ?
      (d->is_immediate ?
       clone(d->predicate) : new CheckCounter(environment.boolean_type,
          d->counter, clone(d->predicate)))
      :  clone((*i)->predicate);

    // Add a test for the predicate

    Test *tst = new Test( bodytree, pred );
    GRCNode *handler;
    if ((*i)->body) {
      environment.surface_context.push(context);
      handler = environment.surface.synthesize((*i)->body);
      environment.surface_context.pop();
    } else handler = context(0);

    *tst >> resume >> handler;
    resume = tst;
  }
 
  // The switch at the top of the depth for the abort

  Switch *topswitch = new Switch( stnode(s) );

  // Its first child is the code for the body of the abort

  *topswitch >> resume;

  // Its remaining children are the bodies of the handlers

  for ( vector<PredicatedStatement*>::const_iterator i = s.cases.begin() ;
	i != s.cases.end() ; i++ ) {
    assert(*i);
    assert((*i)->predicate);
    if ((*i)->body) *topswitch >> recurse((*i)->body);
  }

  context(0) = topswitch;

  return Status();
}
#line 1703 "ASTGRC.nw"
Status Depth::visit(ParallelStatementList &s) {
  Sync *sync = new Sync(stnode(s));
  Fork *fork = new Fork(sync);
  Enter *en;
  int nthr;
  Terminate *t;

  GRCNode **outer = context.continuations.top();
  assert(outer);
  context.push();

  // Create a new terminate for every possible exit level
  // and link each from the sync node


  // Synthesize each thread's surface
  for ( vector<Statement*>::iterator i = s.threads.begin() ;
	i != s.threads.end() ; i++) {
    assert(*i);
    nthr=i-s.threads.begin();
    Switch *sw = new Switch( stnode(s)->children[nthr] );
    assert(sw->st);
    *fork >> sw;
 
    for(int tl = 0; tl < context.size; tl++){
	t = new Terminate(tl, nthr);
	context(tl)=t;
	*t >> sync;
        // this is the self looping enter
	if(tl == 0){
          en=new Enter( stnode(s)->children[nthr]->children[0] );
          assert(en->st);
          push_onto(context(tl), en);	
	  *sw >> en;
	}
    }

   *sw >> recurse(*i);
  } 

  for ( int i = 0 ; i < context.size ; i++ )
	*sync >> outer[i];

  context.pop();
  context(0) = fork;
  push_onto(context(0), new Enter(stnode(s))); // hold
  return Status();
}
#line 1879 "ASTGRC.nw"
Status Depth::visit(Trap &s) {

  assert(s.symbols);
  assert(s.symbols->begin() != s.symbols->end());
  SignalSymbol *ts = dynamic_cast<SignalSymbol*>((*(s.symbols->begin())));
  assert(ts);
  int level = environment.code[ts];
  assert(level > 1); // Should have been assigned by the dismantler

  // Clone each of the trap signals

  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    clone.cloneLocalSignal(ss, environment.module->signals);
  }

  // Esterel permits multiple handlers, but the dismantler should
  // have removed them
  if (s.handlers.size() > 1)
    throw IR::Error("Multiple trap handler.  Did the dismantler run?");

  // Surface for the handler is either a normal termination or
  // the handler for the body
  assert( s.handlers.empty() || s.handlers.front() );
  GRCNode *handlerSurface = context(0);
  if (!s.handlers.empty()) {
    environment.surface_context.push(context);   
    handlerSurface = environment.surface.synthesize(s.handlers.front()->body);
    environment.surface_context.pop();
  }

  GRCNode *handlerDepth =
     s.handlers.empty() ? 0 : recurse(s.handlers.front()->body);

  // Synthesize the body
  context.push();

  assert(handlerSurface);
  context(level) = handlerSurface;

  assert(s.body);
  GRCNode *depth = synthesize(s.body);

  context.pop();

  Switch *topswitch = new Switch( stnode(s) );
  *topswitch >> depth;

  if (handlerDepth) *topswitch >> handlerDepth;

  // Delete the mapping for each of the traps
  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    clone.clearSig(ss);
  } 

  context(0) = topswitch;
  return Status();
}
#line 1995 "ASTGRC.nw"
Status Depth::visit(Signal &s) {
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
        i != s.symbols->end() ; i++ ) {
    SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
    assert(sig);
    clone.cloneLocalSignal(sig, environment.module->signals);
  }

  context(0) = synthesize(s.body);
   for ( SymbolTable::const_iterator i = s.symbols->begin() ;
	 i!= s.symbols->end() ; i++ ) {
     SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
     assert(sig);
     push_onto(context(0), new DefineSignal(clone(sig), false));
     clone.clearSig(sig);
  }
  return Status();
}
#line 2057 "ASTGRC.nw"
Status Depth::visit(Var &s) {
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
	i != s.symbols->end() ; i++ ) {
    VariableSymbol *vs = dynamic_cast<VariableSymbol*>(*i);
    assert(vs);
    clone.hoistLocalVariable(vs, environment.module->variables);
  }
  s.symbols->clear(); // Make sure we do not do this again

  context(0) = synthesize(s.body);
  return Status();
}
#line 3241 "ASTGRC.nw"
  
#line 778 "ASTGRC.nw"
void SelTree::setNode(const ASTNode &n, STNode *sn) {
  assert(sn);
  environment.ast2st[&n] = sn;
}
#line 817 "ASTGRC.nw"
Status SelTree::visit(Pause &s){
  STleaf *leaf = new STleaf();
  setNode(s, leaf);
  return Status(leaf);
}
#line 937 "ASTGRC.nw"
Status SelTree::visit(IfThenElse &s) {  
  STexcl *ite = new STexcl();
  setNode(s, ite);

  *ite >> (s.else_part ? synthesize(s.else_part) : new STref())
       >> (s.then_part ? synthesize(s.then_part) : new STref());

  return Status(ite);
}
#line 996 "ASTGRC.nw"
Status  SelTree::visit(StatementList &s)
{
  STexcl *excl = new STexcl();
  setNode(s, excl);

  for ( vector<Statement*>::reverse_iterator i = s.statements.rbegin() ;
	i != s.statements.rend() ; i++ ){
    assert(*i);
    *excl >> synthesize(*i);
  }

  return Status(excl);
}


#line 1068 "ASTGRC.nw"
Status SelTree::visit(Loop &s) {
  STref *lp = new STref();
  setNode(s, lp);
  *lp >> synthesize(s.body);
  return Status(lp);
}
#line 1112 "ASTGRC.nw"
Status SelTree::visit(Every &s) {

  STref *ab = new STref(); ab->setabort(); setNode(s, ab);
  STexcl *excl = new STexcl();
  STleaf *halt = new STleaf();

  *excl >> halt >> synthesize(s.body);
  *ab >> excl;

  return Status(ab);

}
#line 1218 "ASTGRC.nw"
Status SelTree::visit(Repeat &s) {
  STref *lp = new STref();
  setNode(s, lp);
  *lp >> synthesize(s.body);
  return Status(lp);
}
#line 1282 "ASTGRC.nw"
Status SelTree::visit(Suspend &s)
{
  STexcl *ex = new STexcl();
  STref *sp = new STref();
  sp->setsuspend();
 
  *ex >> sp;
  Delay *d = dynamic_cast<Delay*>(s.predicate);
  if (d && d->is_immediate) *ex >> new STleaf();

  assert(s.body);
  *sp >> synthesize(s.body);

  setNode(s, ex);
  return Status(ex);
}
#line 1424 "ASTGRC.nw"
Status SelTree::visit(Abort &s) {

  // The selection tree for the body of the abort:
  //  An STref node whose only child is the tree
  //  for the body of the abort

  assert(s.body); // Any abort should have a body
  STref *bodytree = new STref();
  bodytree->setabort();
  *bodytree >> synthesize(s.body);

  // The root of the tree for the abort: an exclusive
  // whose first child is the tree for the body of the abort

  STexcl *exclusive = new STexcl();
  *exclusive >> bodytree;

  // Attach the selection tree for each non-vacuous handler
  // under the exclusive node at the top of the abort

  for ( vector<PredicatedStatement*>::const_iterator i = s.cases.begin() ;
	  i != s.cases.end() ; i++ ) {
      assert(*i);
      assert((*i)->predicate);
      if ((*i)->body) *exclusive >> synthesize((*i)->body);
  }

  setNode(s, exclusive);
  return Status(exclusive);
}
#line 1627 "ASTGRC.nw"
Status SelTree::visit(ParallelStatementList &s)
{
  STpar *par = new STpar();
  setNode(s, par);

  for ( vector<Statement*>::iterator i = s.threads.begin() ;
	i != s.threads.end() ; i++ ) {
    assert(*i);
    STexcl *ex = new STexcl();
    *par >> ex; 
    STleaf *term = new STleaf();
    term->setfinal();
    *ex >> term;
    *ex >> synthesize(*i);
  }

  return Status(par);
}
#line 1766 "ASTGRC.nw"
Status SelTree::visit(Trap &s) {

  // Create the topmost exclusive node

  STexcl *exclusive = new STexcl();

  // Create the subtree for the body of the Trap; attach it to the top

  assert(s.body);
  STref *bodytree = new STref();
  *bodytree >> synthesize(s.body);
  *exclusive >> bodytree;

  // Create the subtree for the handler, if any

  switch (s.handlers.size()) {
  case 0:
    // No handler; nothing to do
    break;
  case 1:
    // Single handler: add the selection tree for it to the exclusive node
    assert(s.handlers.front());
    assert(s.handlers.front()->body);
    *exclusive >> synthesize(s.handlers.front()->body);
    break;
  default:
    // Esterel permits multiple handlers, but the dismantler should
    // have removed them
    throw IR::Error("Multiple trap handler.  Did the dismantler run?");
    break;
  }

  setNode(s, exclusive);
  return Status(exclusive);  
}
#line 1965 "ASTGRC.nw"
Status SelTree::visit(Signal &s) {
  STNode *st = new STref();
  *st >> synthesize(s.body);
  return Status(st);
}
#line 2034 "ASTGRC.nw"
Status SelTree::visit(Var &s) {
  STNode *st = new STref();
  *st >> synthesize(s.body);
  return Status(st);
}
#line 3242 "ASTGRC.nw"
  
#line 2985 "ASTGRC.nw"
void Dependencies::compute(GRCNode *root)
{
  assert(root);

  Dependencies depper;

  depper.dfs(root);

  for ( map<SignalSymbol *, SignalNodes>::const_iterator i =
	  depper.dependencies.begin() ; i != depper.dependencies.end() ;
	i++ ) {
    const SignalNodes &sn = (*i).second;
    if (!sn.writers.empty() && !sn.readers.empty()) {
      for ( set<GRCNode*>::const_iterator j = sn.writers.begin() ;
	    j != sn.writers.end() ; j++ )
	for ( set<GRCNode*>::const_iterator k = sn.readers.begin() ;
	      k != sn.readers.end() ; k++ )
	  **k << *j;
    }
  }
}
#line 3015 "ASTGRC.nw"
void Dependencies::dfs(GRCNode *n)
{
  if (!n || visited.find(n) != visited.end() ) return;

  visited.insert(n);

  current = n;
  n->welcome(*this);

  for (vector<GRCNode*>::const_iterator i = n->successors.begin() ;
       i < n->successors.end() ; i++ ) dfs(*i);  
}
#line 3038 "ASTGRC.nw"
Status Dependencies::visit(Action &act)
{
  assert(act.body);
  act.body->welcome(*this);
  return Status();
}
#line 3163 "ASTGRC.nw"
Status Dependencies::visit(Sync &s)
{
  for ( vector<GRCNode*>::const_iterator i = s.predecessors.begin() ;
        i != s.predecessors.end() ; i++ ) {
    // Every predecessor should be a terminate node
    assert( dynamic_cast<Terminate*>(*i) );
    s << *i;
  }
  return Status();
}
#line 3243 "ASTGRC.nw"
  
#line 2165 "ASTGRC.nw"
RecursiveSynth::RecursiveSynth(Module *m, CompletionCodes &c)
  : module(m), code(c), context(code.max() + 1) {
  assert(m);

  assert(m->types);
  boolean_type = dynamic_cast<BuiltinTypeSymbol*>(m->types->get("boolean"));
  assert(boolean_type);
  true_symbol = dynamic_cast<BuiltinConstantSymbol*>(m->constants->get("true"));
  assert(true_symbol);


  for ( SymbolTable::const_iterator i = m->signals->begin() ;
        i != m->signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    clone.sameSig(s);
  }

  for ( SymbolTable::const_iterator i = m->variables->begin() ;
        i != m->variables->end() ; i++ ) {
    VariableSymbol *s = dynamic_cast<VariableSymbol*>(*i);
    assert(s);
    clone.sameVar(s);
  }
}
#line 2197 "ASTGRC.nw"
GRCgraph *RecursiveSynth::synthesize()
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

  context(0) = term0;
  context(1) = term1;

  // Synthesize the trees

  synthesize(module->body);

  *stroot >> finished >> stnode >> boot;
  *top_switch >> enfinished >> depth >> surface;

  GRCgraph *result = new GRCgraph(stroot, engrc);

  // Verify the control-flow graph is acyclic

  visit(engrc);

  return result;
}
#line 2264 "ASTGRC.nw"
void RecursiveSynth::visit(GRCNode *n)
{
  assert(n);
  if (visiting.find(n) != visiting.end() && visiting[n])
    throw IR::Error("Cyclic control-flow!");

  visiting[n] = true;

  for (vector<GRCNode*>::const_iterator i = n->successors.begin() ;
       i < n->successors.end() ; i++ )
    if (*i) visit(*i);

  visiting[n] = false;
}
#line 2287 "ASTGRC.nw"
Status RecursiveSynth::visit(Pause &s)
{
  stnode = new STleaf();

  surface = new Enter(stnode);
  *surface >> context(1);

  depth = context(0);

  return Status();
}
#line 2307 "ASTGRC.nw"
Status RecursiveSynth::visit(Exit &s)
{
  assert(s.trap);
  stnode = new STref();
  surface = new Action(clone(&s));
  *surface >> context(code[s.trap]);
  depth = context(0);
  return Status();
}
#line 2325 "ASTGRC.nw"
Status RecursiveSynth::visit(Emit &s)
{
  stnode = new STref();
  surface = new Action(clone(&s));
  *surface >> context(0);
  depth = context(0);
  return Status();
}
#line 2342 "ASTGRC.nw"
Status RecursiveSynth::visit(Assign &s)
{
  stnode = new STref();
  surface = new Action(clone(&s));
  *surface >> context(0);
  depth = context(0);
  return Status();
}
#line 2359 "ASTGRC.nw"
Status RecursiveSynth::visit(IfThenElse &s)
{  
  if (s.then_part) {
    context.push();
    synthesize(s.then_part);
    context.pop();
  }
  STNode *stthen = s.then_part ? stnode : new STref();
  GRCNode *surfacethen = s.then_part ? surface : context(0);
  GRCNode *depththen = s.then_part ? depth : context(0);
  
  if (s.else_part) {
    context.push();
    synthesize(s.else_part);
    context.pop();
  }
  STNode *stelse = s.else_part ? stnode : new STref();
  GRCNode *surfaceelse = s.else_part ? surface : context(0);
  GRCNode *depthelse = s.else_part ? depth : context(0);

  stnode = new STexcl();
  *stnode >> stelse >> stthen;

  surface = new Test(stnode, clone(s.predicate));
  *surface >> surfaceelse >> surfacethen;

  run_before(surface, new Enter(stnode));

  depth = new Switch(stnode);
  *depth >> depthelse >>depththen;

  return Status();
}
#line 2401 "ASTGRC.nw"
Status RecursiveSynth::visit(StatementList &s)
{
  if ( s.statements.empty() ) {
    stnode = new STref();
    surface = depth = context(0);
    run_before(surface, new Enter(stnode));
    return Status();
  }

  STexcl *stroot = new STexcl();
  Switch *depthswitch = new Switch(stroot);

  for ( vector<Statement*>::reverse_iterator i = s.statements.rbegin() ;
	i != s.statements.rend() ; i++ ) {
    synthesize(*i);
    context(0) = surface;
    *depthswitch >> depth;
    *stroot >> stnode;
  }

  stnode = stroot;
  depth = depthswitch;
  run_before(surface, new Enter(stnode));
  return Status();
}
#line 2443 "ASTGRC.nw"
Status RecursiveSynth::visit(Loop &s)
{
  STref *lp = new STref();

  Nop *loop_bottom = new Nop();

  context(0) = loop_bottom;

  assert(s.body);
  synthesize(s.body);
  *loop_bottom >> surface;

  *lp >> stnode;
  stnode = lp;

  run_before(surface, new Enter(lp));
  return Status();
}
#line 2470 "ASTGRC.nw"
Status RecursiveSynth::visit(Every &s)
{
  STref *stroot = new STref();
  stroot->setabort();
  STexcl *excl = new STexcl();
  STleaf *halt = new STleaf();

  Enter *enhalt = new Enter(halt);
  *enhalt >> context(1);

  context.push();
  context(0) = enhalt;
  synthesize(s.body);
  context.pop();

  *excl >> halt >> stnode;
  *stroot >> excl;

  GRCNode *bsurface = surface;

  Expression *predicate = NULL;
  Delay *d = dynamic_cast<Delay*>(s.predicate);
  if (d) {
    if (d->is_immediate) {
      assert(d->predicate);
      Test *tst = new Test(stroot, clone(d->predicate));
      *tst >> enhalt >> surface;
      surface = tst;
      predicate = clone(d->predicate);
    } else {
      assert(d->counter);
      StartCounter *scnt = new StartCounter(d->counter, clone(d->count));
      surface = new Action(scnt);
      *surface >> enhalt;
      predicate =
	 new CheckCounter(boolean_type, d->counter, clone(d->predicate));
    }
  } else {
    surface = enhalt;
    predicate = clone(s.predicate);
  }

  Test *tst = new Test(stroot, predicate);
  Switch *sw = new Switch(excl);
  *sw >> enhalt >> depth;
  *tst >> sw >> bsurface;

  depth = tst;

  run_before(surface, new Enter(stroot));
  run_before(depth, new Enter(stroot));

  stnode = stroot;
  return Status();
}
#line 2534 "ASTGRC.nw"
Status RecursiveSynth::visit(Repeat &s)
{
  STref *stroot = new STref();

  GRCNode *context0 = context(0);

  Test *tst = new Test(stroot, new CheckCounter(boolean_type, s.counter, 
			 new LoadVariableExpression(true_symbol)));
  context(0) = tst;

  assert(s.body);
  synthesize(s.body);
  *tst >> surface >> context0;

  *stroot >> stnode;
  stnode = stroot;

  assert(s.counter);
  run_before(surface, new Action(new StartCounter(s.counter, clone(s.count))));
  run_before(surface, new Enter(stroot));
  return Status();
}
#line 2566 "ASTGRC.nw"
Status RecursiveSynth::visit(Suspend &s)
{
  synthesize(s.body);

  // Selection tree node for the body of the suspend
  STref *stbody = new STref();
  stbody->setsuspend();
  *stbody >> stnode;
  stnode = stbody;

  run_before(surface, new Enter(stbody));

  Test *immediate_test = NULL;

  Expression *predicate = NULL;
  Delay *d = dynamic_cast<Delay*>(s.predicate);
  if (d) {
    if (d->is_immediate) {
      STNode *imm = new STleaf();

      STexcl *ex = new STexcl();
      *ex >> stnode >> imm;
      stnode = ex;

      // Machinery for the surface: an extra test
      Enter *en = new Enter(imm);
      *en >> context(1);
      immediate_test = new Test(stbody, clone(d->predicate));
      *immediate_test >> surface >> en;
      surface = immediate_test;      
      run_before(surface, new Enter(stnode));

      predicate = clone(d->predicate);
            
    } else {
      // A counted suspend (suspend .. when 5 A)
      assert(d->counter);
      run_before(surface, new Action(new StartCounter(d->counter,
						      clone(d->count))));
      predicate = new CheckCounter(boolean_type, d->counter,
				   clone(d->predicate));
    }
  } else {
    predicate = clone(s.predicate);
  }
  assert(predicate);

  // Machinery for the depth: a test that sends control to either
  // an ST suspend node to context(1) (e.g., the suspend condition)
  // or the depth of the body

  STSuspend *sts = new STSuspend(stbody);
  *sts >> context(1);
  Test *t = new Test(stbody, predicate);
  *t >> depth >> sts;
  depth = t;
  run_before(depth, new Enter(stbody));

  if (immediate_test) {
    Switch *sw = new Switch(stnode);
    *sw >> depth >> immediate_test;
    depth = sw;
  }

  return Status();
}
#line 2641 "ASTGRC.nw"
Status RecursiveSynth::visit(Abort &s)
{
  if (s.is_weak) throw IR::Error("weak abort.  Did the dismantler run?");

  context.push();
  synthesize(s.body);
  context.pop();

  STref *stbody = new STref();
  stbody->setabort();
  *stbody >> stnode;

  run_before(surface, new Enter(stbody));
  run_before(depth, new Enter(stbody));

  GRCNode *bsurface = surface;
  GRCNode *bdepth = depth;

  STexcl *stroot = new STexcl();
  *stroot >> stbody;

  Switch *depth_root = new Switch(stroot);
  Nop *start_depth = new Nop();
  *depth_root >> start_depth;
  
  for ( vector<PredicatedStatement*>::reverse_iterator i = s.cases.rbegin() ;
	i != s.cases.rend() ; i++ ) {
    assert(*i);
    
    if ((*i)->body) {
      context.push();
      synthesize((*i)->body);
      context.pop();
      *stroot >> stnode;
      *depth_root >> depth;
    }
    
    assert((*i)->predicate);
    Expression *predicate = NULL;
    Delay *d = dynamic_cast<Delay*>((*i)->predicate);
    if (d) {
      if (d->is_immediate) {
	// An immediate delay
	
	assert(d->counter == NULL);
	assert(d->predicate);
	
	Test *t = new Test(stbody, clone(d->predicate));
	*t >> bsurface >> ( (*i)->body ? surface : context(0) );
	bsurface = t;
	
	predicate = clone(d->predicate);
	
      } else {
	// A counted delay
	assert(d->counter);
	run_before(bsurface, new Action(new StartCounter(d->counter,
							 clone(d->count))));
	
	predicate =
	  new CheckCounter(boolean_type, d->counter, clone(d->predicate));
      }
    } else {
      predicate = clone((*i)->predicate);
    }
    assert(predicate);

    Test *t = new Test(stbody, predicate);
    *t >> bdepth >> ( (*i)->body ? surface : context(0) );
    bdepth = t;

  }
    
  stnode = stroot;
  run_before(bsurface, new Enter(stroot));
  surface = bsurface;
  *start_depth >> bdepth;
  depth = depth_root;

  return Status();
}
#line 2731 "ASTGRC.nw"
Status RecursiveSynth::visit(ParallelStatementList &s)
{
  STpar *stroot = new STpar();

  Sync *sync = new Sync(stroot);
  Fork *surface_fork = new Fork(sync);
  Fork *depth_fork = new Fork(sync);

  context.push();

  for ( vector<Statement*>::iterator i = s.threads.begin() ;
	i != s.threads.end() ; i++ ) {
    assert(*i);

    int threadnum = i - s.threads.begin();

    STleaf *term = new STleaf();
    term->setfinal();

    for ( int l = 0 ; l < context.size ; l++ ) {
      Terminate *t = new Terminate(l, threadnum);
      *t >> sync;
      context(l) = t;
      // This extra enter should only be necessary for level 0 terminates,
      // since higher levels necessarily terminate the thread group,
      // however, they appear to be necessary so the optimizer doesn't get
      // too aggressive on potentially vacuous depth
      if (l != 1) run_before(context(l), new Enter(term));
    }

    GRCNode *terminate0 = context(0);

    synthesize(*i);

    // Selection tree fragment: track whether the thread has terminated

    STexcl *ex = new STexcl();
    *stroot >> ex;
    *ex >> term >> stnode;

    run_before(surface, new Enter(ex));

    *surface_fork >> surface;

    Switch *sw = new Switch(ex);    
    *sw >> terminate0 >> depth;
    *depth_fork >> sw;
  }

  context.pop();

  // Connect the sync to every continuation in the context

  for ( int i = 0 ; i < context.size ; i++ )
    *sync >> context(i);

  surface = surface_fork;
  run_before(surface, new Enter(stroot));

  depth = depth_fork;
  run_before(depth, new Enter(stroot)); // Actually, a "hold"

  stnode = stroot;
  return Status();
}
#line 2805 "ASTGRC.nw"
Status RecursiveSynth::visit(Trap &s)
{
  assert(s.body);
  assert(s.symbols->size());
  if (s.handlers.size() >= 2)
    throw IR::Error("Multiple trap handler.  Did the dismantler run?");

  SignalSymbol *ts = dynamic_cast<SignalSymbol*>((*(s.symbols->begin())));
  int level = code[ts];
  assert(level > 1); // Should have been assigned by the CompletionCodes class

  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    clone.cloneLocalSignal(ss, module->signals);
  }
  
  GRCNode *handlerSurface = context(0);
  GRCNode *handlerDepth = NULL;

  STref *bodytree = new STref(); // Selection tree for the body of the trap
  STNode *stroot = bodytree;
  STNode *topExclusive = NULL;

  if ( !s.handlers.empty() ) {
    assert(s.handlers.front());
  
    context.push();
    synthesize(s.handlers.front()->body);
    handlerSurface = surface;
    handlerDepth = depth;

    stroot = topExclusive = new STexcl();
    *topExclusive >> bodytree >> stnode;

    context.pop();
  }

  context.push();
  context(level) = handlerSurface;
  synthesize(s.body);
  context.pop();

  *bodytree >> stnode;

  for (SymbolTable::const_iterator i = s.symbols->begin() ;
       i != s.symbols->end() ; i++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*i);
    assert(ss);
    assert(ss->kind == SignalSymbol::Trap);
    run_before(surface, new DefineSignal(ss, true));

    clone.clearSig(ss);
  }

  run_before(surface, new Enter(bodytree));

  if (handlerDepth) {
    run_before(surface, new Enter(stroot));
    Switch *depth_switch = new Switch(stroot);
    *depth_switch >> depth >> handlerDepth;
    depth = depth_switch;
  }
  
  stnode = stroot;
  return Status();
}
#line 2884 "ASTGRC.nw"
Status RecursiveSynth::visit(Signal &s)
{
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
        i != s.symbols->end() ; i++ ) {
    SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
    assert(sig);
    clone.cloneLocalSignal(sig, module->signals);
  }
  
  synthesize(s.body);
  
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
	i!= s.symbols->end() ; i++ ) {
    SignalSymbol *sig = dynamic_cast<SignalSymbol*>(*i);
    assert(sig);
    run_before(surface, new DefineSignal(clone(sig), true));
    run_before(depth, new DefineSignal(clone(sig), false));
    clone.clearSig(sig);
  }

  return Status();
}

Status RecursiveSynth::visit(Var &s)
{
  for ( SymbolTable::const_iterator i = s.symbols->begin() ;
	i != s.symbols->end() ; i++ ) {
    VariableSymbol *vs = dynamic_cast<VariableSymbol*>(*i);
    assert(vs);
    clone.hoistLocalVariable(vs, module->variables);
  }

  synthesize(s.body);
  return Status();
}
#line 2928 "ASTGRC.nw"
Status RecursiveSynth::visit(ProcedureCall &s)
{
  stnode = new STref();
  surface = new Action(clone(&s));
  *surface >> context(0);
  depth = context(0);
  return Status();
}
#line 2947 "ASTGRC.nw"
Status RecursiveSynth::visit(Exec &s)
{
  stnode = new STref();
  surface = context(0);
  depth = context(0);
  return Status();
}
#line 3244 "ASTGRC.nw"
}
