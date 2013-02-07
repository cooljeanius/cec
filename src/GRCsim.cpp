#line 1008 "GRCsim.nw"
#include "GRCsim.hpp"

using namespace std;

namespace AST {
  
#line 36 "GRCsim.nw"
Status GRCsim::visit(Fork &s)
{
  if (debug) cerr << cfgmap[&s] << ":fork" << '\n';
  for (vector<GRCNode *>::iterator i = s.successors.begin() ;
       i !=  s.successors.end() ; i++) schedule(*i,globalcc);
  return Status();
}
#line 54 "GRCsim.nw"
Status GRCsim::visit(Terminate &s)
{
  if (debug) cerr << cfgmap[&s] << ":term " << s.code << endl;
  schedule_child(&s,globalcc);
  return Status();
}
#line 73 "GRCsim.nw"
Status GRCsim::visit(Sync &s)
{
  int MAXTERM = 0;
  int range_bg = 0;    //range begin
  int range_end = 0;   //range end
  int i;

  assert(s.dataPredecessors.size() > 0); // grcdps should have computed them

  //find maximam term lvl
  for ( vector<GRCNode *>::const_iterator it = s.dataPredecessors.begin() ;
	it != s.dataPredecessors.end() ; it++ ) {
    Terminate *t = dynamic_cast<Terminate*>(*it);
    assert(t); assert(t->code >= 0);
    if (debug & debugSync)
    if (debug) cerr << cfgmap[&s] << ":sync checking "
		    << cfgmap[t] << ":term " << t->code << '\n';
    if (t->code > MAXTERM)
      MAXTERM = t->code;
  }

  int orall[MAXTERM+1]; //term lvls and correspond ctrl values

  for(i = 0; i <= MAXTERM; i++)
    orall[i] = 0;

  //OR all the term lvl code & control value
  for ( vector<GRCNode *>::const_iterator it = s.dataPredecessors.begin() ;
        it != s.dataPredecessors.end() ; it++ ) {
    Terminate *t = dynamic_cast<Terminate*>(*it);

    if (sched[t] != 0){ 
      if (sched[t] == 1)
        orall[t->code] = 1;
      else if (orall[t->code] == 0)
        orall[t->code] = sched[t];
    }
  }

  //get the range of active term lvls
  bool bg = false;
  for (i = MAXTERM; i >= 0; i--){
    if (!bg){
      if (orall[i] != 0){
        bg = true;
        range_bg = i;
      }
    }
    if (orall[i] == 1)
      break;
  }
  range_end = i;

  assert(range_bg < (int) s.successors.size());
  assert(orall[range_bg] != 0);

  if (orall[range_bg] == -1){//unknown
    if (debug) cerr << cfgmap[&s] << ":may sync at level ";
    for(i = range_bg; i >= range_end; i--){
      if(orall[i] != 0){
        schedule(s.successors[i],-1);
        if(debug) cerr<< i << ' ';
      }
    }
    if(debug) cerr<<'\n';
  }
  else{
    assert(range_bg == range_end);
    assert(orall[range_bg] == 1);
    schedule(s.successors[range_bg],globalcc);
    if (debug) cerr << cfgmap[&s] << ":sync at level " << range_bg << '\n';
  }

  return Status();  
}
#line 160 "GRCsim.nw"
void GRCsim::setState(STNode *n, bool isInit)
{  
  STexcl *exclusive = 0;

  for (;;) {
    assert(n);

    STNode *parent = n->parent;

    if (dynamic_cast<STpar*>(parent) != NULL) {
      if (debug) cerr << "parent of node " << stmap[n]
	  	      << " is a parallel node" << endl;
      return;
    }

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

  // Check for program termination

  if (exclusive == stroot) {
    STleaf *lf = dynamic_cast<STleaf *>(n);
    if ((lf) && (lf->isfinal())) {
      ISFinal = true;
      if (debug) cerr << "program terminated\n";
    }
  }

  if(isInit){
    state[exclusive] = childnum;
    if (debug) cerr << "   state[" << stmap[exclusive] << "] = "
                  << childnum << endl;
  }
  else{
    nextstate[exclusive]=childnum;
    if (debug) cerr << "   nextstate[" << stmap[exclusive] << "] = "
                  << childnum << endl;
  }

  //NOTE: Enter does not set state if any input signal is unknown 
  //FIXME: it may be wrong and should change to no ctrl flw is unknown in 
  //in current state. 
}
#line 228 "GRCsim.nw"
Status GRCsim::visit(Enter &s)
{
  if (debug) cerr << cfgmap[&s] << ":enter " << stmap[s.st];
  setState(s.st,false);
  schedule_child(&s,globalcc);
  return Status();
}
#line 246 "GRCsim.nw"
Status GRCsim::visit(STSuspend &s)
{//FIXME: anything special?
  if (debug) cerr << cfgmap[&s] << ":suspend " << stmap[s.st]<<"\n";
  schedule_child(&s,globalcc);
  return Status();
}
#line 268 "GRCsim.nw"
Status GRCsim::visit(Action &s)
{
  if (debug) cerr << cfgmap[&s];
  s.body->welcome(*this);  
  schedule_child(&s,globalcc);
  return Status(); 
}
#line 280 "GRCsim.nw"
Status GRCsim::visit(Emit &e)
{
  assert(e.signal);
  SignalSymbol *ss = e.signal;
  if(ss->type && ss->type->name=="integer"){
	signals_v[ss]=intVal(e.value);
	if(debug) cerr<<" value : "<<signals_v[ss]<<" ";
  }
  if (debug) cerr << ":emit " << ss->name <<" = "<<globalcc<< endl;
  if(signals[ss] != 1)
    signals[ss] = globalcc;

  return Status();
}
#line 299 "GRCsim.nw"
Status GRCsim::visit(Exit &e)
{
  assert(e.trap);
  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support Exit yet!\n";
  SignalSymbol *ss = e.trap;
  if (debug) cerr << ":exit " << ss->name << endl;
  signals[ss] = globalcc; //FIXME: correct?

  return Status();
}
#line 314 "GRCsim.nw"
Status GRCsim::visit(Assign &a)
{
  assert(a.variable);
  assert(a.value);
  assert(a.variable->type);

  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support Assign yet!\n";

  if (a.variable->type->name != "integer" &&
      a.variable->type->name != "boolean") {
    throw IR::Error("Only integer and boolean variables supported");
  }

  var[a.variable] = intVal(a.value);

  return Status();
}
#line 336 "GRCsim.nw"
Status GRCsim::visit(StartCounter &stcnt)
{
  if (debug) cerr << ":start counter\n";
  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support StartCounter yet!\n";
  assert(stcnt.counter);
  assert(stcnt.count);
  counters[stcnt.counter] = intVal(stcnt.count);
  return Status();
}
#line 350 "GRCsim.nw"
Status GRCsim::visit(Nop &n)
{
  if (debug) cerr << ":nop\n";
  schedule_child(&n,globalcc);
  return Status();
}
#line 363 "GRCsim.nw"
Status GRCsim::visit(Switch &s)
{
  assert(s.st);

  //Notice that switch alway has one active child - no matter 
  // its ctrlflow value is 1 or x, just pass by the ctrl value
  // to this child.

  int act_ch = doswitch(s.st);

  assert(act_ch < (int) s.successors.size());
  GRCNode *child = s.successors[act_ch];
  if (debug) cerr << cfgmap[&s] << ":switch " << stmap[s.st]
		  << " --" << act_ch << "--> "
		  << (child ? cfgmap[child] : -1) << '\n';
  if (child) schedule(child,globalcc);
  
  return Status();
}
#line 387 "GRCsim.nw"
Status GRCsim::visit(Test &s)
{
  GRCNode *successor;

  assert(s.predicate);
  //assert(s.successors.size()==2);
  int predvalue = intVal(s.predicate);

  if (predvalue==-1){//unknown

    if (debug) {
      cerr << cfgmap[&s] << ":test -- " << predvalue << " --> ";
      for(vector<GRCNode *>::iterator i = s.successors.begin();
       i != s.successors.end(); i++){
        if (*i) cerr << cfgmap[*i];
        else cerr<<" (none)";
      }
      cerr<<"\n";
    }

    ternary = true;

    for(vector<GRCNode *>::iterator i = s.successors.begin();
       i != s.successors.end(); i++){
      schedule(*i,-1);
    }
  }
  else{
    assert ( predvalue >= 0 && predvalue < (int)s.successors.size() );
    successor = s.successors[predvalue];

    if (debug) {
      cerr << cfgmap[&s] << ":test --" << predvalue << "--> ";
      if (successor) cerr << cfgmap[successor];
      else cerr << "(none)";
      cerr << '\n';
    }

    if (successor) schedule(successor,globalcc);
  }
  
  return Status(); 
}
#line 435 "GRCsim.nw"
Status GRCsim::visit(Literal &s)
{
  int val;
  if ( sscanf(s.value.c_str(), "%d", &val) != 1 ) assert(0);
  return Status(val);
}
#line 444 "GRCsim.nw"
Status GRCsim::visit(LoadSignalExpression &lse)
{
  SignalSymbol *ss = lse.signal;
  assert(ss);
  return Status(signals[ss]);
}
#line 453 "GRCsim.nw"
Status GRCsim::visit(LoadSignalValueExpression &lse)
{
  SignalSymbol *ss = lse.signal;
  assert(ss);
  return Status(signals_v[ss]);
}
#line 462 "GRCsim.nw"
Status GRCsim::visit(LoadVariableExpression &lve)
{
  VariableSymbol *vs = lve.variable;
  assert(vs);
  if (var.find(vs) == var.end())
    throw IR::Error("reading uninitialized variable " + vs->name);
  return Status(var[vs]);
}
#line 474 "GRCsim.nw"
Status GRCsim::visit(CheckCounter &chkcnt)
{
  assert(chkcnt.counter);
  assert(chkcnt.predicate);
  if (intVal(chkcnt.predicate))
    counters[chkcnt.counter]--;

  return Status( counters[chkcnt.counter] == 0);
}
#line 486 "GRCsim.nw"
Status GRCsim::visit(BinaryOp &e)
{
  assert(e.source1);
  assert(e.source2);
  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support BinaryOp yet!\n";
  int val1 = intVal(e.source1);
  int val2 = intVal(e.source2);

  int result =
    (e.op == "and") ? (val1 && val2) :
    (e.op == "or") ? (val1 || val2) :
    (e.op == "+") ? (val1 + val2) :
    (e.op == "-") ? (val1 - val2) :
    (e.op == "*") ? (val1 * val2) :
    (e.op == "/") ? (val1 / val2) :
    (e.op == "mod") ? (val1 % val2) :
    (e.op == "=") ? (val1 == val2) :
    (e.op == "<>") ? (val1 != val2) :
    (e.op == "<") ? (val1 < val2) :
    (e.op == "<=") ? (val1 <= val2) :
    (e.op == ">") ? (val1 > val2) :
    (e.op == ">=") ? (val1 >= val2) :
    0;

  return Status(result);
}
#line 515 "GRCsim.nw"
Status GRCsim::visit(UnaryOp &e)
{
  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support UnaryOp yet!\n";

  int val = intVal(e.source);

  int result =
    (e.op == "not") ? ( !val ) :
    (e.op == "-") ? ( -val ) :
    0;
   
  return Status(result);
}
#line 531 "GRCsim.nw"
Status GRCsim::visit(FunctionCall &)
{
  throw IR::Error("Function calls are not supported");
}
#line 547 "GRCsim.nw"
Status GRCsim::visit(DefineSignal &d)
{
  assert(d.signal);
  SignalSymbol *ss = d.signal;
  assert(ss);
  if(debug) cerr<<"Warning: 3-valued-simulator does NOT support DefineSignal yet!\n";
  if (debug) cerr << cfgmap[&d] << ":DefineSignal " << ss->name << '\n';
  signals[ss] = 0;
  schedule_child(&d,globalcc);
  return Status();
}
#line 657 "GRCsim.nw"
void GRCsim::schedule(GRCNode *n, int ctrlflw)
{
  assert(ctrlflw != 0);
  if (n){
    if (sched[n] != 1)
      sched[n] = ctrlflw;
  }
}

void GRCsim::schedule_child(GRCNode *n, int ctrlflw)
{
  assert(n);
  assert(n->successors.size() <= 1);
  if ( !n->successors.empty() )
    schedule(n->successors.front(),ctrlflw);
}
#line 695 "GRCsim.nw"
int GRCsim::doswitch(STNode *n)
{
  if ( (dynamic_cast<STexcl*>(n)) == NULL )
    cerr << "error: testing non-excl node\n";
  if ( state.find(n) == state.end() ){
    cerr << "error: testing unitialized state for node " << stmap[n] << endl; 
    assert(0);
  }

  assert(state[n] >= 0);
  assert(state[n] < (int) n->children.size());
  
  return state[n];
}
#line 719 "GRCsim.nw"
void GRCsim::dfs(GRCNode *n){
  vector<GRCNode *>::iterator i;
  
  if (!n) return;

  if ( debug & debugDFS ) cerr << "visiting " << cfgmap[n] << "...";
  if ( dfs_notwhite.count(n) > 0 ) {
    if (debug & debugDFS ) cerr << "visited before\n";
    if (dfs_black.count(n) == 0) {
      cerr << "GRCsim: cycle detected\n";      
      exit(100);
    }
    return;
  }
  dfs_notwhite.insert(n);

  if (jump) {
    if (debug & debugDFS ) cerr << "JUMP!\n";
    jump = false;
  }

  if ( debug & debugDFS ) cerr << "visiting\n";

  if ( debug & debugDFS ) cerr << "dfs children\n";
  for (i = n->successors.begin(); i != n->successors.end(); i++)
    dfs(*i);
  
  if (n->dataSuccessors.size()>0){
    if ( debug & debugDFS ) cerr << "dfs datadps children\n";
    for (i = n->dataSuccessors.begin();
	 i != n->dataSuccessors.end(); i++){
	jump = true;
	dfs(*i);
	jump = true;
      }
  }
  
  assert(n);
  topo.push_back(n);
  dfs_black.insert(n);
  if ( debug & debugDFS ) cerr << "push back " << cfgmap[n] << "\n";
}
#line 770 "GRCsim.nw"
void GRCsim::execute_max(int maxticks)
{
  int ntick;
  
  useold=0;
  
  init();
  ntick=0;
  ISFinal = false;

  do {
    if (debug) cerr << "######## TICK " << ntick << endl;
    char buf[5];
    sprintf(buf, "%4d ", ntick);
    outf << buf;
    ntick++;
    clear_inputs();
    dotick();
  } while ((!ISFinal) && (maxticks < 0 || (ntick < maxticks)));
}
#line 799 "GRCsim.nw"
void GRCsim::execute_vectors(std::istream &vf)
{
  string line;

  // Enumerate the inputs to be read from the vector file

  vector<SignalSymbol*> inputs;

  for ( SymbolTable::const_iterator isym = sigs->begin() ;
	isym != sigs->end() ; isym++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*isym);
    assert(ss);
    if ( ss->name != "tick" && (ss->kind == SignalSymbol::Input ||
				ss->kind == SignalSymbol::Inputoutput ) ) {
      if ( debug & debugVectors ) std::cout << ss->name << '\n';
      inputs.push_back(ss);
    }
  }

  init();

  int ntick = 0;
  ISFinal = false;
  do {
    getline(vf, line);
    if (vf.fail()) break;

    if ( line.size() < inputs.size() ) {
      cerr << "Not enough inputs (" << line.size() << '<' << inputs.size()
	   << ") in test vector file\n"
           << "Got \"" << line << "\"\n";
      exit(-2);
    }

    clear_inputs();
    string::const_iterator j = line.begin();
    for ( vector<SignalSymbol*>::const_iterator i = inputs.begin() ;
	  i != inputs.end() ; j++ )
	if ((*j) != ' ') {
          if ((*j) == '1')
            signals[*i] = 1;
          else if ((*j) == '0')
  	    signals[*i] = 0;
          else{ 
            signals[*i] = -1; //unknown
            ternary = true; //FIXME: may be wrong
          }
          i++;
	}
    
    if (debug) cerr << "######## TICK " << ntick <<" TV :"<<line<< endl;
    char buf[5];
    sprintf(buf, "%4d ", ntick);
    outf << buf;
    ntick++;

    dotick();
  } while (!ISFinal);
}
#line 868 "GRCsim.nw"
void GRCsim::init()
{
  entergrc = dynamic_cast<EnterGRC*>(top->control_flow_graph);
  if (entergrc)
  	grcroot = dynamic_cast<GRCNode*>(entergrc->successors.back());
  else
	grcroot = dynamic_cast<GRCNode*>(top->control_flow_graph);
  assert (grcroot);
  stroot = dynamic_cast<STexcl *>(top->selection_tree);
  assert(stroot);
  boot = dynamic_cast<STleaf* >(stroot->children.back());
  assert(boot);

  globalcc = 1;  
  setState(boot,true);
  ternary = false;
  
  if ( debug & debugDFS ) cerr << "will dfs\n";
  jump = false;
  dfs(grcroot);
  if (debug & debugDFS ) cerr << "init ok\n";
  
  if (debug & debugDFS ){
    cerr << "--------topo-------:\n";
    for (vector<GRCNode*>::iterator i = topo.end()-1; i >= topo.begin(); i--)
      cerr << cfgmap[*i] << ' ';
    cerr << "\n";
  }

  // Initialize constants

  assert(module->constants);
  for ( SymbolTable::const_iterator i = module->constants->begin();
	i != module->constants->end() ; i++ ) {
    ConstantSymbol *cs = dynamic_cast<ConstantSymbol*>(*i);
    if (cs && cs->initializer) {
      assert(cs->type);
      if (cs->type->name != "integer" &&
	  cs->type->name != "boolean")
	throw IR::Error("only integer and boolean constants are supported");
      var[cs] = intVal(cs->initializer);
    }
  }
}
#line 919 "GRCsim.nw"
void GRCsim::clear_inputs()
{
  for( SymbolTable::const_iterator isym = sigs->begin() ; isym != sigs->end() ;
       isym++ ) {
	
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*isym);
    if (ss) {
	signals[ss] = (ss->name == "tick");
	if(ss->type && ss->type->name=="integer") signals_v[ss]=0;
	}	
  }
}
#line 938 "GRCsim.nw"
void GRCsim::dotick()
{   
  int tsz = topo.size();
  
  for (int i = 0 ; i < tsz ; i++) sched[topo[i]] = 0;

  sched[ topo[tsz-1] ] = 1;
  
  for ( int i = tsz - 1 ; i >= 0 ; i-- ) {
    assert(topo[i]);
    globalcc = 0;
    if ( sched[topo[i]] != 0 ){
      globalcc = sched[topo[i]];
      topo[i]->welcome(*this);
    }
  }
  
  for ( SymbolTable::const_iterator isym = sigs->begin() ;
	isym != sigs->end() ; isym++ ) {
    SignalSymbol *ss = dynamic_cast<SignalSymbol*>(*isym);
    assert(ss);
    if (debug) cerr << ss->name << " : " << signals[ss] << "\n";
    if ( ss->kind == SignalSymbol::Output ){
      outf << ss->name << "=" << signals[ss] << " ";
      if(ss->type && ss->type->name=="integer") outf<<"("<<signals_v[ss]<<") ";
    }
    else if ( ss->kind == SignalSymbol::Inputoutput )
      outf << ss->name << "_IO_O=" << (signals[ss] ? '1' : '0') << " ";
  }
  outf << "\n";

  if(!ternary){ //FIXME: other effects, like counter, also should 
                // be taken back if ternary = true
    for(map<STNode *,int>::iterator it = nextstate.begin();
        it != nextstate.end(); it++){
      state[(*it).first] = (*it).second; 
    }
    //nextstate.swap(state);
    nextstate.clear();
  }
  ternary = false;
}
#line 1014 "GRCsim.nw"
}
