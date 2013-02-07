#line 1323 "GRCOpt.nw"
#include "GRCOpt.hpp"
#include "ASTGRC.hpp"
#include <iostream>
#include <algorithm>

namespace GRCOpt {

  GRCNode::NumMap cfgmap;
  STNode::NumMap stmap;

  
#line 80 "GRCOpt.nw"
void delete_node(GRCNode *n)
{
  assert(n);
  for ( vector<GRCNode*>::const_iterator i = n->successors.begin() ;
        i != n->successors.end() ; i++ )
    if (*i) erase((*i)->predecessors, n);
  for ( vector<GRCNode*>::const_iterator i = n->predecessors.begin() ;
        i != n->predecessors.end() ; i++ )
    *(find((*i)->successors, n)) = 0;
  for ( vector<GRCNode*>::const_iterator i = n->dataSuccessors.begin() ;
        i != n->dataSuccessors.end() ; i++ )
    erase((*i)->dataPredecessors, n);
  for ( vector<GRCNode*>::const_iterator i = n->dataPredecessors.begin() ;
        i != n->dataPredecessors.end() ; i++ )
    erase((*i)->dataSuccessors, n);

  // std::cerr << "Deleting a " << n->className() << std::endl;
  delete n;
}
#line 108 "GRCOpt.nw"
void delete_node(STNode *n)
{
  assert(n);

  if (n->parent) {
    erase(n->parent->children, n);
    n->parent = 0;
  }

  for ( vector<STNode*>::const_iterator i = n->children.begin() ;
        i != n->children.end() ; i++ )
    (*i)->parent = 0;

  n->children.clear();

  delete n;
}
#line 137 "GRCOpt.nw"
void bypass(GRCNode *n) 
{
  assert(n);
  assert(n->successors.size() == 1);

  GRCNode *successor = n->successors.front();
  assert(successor);
  
  erase(successor->predecessors, n);
  n->successors.clear();
  
  for (vector<GRCNode*>::iterator i = n->predecessors.begin() ;
       i != n->predecessors.end() ; i++) {
    vector<GRCNode*>::iterator ip = find( (*i)->successors, n );
    assert(ip != (*i)->successors.end());
    (*ip) = successor;
    successor->predecessors.push_back(*i);
  }

  n->predecessors.clear();

  delete_node(n);
}
#line 170 "GRCOpt.nw"
void bypass(STNode *n)
{
  assert(n);
  assert(n->children.size() == 1);


  STNode *child = n->children.front();
  assert(child);

  assert(n->parent);
  vector<STNode*>::iterator i = find(n->parent->children, n);
  assert(i != n->parent->children.end());
  *i = child;

  child->parent = n->parent;

  n->parent = NULL;
  n->children.clear(); 
  // delete n; // FIXME: This should work, but it causes problems
}
#line 1334 "GRCOpt.nw"
  
#line 254 "GRCOpt.nw"
Simulator::Simulator(GRCgraph &gg) : g(gg)
{
  
#line 270 "GRCOpt.nw"
STNode *stroot = g.selection_tree;
assert(stroot);
st.insert(stroot);

// The root should have three children: initial, running, and terminated
assert(stroot->children.size() == 3);
st.insert(stroot->children[2]); // Mark initial child as reachable

// Mark the topmost node in the control-flow graph (after the EnterGRC)
// as reachable and mark it as needing to be visited

assert(g.control_flow_graph);
assert(g.control_flow_graph->successors.size() == 2);
GRCNode *root = g.control_flow_graph->successors.back();
assert(root);
pending.insert(root);
#line 257 "GRCOpt.nw"
  
#line 294 "GRCOpt.nw"
while ( !pending.empty() ) {
  set<GRCNode *>::iterator pi = pending.begin();
  GRCNode *n = *pi;
  assert(n);
  pending.erase(pi);
  // std::cerr << "Pending node " << cfgmap[n] << '\n';

  cfg.insert(n);
  n->welcome(*this);
}
#line 258 "GRCOpt.nw"
  
#line 317 "GRCOpt.nw"
for ( map<STexcl *, Switch *>::iterator i = switch_for_excl.begin() ;
	i != switch_for_excl.end() ; i++ ) {
  STexcl *excl = (*i).first;
  Switch *sw = (*i).second;
  /* std::cerr << "cleaning exclusive " << stmap[excl] << " and switch "
               << cfgmap[sw] << std::endl; */
  assert(excl);
  assert(sw);
  assert(excl->children.size() == sw->successors.size());

  // Remove the children/successors corresponding to unvisited
  // children in the Selection Tree
  vector<GRCNode*>::iterator j = sw->successors.begin();
  vector<STNode*>::iterator k = excl->children.begin();
  while ( j != sw->successors.end() ) {
    assert(*j);
    assert(*k);
    if ( !contains(st, *k) ) {
	erase((*j)->predecessors, (GRCNode*) sw);
	j = sw->successors.erase(j);
	(*k)->parent = 0;
	k = excl->children.erase(k);
    } else {
	j++;
	k++;
    }
  }
}
#line 259 "GRCOpt.nw"
  
#line 352 "GRCOpt.nw"
for ( map<Sync *, set<int> >::iterator i = sync_levels.begin() ;
      i != sync_levels.end() ; i++ ) {
  Sync *sync = (*i).first;
  set<int> &levels = (*i).second;
  assert(sync);
  assert(!levels.empty());

  for ( vector<GRCNode*>::iterator j = sync->successors.begin() ;
	j != sync->successors.end() ; j++ ) {
    if ( *j && !contains(levels, j - sync->successors.begin()) ) {
       erase((*j)->predecessors, (GRCNode*) sync);
       *j = NULL;
    }
  }
}
#line 260 "GRCOpt.nw"
  
#line 374 "GRCOpt.nw"
entergrc = g.control_flow_graph;
assert(entergrc);
assert(entergrc->successors.size() == 2);
GRCNode *grcroot  = entergrc->successors[1];
all_dfs(grcroot);

set<GRCNode *> unreachablecfg;
set_difference( allcfg.begin(), allcfg.end(),
	        cfg.begin(), cfg.end(),
	        inserter(unreachablecfg, unreachablecfg.begin()) );

// std::cerr << "Unreachable CFG nodes:";
for ( set<GRCNode*>::const_iterator i = unreachablecfg.begin() ;
	i != unreachablecfg.end() ; i++ ) {
  // std::cerr << ' ' << cfgmap[*i];
  delete_node(*i);
}
// std::cerr << std::endl;
#line 261 "GRCOpt.nw"
  
#line 399 "GRCOpt.nw"
st_walk(stroot);

set<STNode *> unreachablest;
set_difference( allst.begin(), allst.end(),
		  st.begin(), st.end(),
		  inserter(unreachablest, unreachablest.begin()) );

// std::cerr << "Unreachable ST nodes:";
for ( set<STNode*>::const_iterator i = unreachablest.begin() ;
	i != unreachablest.end() ; i++ ) {
  // std::cerr << ' ' << stmap[*i];
  delete_node(*i);
}
//  std::cerr << std::endl;
#line 262 "GRCOpt.nw"
}
#line 421 "GRCOpt.nw"
Status Simulator::visit(Switch &s)
{
  // Remember our exclusive node for when we encounter Enter statements
  STexcl *excl = dynamic_cast<STexcl*>(s.st);
  assert(excl);
  switch_for_excl[excl] = &s;

  // Keep our STexcl node
  st.insert(excl);

  // Should always have the same number of children under the switch as
  // under the exclusive node in the ST
  assert(excl->children.size() == s.successors.size());

  // For each visited ST node under the exclusive, schedule the
  // corresponding successor of the switch if it hasn't already been visited

  for ( vector<STNode *>::const_iterator i = excl->children.begin() ;
	i != excl->children.end() ; i++ ) {
    if (contains(st, *i)) {
      // Determine the matching successor of this child
      GRCNode *suc = s.successors[i - excl->children.begin()];
      assert(suc); // All switch successors should be non-NULL
      // If the successor hasn't been visited already, schedule it
      if (!contains(cfg, suc)) pending.insert(suc);
    }
  }

  return Status();
}
#line 460 "GRCOpt.nw"
Status Simulator::visit(Enter &e)
{
  STNode *stnode = e.st;
  assert(stnode);

  // Find the STexcl node corresponding to our ST node
  // by climbing the tree until we hit one
  do {
    // Mark our ST node an all of our parents as visited
    st.insert(stnode);
    stnode = stnode->parent;
    assert(stnode); // Shouldn't go past the root, which should be an STexcl
  } while ( !dynamic_cast<STexcl*>(stnode) );
  STexcl *excl = dynamic_cast<STexcl*>(stnode);
  assert(excl); // Should have found the exclusive node

  // If we know about the corresponding switch node, schedule it
  if (contains(switch_for_excl, excl)) {
    assert(switch_for_excl[excl]);
    pending.insert(switch_for_excl[excl]);
  }
  
  // Schedule our successor
  assert(e.successors.size() == 1);
  assert(e.successors.front());
  pending.insert(e.successors.front());
  return Status();
}
#line 500 "GRCOpt.nw"
Status Simulator::visit(Terminate &t)
{
  // Schedule our successor
  assert(t.successors.size() == 1);
  assert(t.successors.front());
  pending.insert(t.successors.front());
  return Status();
}
#line 532 "GRCOpt.nw"
Status Simulator::visit(Sync &s)
{
  // Count the number of threads coming into this sync: the maximum
  // index among all the preceeding terminate nodes

  int numThreads = 0;
  for ( vector<GRCNode*>::const_iterator i = s.predecessors.begin() ;
        i != s.predecessors.end() ; i++ ) {
    Terminate *term = dynamic_cast<Terminate*>(*i);
    assert(term); // all predecessors should be Terminate nodes
    if (term->index >= numThreads) numThreads = term->index + 1;
  }

  vector<set<int> > levels(numThreads);

  // Build the sets of all exit levels by considering only those that
  // have been visited

  for ( vector<GRCNode*>::const_iterator i = s.predecessors.begin() ;
        i != s.predecessors.end() ; i++ ) {
    Terminate *term = dynamic_cast<Terminate*>(*i);
    assert(term); // all predecessors should be Terminate nodes
    if (contains(cfg, (GRCNode*) term))
      levels[term->index].insert(term->code);
  }

  int overallmin = 0;

  for ( vector<set<int> >::const_iterator i = levels.begin() ;
        i != levels.end() ; i++ ) {
    if ((*i).empty()) {
      // Don't know anything about one of the threads: need more
      // information before concluding which children may run,
      // so we'll stop here
      return Status();
    }
    int min = *((*i).begin());
    for ( set<int>::const_iterator j = (*i).begin() ;
	  j != (*i).end() ; j++ )
      if ( (*j) < min ) min = *j;
    if (min > overallmin) overallmin = min;
  }

  // Compute the union of all levels greater or equal to the
  // maximum of all the minimums

  set<int> &level = sync_levels[&s];
  level.clear();
  for ( vector<set<int> >::const_iterator i = levels.begin() ;
        i != levels.end() ; i++ )
    for ( set<int>::const_iterator j = (*i).begin() ;
	  j != (*i).end() ; j++ )
      if ( (*j) >= overallmin ) level.insert(*j);

  // Schedule all the active children that aren't already visited

  for ( vector<GRCNode*>::const_iterator i = s.successors.begin() ;
        i != s.successors.end() ; i++ )
    if ( contains(level, (int) (i - s.successors.begin())) &&
	 !contains(cfg, *i) ) {
      assert(*i);
      pending.insert(*i);
    }

  return Status();
}
#line 609 "GRCOpt.nw"
Status Simulator::visit(Fork &f)
{
  for (vector<GRCNode*>::const_iterator i = f.successors.begin() ;
       i != f.successors.end() ; i++ ) {
    assert(*i);
    pending.insert(*i);
  }
  return Status();
}
#line 625 "GRCOpt.nw"
Status Simulator::visit(Test &s)
{
  for (vector<GRCNode*>::const_iterator i = s.successors.begin() ;
       i != s.successors.end() ; i++ ) {
    assert(*i);
    pending.insert(*i);
  }
  return Status();
}
#line 644 "GRCOpt.nw"
Status Simulator::visit(DefineSignal &d)
{
  // Schedule our successor
  assert(d.successors.size() == 1);
  assert(d.successors.front());
  pending.insert(d.successors.front());
  return Status();
}
#line 659 "GRCOpt.nw"
Status Simulator::visit(Action &a)
{
  // Schedule our successor
  assert(a.successors.size() == 1);
  assert(a.successors.front());
  pending.insert(a.successors.front());
  return Status();
}
#line 674 "GRCOpt.nw"
Status Simulator::visit(Nop &s)
{
  // Schedule our successor
  assert(s.successors.size() == 1);
  assert(s.successors.front());
  pending.insert(s.successors.front());
  return Status();
}
#line 689 "GRCOpt.nw"
Status Simulator::visit(STSuspend &s)
{
  // Schedule our successor
  assert(s.successors.size() == 1);
  assert(s.successors.front());
  pending.insert(s.successors.front());
  return Status();
}
#line 717 "GRCOpt.nw"
void Simulator::all_dfs(GRCNode *n)
{
  if ( !n || n == entergrc || contains(allcfg, n) ) return;

  allcfg.insert(n);

  for (vector<GRCNode*>::const_iterator ch = n->successors.begin();
      ch != n->successors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->predecessors.begin();
      ch != n->predecessors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->dataSuccessors.begin();
      ch != n->dataSuccessors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->dataPredecessors.begin();
      ch != n->dataPredecessors.end(); ch++) all_dfs(*ch);
}
#line 744 "GRCOpt.nw"
void Simulator::st_walk(STNode *n)
{
  allst.insert(n);

  for (vector<STNode*>::const_iterator ch = n->children.begin();
      ch != n->children.end(); ch++)
    st_walk(*ch);
}
#line 1335 "GRCOpt.nw"
  
#line 816 "GRCOpt.nw"
void Pass::forward_dfs(GRCNode *n)
{
  if (!n || n == exitgrc || contains(reachable_nodes, n) ) return;

  reachable_nodes.insert(n);

  for(vector<GRCNode*>::const_iterator ch = n->successors.begin();
      ch != n->successors.end(); ch++) forward_dfs(*ch);

  topolist.push_back(n);
}
#line 838 "GRCOpt.nw"
void Pass::all_dfs(GRCNode *n)
{
  if ( !n || n == exitgrc || n == entergrc || contains(all_nodes, n) ) return;

  all_nodes.insert(n);

  for (vector<GRCNode*>::const_iterator ch = n->successors.begin();
      ch != n->successors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->predecessors.begin();
      ch != n->predecessors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->dataSuccessors.begin();
      ch != n->dataSuccessors.end(); ch++) all_dfs(*ch);
  for (vector<GRCNode*>::const_iterator ch = n->dataPredecessors.begin();
      ch != n->dataPredecessors.end(); ch++) all_dfs(*ch);
}
#line 871 "GRCOpt.nw"
void Pass::transform()
{
  forward_dfs(grcroot); // build topolist
  all_dfs(grcroot);

  set<GRCNode *> unreachable;
  set_difference( all_nodes.begin(), all_nodes.end(),
		  reachable_nodes.begin(), reachable_nodes.end(),
		  inserter(unreachable, unreachable.begin()) );

  for (set<GRCNode *>::const_iterator i = unreachable.begin();
       i != unreachable.end() ; i++)
    delete_node(*i);

  if (forward)
    for (vector<GRCNode *>::iterator i = topolist.begin() ;
	 i != topolist.end() ; i++ ) (*i)->welcome(*this);
  else
    for (vector<GRCNode *>::reverse_iterator i = topolist.rbegin() ;
	 i != topolist.rend() ; i++ ) (*i)->welcome(*this);
}
#line 1336 "GRCOpt.nw"
  
#line 1095 "GRCOpt.nw"
Status DanglingST::visit(Enter &s)
{
  if ( !contains(stkept, s.st) ) {
    // std::cerr << "Dangling Enter: " << cfgmap[&s] << '\n';
    bypass(&s);
  }
  return Status();
}

Status DanglingST::visit(STSuspend &s)
{
  if ( !contains(stkept, s.st) ) {
    // std::cerr<<"Dangling STSuspend: "<<cfgmap[&s]<<'\n';
    bypass(&s);
  }
  return Status();
}
#line 1337 "GRCOpt.nw"
  
#line 984 "GRCOpt.nw"
Status PruneSW::visit(Switch &s)
 {
   for ( vector<STNode*>::iterator sch = s.st->children.begin() ;
	 sch != s.st->children.end() ; ) {
      if (*sch && contains(stkept, *sch) ) {
	sch++;
      } else {
	// The decision was made to delete the selection tree node
	// corresponding to this child of the switch.
	// Remove the arc from the switch to the corresponding child
	vector<GRCNode*>::iterator ch =
	  s.successors.begin() + (sch - s.st->children.begin());
	erase( (*ch)->predecessors, (GRCNode*) &s );
	s.successors.erase(ch);
	sch = s.st->children.erase(sch); // now sch points to the next element
      }
   }

    // remove switches with only 1 child
    if ( s.successors.size() == 1 ) {
      bypass(&s);  // Remove the switch

      // Each switch in the ST should have exactly one switch
      // in the control-flow graph, so we should never encounter
      // an already-removed switch
      assert( contains(stkept, s.st) );

      bypass(s.st);  // Remove the selection tree node
      stkept.erase(s.st);  // Mark the selection tree node as gone
    }
    
    return Status();
  }
#line 1338 "GRCOpt.nw"
  
#line 1035 "GRCOpt.nw"
Status MergeSW::visit(Switch &s)
{

  if (s.predecessors.size() > 1)
    return Status();

  int szc = s.successors.size();
  assert(szc == (int) s.st->children.size() );

  Switch *p = dynamic_cast<Switch*>(s.predecessors.front());
  if (!p) return Status();

  // Parent of this switch is also a switch: merge

  vector<GRCNode*>::iterator ip = find(p->successors, (GRCNode*) &s);
  int chno = ip-p->successors.begin();
  ip = p->successors.erase(ip);

  for (int ich = 0; ich < szc; ich++ ) {
    GRCNode *ch = s.successors[ich];
    vector<GRCNode*>::iterator i = find(ch->predecessors, (GRCNode*) &s);
    (*i)=p;
    ip = p->successors.insert(ip, ch);
    ip++;
  }
    
  STNode *st_par = p->st;
  vector<STNode*>::iterator ips = st_par->children.begin() + chno;
  ips = st_par->children.erase(ips);

  for (int ich = 0 ; ich < szc ; ich++) {
    STNode *chs = s.st->children[ich];
    chs->parent = st_par;
    ips = st_par->children.insert(ips, chs);
    ips++;
  }

  stkept.erase(s.st);

  return Status();
}
#line 1339 "GRCOpt.nw"
  
#line 915 "GRCOpt.nw"
STNode *STSimplify::check_st(STNode *n, STNode *realpar)
{
  bool keep = false;
  STNode* c;
  STref *ref;
  int is_simpleref;

  n->parent = realpar;

  if (dynamic_cast<STleaf*>(n) ) {
    stkept.insert(n);
    return n;
  }

  is_simpleref = 0;
  if((ref=dynamic_cast<STref*>(n)))
    is_simpleref = ! (ref->isabort() || ref->issuspend()); 

  if(!is_simpleref) realpar = n; // try to keep

  c=NULL; keep = false;
  for(vector<STNode*>::iterator i=n->children.begin(); i!=n->children.end(); i++)
    if(*i){
      (*i) = c = check_st(*i, realpar);
      if(c) keep=1;
    }
    
  if(is_simpleref)
    if(keep) return c; else return NULL;
    
  if(keep) { stkept.insert(n); return n; } else return NULL;
}
#line 1340 "GRCOpt.nw"
  
#line 962 "GRCOpt.nw"
Status RemoveNops::visit(Nop &s){
     bypass(&s);
     return Status();
};
#line 1341 "GRCOpt.nw"
  
#line 1129 "GRCOpt.nw"
Status RedundantEnters::visit(Enter &s)
{
  assert(s.st);
  if ( dynamic_cast<STexcl*>(s.st->parent) == NULL ) {
    // Parent is either not an exclusive node or does not exist: delete this
    // Enter node
    bypass(&s);
  }
  return Status();
}
#line 1342 "GRCOpt.nw"
  
#line 1158 "GRCOpt.nw"
struct Dependencies : public ASTGRC::Dependencies {
  Status visit(Sync&) { return Status(); } // disable adding sync dependencies
};

UnobservedEmits::UnobservedEmits(Module *m, GRCgraph *g) : Pass(g, false)
{
  assert(g);
  GRCNode *root = g->control_flow_graph;
  assert(root);

  Dependencies depper;

  depper.dfs(root);

  // Every signal with one or more readers is observed
  for ( map<SignalSymbol *, ASTGRC::Dependencies::SignalNodes>::const_iterator
	  i = depper.dependencies.begin() ; i != depper.dependencies.end() ;
	  i++ ) {
    const ASTGRC::Dependencies::SignalNodes sn = (*i).second;
    if (sn.readers.size() > 0) observed.insert((*i).first);
  }

  // Every output or inputoutput signal is observed by the environment
  for ( SymbolTable::const_iterator i = m->signals->begin();
	i != m->signals->end() ; i++ ) {
    SignalSymbol *s = dynamic_cast<SignalSymbol*>(*i);
    assert(s);
    if (s->kind == SignalSymbol::Output ||
	s->kind == SignalSymbol::Inputoutput)
      observed.insert(s);
  }
}
#line 1195 "GRCOpt.nw"
Status UnobservedEmits::visit(Action &s)
{
  Emit *emit = dynamic_cast<Emit*>(s.body);
  if (emit && observed.find(emit->signal) == observed.end()) bypass(&s);
  Exit *exit = dynamic_cast<Exit*>(s.body);
  if (exit && observed.find(exit->trap) == observed.end()) bypass(&s);
  return Status();
}
#line 1210 "GRCOpt.nw"
Status UnobservedEmits::visit(DefineSignal &s)
{
  if (observed.find(s.signal) == observed.end() &&
      !(s.is_surface && s.signal->initializer)) bypass(&s);
  return Status();
}
#line 1343 "GRCOpt.nw"
}

#line 1224 "GRCOpt.nw"
int main(int argc, char *argv[])
{   
  try {
    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;

    AST::Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    for ( std::vector<AST::Module*>::iterator i = mods->modules.begin() ;
 	  i != mods->modules.end() ; i++ ) {
      AST::Module* mod = *i;
      assert(mod);

      AST::GRCgraph *g = dynamic_cast<AST::GRCgraph*>(mod->body);
      if (!g) throw IR::Error("Module is not in GRC format");

      g->enumerate(GRCOpt::cfgmap, GRCOpt::stmap);

      // Remove unreachable nodes by performing symbolic simulation
      GRCOpt::Simulator sim(*g);

      // Set that contains the selection tree nodes to be preserved
      std::set<AST::STNode*> stkept;

      // Remove needless ST nodes
      GRCOpt::STSimplify pass1(g, stkept);
      pass1.simplify();

      GRCOpt::RemoveNops pass1a(g);
      pass1a.transform();

      // Remove children of switches corresponding to needless ST nodes
      GRCOpt::PruneSW pass2(g, stkept);
      pass2.transform();

      // Merge cascaded switches
      GRCOpt::MergeSW pass3(g, stkept);
      pass3.transform();

      // Remove Enter and STSuspend nodes corresponding to removed ST nodes
      GRCOpt::DanglingST pass4(g, stkept);
      pass4.transform();

      // Remove any unreachable nodes
      GRCOpt::Pass pass5(g, true);
      pass5.transform();

      // Remove redundant Enter nodes, i.e., those not immediately beneath
      // an STexcl
      GRCOpt::RedundantEnters pass6(g);
      pass6.transform();

      // Remove emit nodes for signals that are never tested
      GRCOpt::UnobservedEmits pass7(mod, g);
      pass7.transform();
    }

    // end of transformations

    IR::XMLostream w(std::cout);
    w << n;

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  }
  return 0;
}
