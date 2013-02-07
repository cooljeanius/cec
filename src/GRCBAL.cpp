#include "GRCBAL.hpp"
#include <cassert>
#include <stdio.h>

namespace GRCBAL {

  std::list<SGRNode *> SGRNode::allNodes;
 
  /* ********************************************************************* */
  
  void BALDP::drawGraph(nodeVector graph){

    for(nodeVector::iterator i = graph.begin(); i != graph.end(); i++)
      if (*i) drawComponent(*i);
  }
  
  void BALDP::drawComponent(SGRNode *n){
    assert(n);

    // draw the node
    if(reached.find(n) == reached.end()){ // not yet visited
      reached.insert(n);

      int mynum = n->nnum;
      
      // o << 'n' << mynum << ' ';
      n->printDot(o, mynum);


      bool branch = (n->children.size() > 1);

      // draw the links from the parent to the child
      for(nodeVector::iterator j = n->children.begin() ;
	  j != n->children.end(); j++){
	if(*j){
	  o << 'n' <<  mynum << " -> n" << (*j)->nnum;
	  
	  if(dynamic_cast<SwitchThreadNode*>(n)){
	    o << " [style=dotted]";
	  }
	  else if(branch){
	    o << " [taillabel= \""<< (j - n->children.begin()) <<"\"]";
	  }

	  o << "\n";
	}
      }
      

      // visit children
      for(nodeVector::iterator j = n->children.begin() ;
	  j != n->children.end() ; j++){
	if(*j) drawComponent(*j);
      }

      // visit parents
      for(nodeVector::iterator j = n->parents.begin() ;
	  j != n->parents.end() ; j++){
	if(*j) drawComponent(*j);
      }
    }
  }


  /* ********************************************************************** */
  
  void drawDot(std::ostream &o, nodeVector graph1,  nodeVector graph2,
	       list_of_lists &context_switch_points,
	       Module *m)
  {
    BALDP visitor(o);
    //    visitor.drawstlink = drawstlink;
    //    visitor.clean = clean;

    o << "digraph " << m->symbol->name << "  {" << std::endl;
    o << "size=\"7.5,10\"\n";

    o << "{ rank=same";
    for(nodeVector::iterator i = graph1.begin(); i != graph1.end(); i++)
      if (*i) o << " n" << (*i)->nnum;
    for(nodeVector::iterator i = graph2.begin(); i != graph2.end(); i++)
      if (*i) o << " n" << (*i)->nnum;
    o << "}\n";
   
    visitor.drawGraph(graph1);
    visitor.drawGraph(graph2);

    o << "/* Context switches */ \n";
    for ( list_of_lists::const_iterator i = context_switch_points.begin() ;
	  i != context_switch_points.end() ; i++ ) {
      const std::list<int> &nodes = *i;

      o << "{rank=same";
      for (std::list<int>::const_iterator j = nodes.begin() ;
	   j != nodes.end () ; j++) {
	o << " n" << *j;
      }
      o << "}\n";
    }

    o << "}" << std::endl;
  }

  void convertGRC(std::ostream &o, GRCgraph *g, Module *m,
		  bool dumpDot, bool debug){
    assert(g);
    assert(m);
    assert(m->symbol);

    CFGmap cfgmap;
    STmap stmap;
    g->enumerate(cfgmap, stmap);

    GRC2BAL converter(o);    
    converter.assignSchedule(g->control_flow_graph);
    converter.assignThreads(g->control_flow_graph);
    converter.removeConcurrency(debug, cfgmap);

    SGRNode::removeNOPs();

    if (dumpDot) {
      drawDot(o, converter.threads, converter.dead_threads,
	      converter.context_switch_points, m);
      return;
    }

    STexcl *excl = dynamic_cast<STexcl*>(g->selection_tree);
    assert(excl);
    assert(!excl->children.empty());
    int initial_state = excl->children.size() - 1;
    o << "BEGIN MODULE" << std::endl;
    converter.printBAL(m, stmap, excl, initial_state);
    o << "END MODULE" << std::endl;
  }

  void SGRNode::removeNOPs() {
    for (std::list<SGRNode *>::iterator i = allNodes.begin() ;
	 i != allNodes.end() ; i++) {
      SGRNode *n = *i;
      for (unsigned int c = 0 ; c < n->children.size() ; ++c) {
	NOPNode *nn = dynamic_cast<NOPNode *>(n->children[c]);
	if (nn && nn->children.size() == 1) {
	  SGRNode *new_successor = nn->children[0]; /* node after the NOP */
	  assert(new_successor);
	  n->children[c] = new_successor; /* set the child */
	  /* Patch the parents of the destination */
	  for (unsigned int d = 0 ; d < new_successor->parents.size() ; d++) {
	    if (new_successor->parents[d] == nn)
	      new_successor->parents[d] = n;
	  }
	}
      }
    }
  }

  /* ******************************************************************** */

  /*
   *  Class: GRC2BAL
   *  Purpose: convert the GRC of ESTERL to BAL
   */
  
  void GRC2BAL::assignThreads(GRCNode *g) {
    if(tnum.find(g) == tnum.end()){ 
      // first node and no thread has been assigned
      tnum[g] = num_threads++;
    }
    myth = tnum[g];
    
    bool assign_thread = true;
    for(vector<GRCNode *>::iterator i = g->predecessors.begin();
	i != g->predecessors.end(); i++){
      assign_thread &= (tnum.find(*i) != tnum.end());
    }
    /* 
       for(vector<GRCNode *>::iterator i = schedule.begin(); 
       i != schedule.end();  i++)
       if((*i) == g)
       std::cout<<"visited " << (i - schedule.begin()) << " t: ";
       std::cout<<myth<<" a? " << assign_thread<< std::endl;
    */
    if(assign_thread){
      if (dynamic_cast<Sync*>(g) != NULL){
	joinThreadNum(g);
      }
      else if (dynamic_cast<Fork*>(g) != NULL){
	forkThreadNum(g);
      }
      else{
	genThreadNum(g);
      }
    }
  }

  void GRC2BAL::joinThreadNum(GRCNode *n){ // join statement
    myth = num_threads; // find the min of all the predecessors for my thread
    for ( vector<GRCNode*>::iterator j = n->predecessors.begin();
	  j != n->predecessors.end() ; j++ ) {
      if (*j) { // parent exists
	if (myth > tnum[*j]) myth = tnum[*j];
      }
    }
    //     std::cout<<"JOIN t: "<< myth<<std::endl;
    tnum[n] = myth;
     
    // now consider the children
    genThreadNum(n);
  }

  void GRC2BAL::forkThreadNum(GRCNode *n){ // fork statement
    // num_threads incrememented when no new threads are created.
    // seems like values are coming through this function multiple times
    GRCNode *fc = n->successors.front();
    if(tnum.find(fc) != tnum.end()) return;
    tnum[fc] = myth;

    for ( vector<GRCNode*>::iterator j = n->successors.begin()++;
	  j != n->successors.end() ; j++ ) {
      if ( (*j) != fc) {
	myth = num_threads++;
	//	std::cout<< myth << " ";
	tnum[*j] = myth;
      }
    }
    //   std::cout<<std::endl;
    // go through this node's successors and assign them different thread
    for ( vector<GRCNode*>::iterator j = n->successors.begin();
	  j != n->successors.end() ; j++ ) {
      if (*j) {
	// the child exists assign it a thread number 
	assignThreads(*j);
      }
    }
  }

  void GRC2BAL::genThreadNum(GRCNode *g){ // everything else
    // go through this node's successors and assign them my thread
    for ( vector<GRCNode*>::iterator j = g->successors.begin();
	  j != g->successors.end() ; j++ ) {
      if (*j) {
	// the child exists assign it a thread number
	tnum[*j] = tnum[g];
	assignThreads(*j);
      }
    }
  }

  void GRC2BAL::assignSchedule(GRCNode *g){
    calculateSchedule(g, schedule);
  }
  
  int GRC2BAL::findSchedPos(GRCNode *g){
    for (unsigned int j = 0; j < schedule.size() ; j++ ) {
      if(schedule.at(j) == g){
	return j;
      }
    }
    return -1;
  }
  
  
  Status GRC2BAL::visit(DefineSignal &s){
    currentNode = new DSNode(s.signal, s.is_surface);
    return Status();
  }
  Status GRC2BAL::visit(Nop &s){
    currentNode = new NOPNode();
    return Status();
  }
  Status GRC2BAL::visit(ExitGRC &s){
    currentNode = new ExitGRCNode();
    return Status();
  }
  Status GRC2BAL::visit(EnterGRC &s){
    currentNode = new EnterGRCNode();
    return Status();
  }
  Status GRC2BAL::visit(Enter &s){
    currentNode = new EnterNode();
    return Status();
  }
  Status GRC2BAL::visit(Action &s){
    currentNode = new ActionNode(s.body);
    return Status();
  }
  Status GRC2BAL::visit(Fork &s){
    currentNode = new ForkNode();
    return Status();
  }
  Status GRC2BAL::visit(Sync &s){
    currentNode = new JoinNode();
    return Status();
  }
  Status GRC2BAL::visit(Terminate &s){
    assert(s.successors.size() == 1);
    currentNode = new TermNode(s.code, dynamic_cast<Sync *>(s.successors[0]));
    return Status();
  }
  Status GRC2BAL::visit(Test &s){
    currentNode = new TWBNode(s.predicate);
    return Status();
  }
  Status GRC2BAL::visit(Switch &s){
    currentNode = new MWBNode();
    return Status();
  }
  
  Status GRC2BAL::visit(STSuspend &s){
    currentNode = new SuspendNode();
    return Status();
  }

  void GRC2BAL::testForks(){
    //test the GRC Forks to see why new ap are made when not necessary
    int pos = 0;
    for(vector<GRCNode *>::iterator i = schedule.begin(); i != schedule.end();
	i++){
      Fork * f = dynamic_cast<Fork *> (*i);
      if(f != NULL){
	std::cout<<pos<<" "<< tnum[f]<<"\t";
	for(vector<GRCNode *>::iterator j = f->successors.begin();
	    j != f->successors.end(); j++){
	  std::cout<<tnum[(*j)]<<" ";
	}
	std::cout<<std::endl;
      }
      pos++;
    }
  }

  void GRC2BAL::removeConcurrency(bool debug, CFGmap cfgmap) {
    if (debug) std::cout << num_threads << " threads total" << std::endl;    

    // The most recent SGRNode for each GRCNode

    map<GRCNode *, SGRNode *> new_node;

    // Set of nodes that are about to run in each thread
    // These are the points where context switching nodes will be added

    set<SGRNode *> about_to_run[num_threads];

    assert(schedule.size() > 1);
    assert(schedule[1]);

    // Create a stub for the initial thread

    InitThreadsNode * root = new InitThreadsNode();
    root->nnum = nextnum++;
    root->tnum = 0;
    root->GRCme = schedule[1];
    new_node[schedule[1]] = root;    

    // Initial state is the last child under the topmost switch
    initial_state = schedule[1]->successors.size() - 1;

    threads.resize(num_threads);
    dead_threads.resize(num_threads);

    threads[0] = root;
    about_to_run[0].insert(root);
    dead_threads[0] = NULL; // Thread 0 is never dead

    // Go through the nodes in scheduled order

    int last_thread = 0;
    for (vector<GRCNode *>::iterator k = ++schedule.begin() ;
	 k != schedule.end() ; k++) {
      GRCNode * node = *k; // GRC node being added to the graph
      assert(node);
      int thread = tnum[node];  // number of current thread

      if (debug)
	std::cout << "** Synthesizing node " << cfgmap[node]
		  << " in thread " << thread << std::endl;
     
      if ( thread != last_thread ) {

	// Context switch       

	if (debug)
	  std::cout << "Context switch from " << last_thread << " to "
		    << thread << std::endl;

	context_switch_points.push_back(std::list<int>());

	// To each node that was about to run in the suspending thread,
	// attach a context-switching node. These become the new
	// about_to_run nodes for the thread.

	set<SGRNode *> last_running = about_to_run[last_thread];
	about_to_run[last_thread].clear();

	for (set<SGRNode *>::iterator a = last_running.begin() ;
	     a != last_running.end() ; a++) {
	  SGRNode * abr = *a;
	  assert(abr);

	  context_switch_points.back().push_back(nextnum);

	  SGRNode * sw = new SwitchThreadNode(thread);
	  sw->tnum = last_thread;
	  sw->nnum = nextnum++;
	  sw->GRCme = abr->GRCme;
	  sw->parents.push_back(abr);
	  abr->children.push_back(sw);


	  about_to_run[last_thread].insert(sw);
	}
	       	
	// To each "about_to_run" node in the newly-activated thread,
	// attach an ActivePoint.  These become the new about_to_run nodes
	// for this thread.
	
	last_running = about_to_run[thread];
	about_to_run[thread].clear();
	for (set<SGRNode *>::iterator a = last_running.begin() ;
	     a != last_running.end() ; a++) {
	  SGRNode * abr = *a;
	  assert(abr);

	  context_switch_points.back().push_back(nextnum);
	  
	  SGRNode * ap = new ActivePoint();
	  ap->tnum = thread;
	  ap->nnum = nextnum++;
	  ap->GRCme = abr->GRCme;
	  ap->parents.push_back(abr);
	  abr->children.push_back(ap);
	  
	  about_to_run[thread].insert(ap);
	  new_node[abr->GRCme] = ap;
	  if (debug)
	    std::cout << "Created an ActivePoint for node "
		      << cfgmap[abr->GRCme] << std::endl;
	}
	
	last_thread = thread;
      }

      // Synthesize an SGRNode from the GRCNode

      node->welcome(*this);
      SGRNode * snode = currentNode;
      snode->nnum = nextnum++;
      snode->GRCme = node;
      snode->tnum = thread;

      // Connect the new SGRNode for the node to the stub we
      // should have created for it earlier (a successor of a node earlier in
      // the schedule, an activation point when this thread resumed, etc.)

      assert(new_node.find(node) != new_node.end());
      SGRNode * stub = new_node[node];
      assert(stub);
      stub->children.push_back(snode);
      snode->parents.push_back(stub);

      // We are now running this node so remove it from the "about to run" set

      assert(about_to_run[thread].find(stub) != about_to_run[thread].end());
      about_to_run[thread].erase(about_to_run[thread].find(stub));

      // Attach this node's successors

      for ( vector<GRCNode *>::const_iterator i =
	      node->successors.begin() ;
	    i != node->successors.end() ; i++ ) {
	GRCNode * old_successor = *i;
	if ( !old_successor ) {

	  // Null successor: connect it to a NOP node and forget about it

	  SGRNode * new_successor = new NOPNode();
	  new_successor->nnum = nextnum++;
	  new_successor->tnum = thread;
	  new_successor->GRCme = NULL;
	  snode->children.push_back(new_successor);
	  new_successor->parents.push_back(snode);

	} else {
	  int starting_thread = tnum[old_successor];

	  if (starting_thread != thread && dynamic_cast<Fork *>(node)) {

	    if (debug)
	      std::cout << "Starting thread " << starting_thread
			<< std::endl;

	    ForkNode * fn = dynamic_cast<ForkNode *>(snode);
	    assert(fn);
	    fn->add(starting_thread);
	      
	    // This thread is starting
	      
	    // Create two roots for the thread:
	    // An active one and a dead one
	      
	    SGRNode * running = new ActivePoint();
	    running->nnum = nextnum++;
	    running->GRCme = old_successor;
	    running->tnum = thread;

	    SGRNode * dead = new ActivePoint();
	    dead->nnum = nextnum++;
	    dead->GRCme = 0;
	    dead->tnum = thread;
	      
	    threads[starting_thread] = running;
	    dead_threads[starting_thread] = dead;

	    // Make sure this thread is started at the beginning of
	    // the program

	    root->add(starting_thread);
	     	      
	    about_to_run[starting_thread].insert(running);
	    about_to_run[starting_thread].insert(dead);
	    new_node[old_successor] = running;
	    if (debug)
	      std::cout << "Created a thread root of node "
			<< cfgmap[old_successor] << std::endl;
	  } else if ( starting_thread != thread && dynamic_cast<Sync *>(old_successor) ) {

	    // The last node in a thread: turn it into NOP and make sure
	    // we context switch correctly

	    SGRNode * new_successor = new NOPNode();
	    new_successor->tnum = thread;
	    new_successor->nnum = nextnum++;
	    new_successor->GRCme = NULL;
	    snode->children.push_back(new_successor);
	    new_successor->parents.push_back(snode);
	    about_to_run[thread].insert(new_successor);
	    if (debug)
	      std::cout << "marking " << cfgmap[new_successor->GRCme]
			<< " as about_to_run for terminating thread " << thread
			<< std::endl;

	  } else {
	    
	    // A normal node: create a new NOP node if there
	    // isn't already something and connect it up
	   
	    if (new_node.find(old_successor) == new_node.end()) {
	      if (debug) std::cout << "created a new NOP successor" << std::endl;
	      SGRNode * new_successor =  new NOPNode();
	      new_successor->nnum = nextnum++;
	      new_successor->GRCme = old_successor;
	      new_successor->tnum = thread;
	      new_node[old_successor] = new_successor;
	      if (debug)
		std::cout << "Created a NOP for node "
			  << cfgmap[old_successor]
			  << " in thread " << thread << std::endl;
	      
	    }
	    SGRNode * new_successor = new_node[old_successor];
	    snode->children.push_back(new_successor);
	    new_successor->parents.push_back(snode);
	    
	    // Remember that this node is ready-to-run
	    about_to_run[thread].insert(new_successor);
	    if (debug) std::cout << "marking " << cfgmap[new_successor->GRCme]
				 << " as about_to_run for thread " << thread
				 << std::endl;
	  }
	}
      }	 
    }
  }

  void DSNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=house orientation=90 label=\"p:"<<n<<" t:"<<tnum<<" DefSig "<< signal->name <<"\" ]\n";
  }
  void SwitchThreadNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=plaintext label=\"p:"<<n<<" t:"<<tnum<<" t':"<<switchThread<<" x\" ]\n";

  }
  void ActivePoint::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=point ]\n";
  }
  void TWBNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=diamond label=\"p:"<<n<<" t:"<<tnum<<" SigBrch\" ]\n";
  }
  void MWBNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=diamond peripheries=2 label=\"p:"<<n<<" t:"<<tnum<<" Switch\" ]\n";
  }
  void NOPNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [color=pink style=filled shape=plaintext label=\"p:"<<n<<" t:"<<tnum<<" NOP\" ]\n";
  }
  void ActionNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=house orientation=270 label=\"p:"<<n<<" t:"<<tnum<<" Action\" ]\n";
  }
  void JoinNode::printDot(std::ostream &o, int n){
    
    o << 'n' << n << " [ shape=invtriangle label=\"p:"<<n<<" t:"<<tnum<<" Join ";
    o << "\" ]\n";
  }

  void ForkNode::printDot(std::ostream &o, int n){

    o << 'n' << n << " [ shape=triangle label=\"p:"<<n<<" t:"<<tnum<<" Fork";
    for(vector<int>::iterator i = startthreads.begin();
	i != startthreads.end(); i++)
      o << " " << (*i);
    o << "\" ]\n";

  }
  void ExitGRCNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=ellipse label=\"p:"<<n<<" t:"<<tnum<<" ExitGRCNode\" ]\n";
  }
  void EnterGRCNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=ellipse label=\"p:"<<n<<" t:"<<tnum<<" EnterGRCNode\" ]\n";
  }
  void EnterNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=house label=\"p:"<<n<<" t:"<<tnum<<" Enter\" ]\n";
  }
  void TermNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=octagon label=\"p:"<<n<<" t:"<<tnum<<" Term\" ]\n";
  }
  void InitThreadsNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=triangle label=\"p:"<<n<<" t:"<<tnum<<" Init";
    for(vector<int>::iterator i = startthreads.begin();
	i != startthreads.end(); i++)
      o << " " << (*i);
    o << "\" ]\n";
  }
  void SuspendNode::printDot(std::ostream &o, int n){
    o << 'n' << n << " [ shape=egg label=\"Suspend\" ]\n";
  }

  /* ***************************** BAL Printers **************************** */

  void GRC2BAL::printBAL(Module *m, STmap stmap, STexcl *root,
			 int initial_state ) {

    SymbolTable * signals = m->signals;
    SymbolTable * constants = m->constants;
    SymbolTable * variables = m->variables;

    sig_num = 0;
    state_num = 0;
    join_num = 0;
    var_num = 0;

    signal_regs.clear();
    state_regs.clear();
    term_regs.clear();
    var.clear();
    valued_signals.clear();

    // Report the INITIAL STATE

    o << "INITIAL STATE " << initial_state << std::endl;

    o << "CONSTANTS" << std::endl;
    for ( SymbolTable::const_iterator i = constants->begin();
	  i != constants->end() ; i++ ) {
      ConstantSymbol *cs = dynamic_cast<ConstantSymbol*>(*i);
      if(cs && !cs->initializer) {
	char var_name[15];
	sprintf(var_name, "%i", var_num);
	o << var_num << " " << cs->name << " ; constant" << std::endl;
	var[cs] = var_name;
	var_num++;
      }
    }

    // Signals

    o << "INPUT SIGNALS" << std::endl;
    for ( SymbolTable::const_iterator i = signals->begin() ;
	  i != signals->end() ; i++ ) {
      SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
      if (s->name != "tick" && s->kind == SignalSymbol::Input) {
	char sig_name[15];
	sprintf(sig_name, "%i", sig_num);
	sig_num++;
	signal_regs[s] = sig_name;
	o <<"; " << sig_name << " " << s->name;
	// Add a variable if this signal is a valued signal
	// Only supporting integers and boolean as of now
	if(s->type && (s->type->name == "integer" ||
		       s->type->name == "boolean")) {
	  char var_name[15];
	  sprintf(var_name, "%i", var_num);
	  valued_signals[s] = var_name;
	  o << " : associated variable: " << var_num;
	  var_num++;
	}
	o <<"\n";
      }
    }

    o << "INPUTOUTPUT SIGNALS" << std::endl;
    for ( SymbolTable::const_iterator i = signals->begin() ;
	  i != signals->end() ; i++ ) {
      SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
      if (s->name != "tick" && s->kind == SignalSymbol::Inputoutput) {
	char sig_name[15];
	sprintf(sig_name, "%i", sig_num);
	sig_num++;
	signal_regs[s] = sig_name;
	o <<"; " << sig_name << " " << s->name;
	// Add a variable if this signal is a valued signal
	// Only supporting integers and boolean as of now
	if(s->type && (s->type->name == "integer" ||
		       s->type->name == "boolean")){
		char var_name[15];
		sprintf(var_name, "%i", var_num);
		valued_signals[s] = var_name;
		o << " : associated variable: " << var_num;
		var_num++;
	}
	o <<"\n";
      }
    }
    
    o << "OUTPUT SIGNALS" << std::endl;
    for ( SymbolTable::const_iterator i = signals->begin() ;
	  i != signals->end() ; i++ ) {
      SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
      if (s->kind == SignalSymbol::Output) {
	char sig_name[15];
	sprintf(sig_name, "%i", sig_num);
	sig_num++;
	signal_regs[s] = sig_name;
	o <<"; " << sig_name << " " << s->name;
	// Add a variable if this signal is a valued signal
	// Only supporting integers and boolean as of now
	if(s->type && (s->type->name == "integer" ||
		       s->type->name == "boolean")){
		char var_name[15];
		sprintf(var_name, "%i", var_num);
		valued_signals[s] = var_name;
		o << " : associated variable: " << var_num;
		var_num++;
	}
	o <<"\n";
      }
    }

    o << "LOCAL SIGNALS" << std::endl;
    for ( SymbolTable::const_iterator i = signals->begin() ;
	  i != signals->end() ; i++ ) {
      SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
      if (s->kind == SignalSymbol::Local) {
	char sig_name[15];
	sprintf(sig_name, "%i", sig_num);
	sig_num++;
	signal_regs[s] = sig_name;
	o <<"; " << sig_name << " " << s->name;
	// Add a variable if this signal is a valued signal
	// Only supporting integers and boolean as of now
	if (s->type &&
	    (s->type->name == "integer" || s->type->name == "boolean")) {
	  if (s->reincarnation) {
	    string var_name = valued_signals[s->reincarnation];
	    valued_signals[s] = var_name;
	      o << " : associated variable: " << var_name;
	  } else {	    
	    char var_name[15];
	    sprintf(var_name, "%i", var_num);
	    valued_signals[s] = var_name;
	    o << " : associated variable: " << var_num;
	    var_num++;
	  }
	}
	o <<"\n";
      }
    }

    o << "TRAPS\n";

    for ( SymbolTable::const_iterator i = signals->begin() ;
	  i != signals->end() ; i++ ) {
      SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
      if (s->kind == SignalSymbol::Trap) {
	char sig_name[15];
	sprintf(sig_name, "%i", sig_num);
	sig_num++;
	signal_regs[s] = sig_name;
	std::cout<<"; " << sig_name << " " << s->name;
	// Add a variable if this is a valued trap
	// Only supporting integers and boolean as of now
	if (s->type && (s->type->name == "integer" ||
			s->type->name == "boolean")) {
	  char var_name[15];
	  sprintf(var_name, "%i", var_num);
	  valued_signals[s] = var_name;
	  o << " : associated variable: " << var_num;
	  var_num++;
	}
	std::cout<<"\n";
      }
    }


    o << "VARIABLES" << std::endl;

    // Add all the variables in the var array
    for ( SymbolTable::const_iterator i = variables->begin();
	  i != variables->end() ; i++ ) {
      VariableSymbol *vs = dynamic_cast<VariableSymbol*>(*i);
      if(vs != NULL){
	char var_name[15];
	sprintf(var_name, "%i", var_num);
	o << var_num << " " << vs->name << std:: endl;
	var[vs] = var_name;
	var_num++;
      }
    }

    o << "COUNTERS" << std::endl;

    // Add all the counters to the variable array
    for ( vector<Counter*>::const_iterator i = m->counters.begin();
	  i != m->counters.end() ; i++ ) {
      assert(*i);
      char var_name[15];
      sprintf(var_name, "%i", var_num);
      counters[*i] = var_name;
      o << var_num << " counter\n";
      var_num++;
    }

    o << "STATES" << std::endl;

    state_regs[root] = "0";
    ++state_num;

    // print state definitions
    for ( STmap::iterator i = stmap.begin() ; 
	  i != stmap.end() ; i++ ) {
      STexcl *e = dynamic_cast<STexcl*>((*i).first);
      if (e && e != root) {
	char state_name[15];
	sprintf(state_name, "%i", state_num);
	state_num++;
	state_regs[e] = state_name;
	o << "; " << state_name << " " << stmap[e] << std::endl;
	
      }
    }

    o << "PARALLEL SYNCHRONIZERS" << std::endl;

    // get the term variables
    visited.clear();
    join_num = 0;
    for (nodeVector::iterator i = threads.begin(); i != threads.end(); i++)
      enumerateJoins(*i);


    o <<
      "; TOTALS" "\n" 
      "; " << sig_num << " signals" "\n"
      "; " << state_num << " state variables" "\n"
      "; " << join_num << " joins" "\n"
      "; " << var_num << " registers" "\n"
      "; " << num_threads << " threads" "\n"
      "; " << context_switch_points.size() << " context switch points" "\n"
      ;

    if (sig_num > 255)
      throw IR::Error("Implementation limit: program requires more than 255 signals");

    if (state_num > 255)
      throw IR::Error("Implementation limit: program requires more than 255 state variables");

    if (join_num > 255)
      throw IR::Error("Implementation limit: program requires more than 255 joins");

    if (var_num > 255)
      throw IR::Error("Implementation limit: program requires more than 255 registers");


    if (num_threads > 255)
      throw IR::Error("Implementation limit: program requires more than 255 threads");

    // print threads

    visited.clear();
    node_label.clear();

    next_label = 0;
    for( int thread = 0 ; thread < num_threads ; thread++ ) {
      if (threads[thread]) {
	// print thread body, starting with its entry point label
	o << "thread_" << thread << ":\t\n";
	assignLabels(threads[thread]);
	printDFS(threads[thread]);
      }

      if (dead_threads[thread]) {
	// print thread body, starting with its entry point label
	o << "dead_thread_" << thread << ":\t\n";
	assignLabels(dead_threads[thread]);
	printDFS(dead_threads[thread]);
      }
    }

  }

  // Do a DFS of the graph, giving nodes label numbers

  void GRC2BAL::assignLabels(SGRNode *n) {
    if (node_label.find(n) != node_label.end()) return;
    node_label[n] = next_label++;
    for(nodeVector::iterator i = n->children.begin();
	i != n->children.end(); i++)
      if (*i) assignLabels(*i);
  }

  // Do a DFS of the graph, associating each Join node with a number
  void GRC2BAL::enumerateJoins(SGRNode *n) {
    if (visited.find(n) != visited.end()) return;
    visited.insert(n);
    JoinNode *j = dynamic_cast<JoinNode *>(n);
    if (j) {
      Sync *s = dynamic_cast<Sync *>(j->GRCme);
      assert(s);
      char join_name[15];
      sprintf(join_name, "%i", join_num++);
      term_regs[s] = join_name;
    }
    for(nodeVector::iterator i = n->children.begin();
	i != n->children.end(); i++)
      if (*i) enumerateJoins(*i);
  }

  void GRC2BAL::printDFS(SGRNode *n) {

    visited.insert(n);
    int my_label = node_label[n];
	o << "L" << my_label << ":";

    n->welcome(*this);
    o << std::endl;

    if (n->children.size() == 1 &&
	node_label[n->children.at(0)] != my_label + 1) {
      o << "\t JMP L" << node_label[n->children.at(0)] << "\n";
      return;
    }

    for(nodeVector::iterator i = n->children.begin();
	i != n->children.end(); i++)
      if (*i && visited.find(*i) == visited.end())
	printDFS(*i);
  }

  // multiple line statements - create predicate
  Status TWBNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(TWBNode &n){
    generateBAL(n.predicate);
    o << "\t BNZ ";

    o << "L" << node_label[n.children.at(1)];
    if (node_label[n.children.at(0)] != node_label[&n] + 1)
      o << "\n\t JMP L" << node_label[n.children.at(0)]; /* FIXME? */

    o << "\t;" << n.nnum << " \n" ;//<< sigexp->signal->name << "\n";
    
    return Status();
  }
  Status MWBNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(MWBNode &n){
    o << "\t BST ";
    Switch * s = dynamic_cast<Switch *> (n.GRCme);
    assert(s);   
    STexcl * ste = dynamic_cast<STexcl *> (s->st);
    assert(ste);
    o << state_regs[ste] << " ";
    for(nodeVector::iterator i = n.children.begin();
	i != n.children.end(); i++) {
      o << "L" << node_label[*i] << " ";
    }
    o << "\t;" << n.nnum << " r:" << state_regs[ste];

    return Status();
  }
  Status JoinNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(JoinNode &n) {

    assert(n.GRCme);
    Sync * s = dynamic_cast<Sync *>(n.GRCme);
    assert(s);
    o << "\t BCC " << term_regs[s] << " " ;

    for(nodeVector::iterator i = n.children.begin();
	i != n.children.end(); i++) {
      if (*i) {
	o << "L" << node_label[*i] << " ";
      } else {
	o << "done"; /* dynamically unreachable */
      }
    }

    o << "\t;" << n.nnum;
    return Status();
  }
  
  Status EnterGRCNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(EnterGRCNode &n){
    o << "\nstart:";
    return Status();
  }

  // single line statements
  Status DSNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(DSNode &n) {
    if (n.signal->initializer && n.is_surface) {
      generateBAL(n.signal->initializer);
      o << "\t STR " << valued_signals[n.signal] << " ; Initializing Value\n";
    }
    o << "\t RST " << signal_regs[n.signal] << " ; " << n.signal->name;
    return Status();
  }
  Status SwitchThreadNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(SwitchThreadNode &n){
    o << "\t SWC " << n.switchThread;
    return Status();
  }
  Status ActivePoint::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(ActivePoint &n){
    //    o << std::endl;
    return Status();
  }
  Status NOPNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(NOPNode &n){
    o << "; NOP";
    return Status();
  }
  Status ActionNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(ActionNode &n){
	
    Emit * e = dynamic_cast<Emit *> (n.body);
    if (e) {
      if(e->value){
	// Generate the BAL for the expression
	generateBAL(e->value);
	// And store in the register corresponding to the value of the signal
	o << "\t STR " << valued_signals[e->signal] << "; Loading Value\n" ;
      }

      o << "\t EMIT " << signal_regs[e->signal]
	<< " ; " << e->signal->name;
      return Status();
    }

    Assign * as = dynamic_cast<Assign *> (n.body);
    if (as) {
      generateBAL(as);
      return Status();	
    }
	
    StartCounter* sc = dynamic_cast<StartCounter*>(n.body);
    if (sc) {
      generateBAL(sc);
      return Status();
    }

    Exit* ex = dynamic_cast<Exit*>(n.body);
    if (ex != NULL) {
      assert(ex->trap);
      if (ex->value) {
	generateBAL(ex->value);
	o << "\t STR " << valued_signals[ex->trap] << "; Loading Value\n" ;
      }
      o << "\tEMIT " << signal_regs[ex->trap] << " ; " << ex->trap->name;
      return Status();
    }

    // If we reached here, there is something wrong!
    //assert(0);

    return Status();
  }
  Status ForkNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(ForkNode &n){
    for(vector<int>::iterator i = n.startthreads.begin();
	i != n.startthreads.end(); i++)
      o << "\n\tSTRT " << (*i) << " thread_" << (*i);
    return Status();
  }
  Status ExitGRCNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(ExitGRCNode &n){
    o << "\ndone:\n";
    o << "\t TICK";
    return Status();
  }
  Status EnterNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(EnterNode &n){ // Fix
    STexcl *e = 0;
    Enter *grcme = dynamic_cast<Enter *> (n.GRCme);
    STNode *stn = grcme->st;
    
    for (;;) {
      assert(stn);
      
      STNode *parent = stn->parent;
      
      // If we hit a parallel first, this Enter is unnecessary; do not generate
      // any codestart->parents.
      if (dynamic_cast<STpar*>(parent) != NULL) return Status();
      
      e = dynamic_cast<STexcl*>(parent);
      if (e != NULL) break; // found the exclusive node
      stn = parent;
    }

    assert(e != NULL);

    // Locate node n among the children of "parent"
    vector<STNode*>::iterator i = e->children.begin();
    while (*i != stn && i != e->children.end()) i++;
    assert(i != e->children.end());
    
    int childnum = i - e->children.begin();    
    assert(childnum >= 0);
    
    o << "\t SET " << state_regs[e] << " " << childnum;
  
    return Status();
  }
  Status TermNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(TermNode &n) {
    Terminate * t = dynamic_cast<Terminate *>(n.GRCme);
    assert(t);
    assert(t->successors.size() == 1);
    Sync * sync = dynamic_cast<Sync *>(t->successors[0]);
    if (sync) {
      assert(term_regs.find(sync) != term_regs.end());
      o << "\t TRM " << term_regs[sync] << " " << n.code;
    } else {
      o << " ; TermNode without a matching sync";
    }
    return Status();
  }

  Status InitThreadsNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(InitThreadsNode &n){
    for(vector<int>::iterator i = n.startthreads.begin();
	i != n.startthreads.end(); i++)
      o << "\t STRT " << (*i) << " dead_thread_" << (*i) << "\n";
    return Status();
  }

  Status SuspendNode::welcome(GVisitor &v) { return v.visit(*this); }
  Status GRC2BAL::visit(SuspendNode &n){
    o << "; Suspend";
    return Status();
  }

  /* ********************************************************************** */

  /*
   * Class: Scheduler
   * Purpose: calculate the schedule of the nodes of a graph
   * Code copied from GRCC2.cpp
   */

  void calculateSchedule(GRCNode *n, vector<GRCNode*> &s) {
    Scheduler scheduler(n, s);
  }
  void Scheduler::visit(GRCNode *n)
  {
    if (n == NULL || visited.find(n) != visited.end()) return;
    
    visited.insert(n);
    
    for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	 i != n->successors.end() ; i++)
      visit(*i);
    
    for (vector<GRCNode*>::iterator i = n->dataSuccessors.begin() ;
	 i != n->dataSuccessors.end() ; i++)
      visit(*i);
    
    schedule.insert(schedule.begin(), n);
  }
	
  void GRC2BAL::generateBAL(Expression* exp){
    Literal* lt = dynamic_cast<Literal *> (exp);
    if (lt) {
      generateBAL(lt);
      return;
    }

    LoadVariableExpression* lve =
      dynamic_cast<LoadVariableExpression *>(exp);
    if (lve) {
      generateBAL(lve);
      return;
    }

    LoadSignalExpression* lse =
      dynamic_cast<LoadSignalExpression *>(exp);
    if (lse) {
      generateBAL(lse);
      return;
    }

    LoadSignalValueExpression* lsve =
      dynamic_cast<LoadSignalValueExpression *>(exp);
    if (lsve) {
      generateBAL(lsve);
      return;
    }


    UnaryOp* uop = dynamic_cast<UnaryOp *>(exp);
    if(uop != NULL){
      generateBAL(uop);
      return;
    }

    BinaryOp* bop = dynamic_cast<BinaryOp*>(exp);
    if(bop != NULL){
      generateBAL(bop);
      return;
    }
    CheckCounter* cc = dynamic_cast<CheckCounter*>(exp);
    if(cc != NULL){
      generateBAL(cc);
      return;
    }

    FunctionCall* fc = dynamic_cast<FunctionCall*>(exp);
    if(fc != NULL){
      throw IR::Error("Implementation limit: Function calls are not supported");
    }
    // If we reached here, there is something wrong!
    assert(0);
  }

  void GRC2BAL::generateBAL(Assign *a)
  {
    assert(a->variable);
    assert(a->value);
    assert(a->variable->type);
   	
    // Evaluate the expression first
    generateBAL(a->value);

    // And store its value in the variable
    o << "\t STR " << var[a->variable] << " ; " << a->variable->name << std:: endl;
		
  }

  void GRC2BAL::generateBAL(Literal *s)
  {
    o << "\t LDI " << s->value << std:: endl;
  }

  void GRC2BAL::generateBAL(LoadSignalExpression *lse)
  {
    SignalSymbol *ss = lse->signal;
    assert(ss);
    if (signal_regs.find(ss) == signal_regs.end()) {
      // The only undefined signal should be "tick"
      o << "\tLDI 1 ; tick" << std::endl;
    } else {
      o << "\t LDS " << signal_regs[ss] << " ; " << ss->name << std::endl;    
    }
  }

  void GRC2BAL::generateBAL(LoadSignalValueExpression *lse)
  {
    SignalSymbol *ss = lse->signal;
    assert(ss);
    //o << "\t LDR " << signal_regs[ss] << " ; " << ss->name << std::endl;
    // Load the value in the corresponding register
    o << "\t LDR " << valued_signals[ss] << " ; " << ss->name << std::endl;
  }

  void GRC2BAL::generateBAL(LoadVariableExpression *lve)
  {
    ConstantSymbol *cs = dynamic_cast<ConstantSymbol *>(lve->variable);
    if (cs) {      
      if (cs->initializer) {
	Literal * ls = dynamic_cast<Literal *>(cs->initializer);
	if (ls) {
	  o << "\tLDI " << ls->value << std::endl;
	  return;
	}
      }
      o << "\t LDR " << var[cs] << " ; " << cs->name << std::endl;
    } else {
      VariableSymbol *vs = lve->variable;
      assert(vs);
      if (var.find(vs) == var.end()) {
	// Reading uninitialized variable
	throw IR::Error("Reading uninitialized variable " + vs->name);
	assert(0);
      }
      o << "\t LDR " << var[vs] << " ; " << vs->name << std::endl;
    }
  }

  void GRC2BAL::generateBAL(StartCounter *scnt)
  {
    assert(scnt->counter);
    assert(scnt->count);

    // Generate the BAL for the initializing expression
    generateBAL(scnt->count);

    o << "\t STR " << counters[scnt->counter] << std::endl;

  }

  void GRC2BAL::generateBAL(CheckCounter *chkcnt)
  {
    assert(chkcnt->counter);
    assert(chkcnt->predicate);

    generateBAL(chkcnt->predicate);
    o << "\t CDEC " << counters[chkcnt->counter] << std:: endl;
  }

  void GRC2BAL::generateBAL(BinaryOp *e)
  {
    assert(e->source1);
    assert(e->source2);

    generateBAL(e->source1);
    generateBAL(e->source2);

    if (e->op == "and")      o << "\t AND";
    else if (e->op == "or")  o << "\t OR" ;
    else if (e->op == "+")   o << "\t ADD";
    else if (e->op == "-")   o << "\t SUB";
    else if (e->op == "*")   o << "\t MUL";
    else if (e->op == "/")   o << "\t DIV";
    else if (e->op == "mod") o << "\t MOD";
    else if (e->op == "=")   o << "\t CMP";
    else if (e->op == "<>")  o << "\t NEQ";
    else if (e->op == "<")   o << "\t LT" ;
    else if (e->op == "<=")  o << "\t LEQ";
    else if (e->op == ">")   o << "\t GT" ;
    else if (e->op == ">=")  o << "\t GEQ";
    else throw IR::Error(e->op);
    o << std::endl;
    return;
  }

  void GRC2BAL::generateBAL(UnaryOp *e)
  {
    generateBAL(e->source);

    if (e->op == "not") o << "\t NOT";
    else if (e->op == "-") o << "\t NEG";
    else throw IR::Error(e->op);
    o << std::endl;
    return;
  }

}
