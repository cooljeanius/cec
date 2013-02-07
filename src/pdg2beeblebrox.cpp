#include "pdg2beeblebrox.hpp"


// sort nodes in topological order, according to PDG ( CDG _AND_ DDG )
void pdg2beeblebrox::dfs(GRCNode *n)
{
  if(!n || dfs_black.count(n) > 0 ) return;
  dfs_black.insert(n);
  
  for(vector<GRCNode*>::iterator i = n->successors.begin(); i!= n->successors.end(); i++)
    dfs(*i);
  if(dynamic_cast<Terminate*>(n))
    for(vector<GRCNode*>::iterator i = n->dataSuccessors.begin(); i!= n->dataSuccessors.end(); i++)
      dfs(*i);

  topo.insert(topo.begin(), n);
}

// rec. walk the ST and build state machines for the exclusive nodes
void pdg2beeblebrox::sm_init(STNode *n)
{
  int nchildren, c;

  if(!n) return;
  if(dynamic_cast<STexcl*>(n)){
    smachines[n] = new state_machine(this, n);
  }

  nchildren = n->children.size();
  for(c=0; c<nchildren; c++){
    if(n->children[c] == boot) smachines[n]->setBoot(c);
    sm_init((STNode*)n->children[c]);
  }

}

// rec.walk the ST and generates the circuitry for the state_machines
void pdg2beeblebrox::sm_build(STNode *n, wire *susp)
{

  int nchildren, c;
  wire *w;

  if(!n) return;
 
  if(suspends.find(n) != suspends.end()){
    w = current->build_orgate(suspends[n]);
    susp = current->build_orgate(susp, w);
  }

  if(dynamic_cast<STexcl*>(n)){
    smachines[n] -> setSuspend(susp);
    if(opt.ff) smachines[n] -> build_circuit_ff();
    else smachines[n]-> build_circuit();
  }  

  nchildren = n->children.size();
  
  for(c=0; c<nchildren; c++){
    sm_build((STNode*)n->children[c], susp);
  }

}

// this is a hack; counters should be in a symbol table
void pdg2beeblebrox::init_counters()
{

  for(vector<Counter*>::iterator ic = mod->counters.begin(); ic!=mod->counters.end(); ic++){
    counters[*ic] = new beeble_counter();
    //    cerr<<"new counter !"<<*ic<<"\n";
  }
}

void pdg2beeblebrox::close_counters()
{

  for(vector<Counter*>::iterator ic = mod->counters.begin(); ic!=mod->counters.end(); ic++)
   counters[*ic]->build();
}

void pdg2beeblebrox::init_signals()
{
  SignalSymbol *sg;
  wire *sigw;

  for(SymbolTable::const_iterator isym = sigs->begin(); isym != sigs->end(); isym++){
    sg = dynamic_cast<SignalSymbol*>(*isym);
    assert(sg);

    //    cerr<<"signal "<<sg->name<<" of type "<<sg->direction<<"\n";

    sigw = new wire();
    if(sg -> kind == SignalSymbol::Output){
      sigw->required = sigw->output = 1;
      sigw->name = sg->name;
      sigw->monitor = 3;
    }
    if(sg->kind == SignalSymbol::Inputoutput){
      sigw->required = sigw->output = 1;
      sigw->name = sg->name+"_IO_O";
      sigw->monitor = 3;
    }
    if(sg->kind == SignalSymbol::Input){
      sigw->name = sg->name+"_O";
    }
    if(sg->kind  == SignalSymbol::Local){
      sigw->name = sg->name;
    }


    signals[sg] = new zaphod_signal();
    signals[sg] -> signal = sigw;

  }
}
void pdg2beeblebrox::close_signals()
{

  SignalSymbol *sg;
  wire *sigw;

  for(SymbolTable::const_iterator isym = sigs->begin(); isym != sigs->end(); isym++){
    sg = dynamic_cast<SignalSymbol*>(*isym);
    

    if(sg->kind == SignalSymbol::Input || sg->kind==SignalSymbol::Inputoutput){
        if(sg->name == "tick"){
	  signals[sg]->sigemit.clear();
          signals[sg]->sigemit.push_back(w_1);
	} else {
	  // add external emitter
	  if(sg->kind==SignalSymbol::Input) sigw = new wire(sg->name); else
	    sigw = new wire(sg->name+"_IO_I");
	  sigw->input = 1;
	  signals[sg]->sigemit.push_back(sigw);         
	}
    }
    sigw=current->build_orgate(signals[sg]->sigemit);
    new strap(signals[sg]->signal, sigw?sigw:w_0);
  }
    
}

// this is the main function of pdg2beeblebrox
void pdg2beeblebrox::execute()
{

  Enter *en; STSuspend *sp;

  debug = 0;

  dfs(top_flow);
  sm_init(stroot);

  init_signals();
  init_counters();


  for(vector<GRCNode*>::iterator in = topo.begin(); in != topo.end(); in++){
    v_flow[*in] = new vector<wire*>();
    or_flow[*in] = NULL;
  }

  v_flow[top_flow]->push_back(w_1);

  for(vector<GRCNode*>::iterator in = topo.begin(); in != topo.end(); in++){
    if (debug) cerr<<"node "<< cfgmap[*in]<<"\n";
    (*in)->welcome(*this);
  }

  close_signals();
  close_counters();

  for(vector<GRCNode*>::iterator in = topo.begin(); in != topo.end(); in++){
    if((en = dynamic_cast<Enter*>(*in)))
      enter_sm(*en);
    if((sp = dynamic_cast<STSuspend*>(*in)))
      suspend_sm(*sp);
  }  


  sm_build(stroot, w_0);

  if(opt.ff) cv.remove_unused();
  if(opt.gendot) cv.drawdot();
  // no v5, , , don't put internal signals in testbench
  if(opt.genver) cv.build_interface(mod->symbol->name, false, opt.genxilinx, opt.gentb, false);
  cv.build_blif(mod->symbol->name);
}

// used by most nodes, except sync
wire * pdg2beeblebrox::flow_or_parents(GRCNode *n)
{
  wire *flow;
  flow =or_flow[n] = current->build_orgate(*v_flow[n]);
  return flow;
}

// pushes the same flow to all children; used by most nodes, except switch, test, sync
void pdg2beeblebrox::flow_all_children(GRCNode *n, wire *flow)
{
  for(vector<GRCNode*>::iterator ic=n->successors.begin(); ic!=n->successors.end(); ic++)
    if(*ic) v_flow[*ic]->push_back(flow);
}

// adds flow between parent and child
void pdg2beeblebrox::flow_add_arc(GRCNode *p, GRCNode *c, wire *flow)
{
  if(!flow || flow == w_0) return;
  v_flow[c]->push_back(flow);
}


// following we have a visit() function for each type of GRC node
// briefly, each node computes its flow and pushes the right flow to its children
// most nodes just or the flow from parents and push it children, but
// - sync : most complicated, handles termination levels (see below)
// - switch, test : the children flow depends on the tested condition

Status pdg2beeblebrox::visit(Switch &s)
{
  wire *flowin;
  int nchildren, c;
  GRCNode *ch;
  state_machine * sm;

  if(debug) cerr<<"Switch\n";

  flowin = flow_or_parents(&s);
  sm = smachines[s.st];
  
  nchildren = s.successors.size();
  for(c=0; c < nchildren; c++){
    ch = s.successors[c];
    if(!ch) continue;
    flow_add_arc(&s, ch, current->build_andgate(flowin, sm -> chkState(c))); // check the state
  }
  return Status();
}

// enter has 2 functions
// first one is visit(); as most nodes, it compues the flow
// the second (enter_sm) it is called later (after all the flow is built)
//   to generate the real inputs to the state machines

Status pdg2beeblebrox::visit(Enter &s)
{
  if(debug) cerr<<"Enter\n";
  flow_or_parents(&s);
  return Status();
}

Status pdg2beeblebrox::enter_sm(Enter &s)
{

  wire *flowin;
  STNode *st_par;
  state_machine *sm;
  int nchildren, c;

  vector<wire *> succ_enters;
  wire *wcancel, *realenter;

  if(debug) cerr<<"enter_sm\n";

  flowin = or_flow[&s];
  
  st_par = s.st->parent;
  if(dynamic_cast<STexcl*>(st_par)){
    sm = smachines[st_par];
    // find the child number
    nchildren = st_par->children.size();
    for(c=0;c<nchildren;c++)
      if(st_par->children[c] == s.st) break;
    assert(c < nchildren);
    
    for(vector<GRCNode*>::iterator is = s.dataSuccessors.begin(); is != s.dataSuccessors.end(); is++){
      succ_enters.push_back(or_flow[*is]);
    }
    wcancel = current -> build_orgate(succ_enters);
    realenter = current-> build_andgate( flowin, current->build_notgate(wcancel));

    sm -> addEnter(c, realenter);
  }
  return Status();
}


Status pdg2beeblebrox::visit(STSuspend &s)
{
  if(debug) cerr<<"Suspend\n";
  flow_or_parents(&s);
  return Status();  
}

Status pdg2beeblebrox::suspend_sm(STSuspend &s)
{
  wire *flowin;

  vector<wire *> succ_enters;
  wire *wcancel, *realsuspend;  

  if(debug) cerr<<"suspend_sm\n";

  flowin = or_flow[&s];

  for(vector<GRCNode*>::iterator is = s.dataSuccessors.begin(); is != s.dataSuccessors.end(); is++){
    succ_enters.push_back(or_flow[*is]);
  }
  wcancel = current -> build_orgate(succ_enters);
  realsuspend = current-> build_andgate( flowin, current->build_notgate(wcancel)); 

  suspends[s.st].push_back(realsuspend);

  return Status();
}


Status pdg2beeblebrox::visit(Test &s)
{
  wire *flowin;
  wire *t, *nt;
  
  GRCNode *ch;

  if(debug) cerr<<"Test\n";

  flowin = flow_or_parents(&s);

  bc.flow = flowin; // for suspending counters
  t = bc.getbool(s.predicate);


  ch = s.successors[1];
  if(ch) flow_add_arc(&s, ch, current-> build_andgate(flowin, t));

  ch = s.successors[0];
  if(ch) {
    nt = current ->build_notgate(t);
    flow_add_arc(&s, ch, current-> build_andgate(flowin, nt));
  }

  return Status();
}

Status pdg2beeblebrox::visit(Terminate &s)
{
  if(debug) cerr<<"Terminate\n";

  if(s.code == 1) return Status();
  flow_or_parents(&s); 

  // does nothing, sync will look for flow

  return Status();
}

// very carefull ...

Status pdg2beeblebrox::visit(Sync &s)
{

  Terminate *t;
  int nch, nthr, nt0, i;
  vector<vector<wire*> > flowin_v;
  wire *whilev, *wc, *w, *wnhilev;

  if(debug) cerr<<"Sync\n";
 
  // take flow from data predecessors ...

  nch = s.successors.size();
  flowin_v.resize(nch);

  nthr = s.st->children.size();
  nt0=0;
  for(vector<GRCNode*>::iterator ip = s.dataPredecessors.begin();
      ip != s.dataPredecessors.end(); ip++){
    t=dynamic_cast<Terminate*>(*ip);
    assert(t);
    assert(t->code >= 0 && t->code < nch);
    if(t->code == 0) nt0++;
    flowin_v[t->code].push_back(or_flow[t]);
  }

  whilev = w_0;
  for(i=nch-1; i>=0; i--){
    if(i==1) continue;
    if(i==0 && nt0 != nthr) continue;
    wc = i ? current-> build_orgate(flowin_v[i]) :
      current->build_andgate(flowin_v[i]);

    if(s.successors[i]){
      wnhilev = current-> build_notgate(whilev);
      w = current-> build_andgate(wc, wnhilev);
      flow_add_arc(&s, s.successors[i], w);
    }

    whilev = current->build_orgate(wc, whilev);

  }

  return Status();
}

Status pdg2beeblebrox::visit(Fork &s)
{
  if(debug) cerr<<"Fork\n";

  flow_all_children( &s, flow_or_parents(&s));

  return Status();
}

Status pdg2beeblebrox::visit(Action &s)
{
  wire *flowin;

  if(debug) cerr<<"Action\n";

  flowin = flow_or_parents(&s);
  bc.do_statement(s.body, flowin);
  flow_all_children(&s, flowin);

  return Status();
}

void Usage()
{
  cerr<<
    "Usage: cec-pdgblif\n"
    " pdg (stdin) -> blif (stdout)\n"
    "  --dot <filename>     outputs the dot circuit\n"
    "  --sm <filename>      generates the default encoding\n"
    "  --ff                 builds ff - debuging\n"
    "  --verilog <filename> makes the verilog wrapper\n"
    "  --xilinx     -NOT SUPP YET -        makes a xilinx wrapper, implies verilog\n"
    "  --testbench  -NOT SUPP YET -        makes the testbench, implies verilog\n";
  exit(-1);
}

void IOError(char *fname)
{
  cerr<<"Can't open "<<fname<<"\n";
  exit(-1);
}

int main(int argc, char *argv[])
{

  beeble_options opt;

  istream &pdgfile(std::cin);
  ostream &bliffile(std::cout);

  --argc; ++argv;
  while(argc > 0 ){

    if(!strcmp(argv[0], "--dot")){
      --argc; ++argv;
      opt.gendot = true;
      opt.dotfile.open(argv[0]); if(!opt.dotfile.good()) IOError(argv[0]);
    } else if(!strcmp(argv[0], "--sm")){
      --argc; ++argv;
      opt.gensm = true;
      opt.smfile.open(argv[0]); if(!opt.smfile.good()) IOError(argv[0]);
    } else if(!strcmp(argv[0],"--ff")){
      opt.ff = true;
    } else if(!strcmp(argv[0], "--verilog")){
      --argc; ++argv;
      opt.genver = true;
      opt.vfile.open(argv[0]); if(!opt.vfile.good()) IOError(argv[0]);
    } else if(!strcmp(argv[0], "--xilinx")){
      opt.genxilinx = true;
    } else if(!strcmp(argv[0], "--testbench")){
      opt.gentb = true;
    } else {
      Usage();
    }
    --argc; ++argv;
  }

  IR::XMListream xml_stream(pdgfile);
  IR::Node *xml_rt;
  xml_stream >> xml_rt;

  Modules *mods = dynamic_cast<Modules*>(xml_rt);
  assert(mods);
  assert(mods->modules.size() == 1); // only one module
  Module *mod = mods->modules[0];

  CFGmap cfgmap; STmap stmap;
  AST::GRCgraph *g = dynamic_cast<AST::GRCgraph*>(mod->body); assert(g);
  g->enumerate(cfgmap, stmap);

  pdg2beeblebrox beeb (mod, bliffile, opt, cfgmap, stmap);
  beeb.execute();

  if(opt.gensm) opt.smfile.close();
  if(opt.gendot) opt.dotfile.close();
  if(opt.genver) opt.vfile.close();


  return 0;
}
