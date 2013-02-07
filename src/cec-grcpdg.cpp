#line 1398 "GRCPDG.nw"
#include "IR.hpp"
#include "AST.hpp"

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>

using namespace AST;
using namespace std;

typedef map<GRCNode *, int> CFGmap;
typedef map<STNode *, int> STmap;

#line 33 "GRCPDG.nw"
template <class T> bool contains(set<T> &s, T o) {
  return s.find(o) != s.end();
}
#line 39 "GRCPDG.nw"
template <class T, class U> bool contains(map<T, U> &m, T o) {
  return m.find(o) != m.end();
}

#line 47 "GRCPDG.nw"
class STDPS {
  EnterGRC *entergrc;	
  set<GRCNode *> visited;
  map<GRCNode *, set<GRCNode *> > enter_nodes; //enter nodes under, for node with multi-par only

  public:

  STDPS(EnterGRC *entergrc): entergrc(entergrc) {}
  ~STDPS() {}

  Status execute() {
	visited.clear();
	variable_dfs(entergrc);
	return Status();
  }

  private:

  set<GRCNode *> variable_dfs(GRCNode *n)
  {
    set<GRCNode *> RET;

    if (!n) 
      return RET;
    if (visited.count(n) >0){
      assert(enter_nodes.count(n)>0);
      return enter_nodes[n];
    }

    visited.insert(n);    
    
    for (vector<GRCNode *>::iterator i = n->successors.begin();
	 i != n->successors.end(); i++){
      set<GRCNode *> ch_set = variable_dfs(*i);
      if (ch_set.size()>0){// if find some enters under
	RET.insert(ch_set.begin(), ch_set.end());
      }
    }
    if (dynamic_cast<Enter *>(n)){// if it's an enter, decide whether to add dps
      for (set<GRCNode *>::iterator j = RET.begin();
	j != RET.end(); j++){
	  if (same_sstp(n,*j)){
	    **j << n;
	  }
	}
    }
    if ((dynamic_cast<STSuspend *>(n))
	 ||(dynamic_cast<Switch *>(n))) {//if it's suspend or switch, decide whether to add dps
      for (set<GRCNode *>::iterator j = RET.begin();
	     j != RET.end(); j++){
	  if(st_ancestor(n,*j)){
	    **j << n;
	  }
	}
    }

    if (dynamic_cast<Enter *>(n))
      RET.insert(n);

    if (n->predecessors.size()>1){
      enter_nodes[n].insert(RET.begin(), RET.end());
    }

    return RET;
  }

  //test if two nodes have the same st pointer, n1-suspend, n2-enter
  bool same_stp(GRCNode *n1, GRCNode *n2)
  {
    STSuspend *s;
    Enter *e;

    s = dynamic_cast<STSuspend *>(n1);
    e = dynamic_cast<Enter *>(n2);

    if (s->st == e->st)
      return true;

    return false;
  }

  bool same_sstp(GRCNode *n1, GRCNode *n2)
  {
    Enter *e1, *e2;

    e1 = dynamic_cast<Enter *>(n1); assert(e1);
    e2 = dynamic_cast<Enter *>(n2); assert(e2);

    //if they point to the same stnode, not need to add constrain btw them
    if(e1->st == e2->st)
      return false;

    if (e1->st->parent == e2->st->parent) {
      if (dynamic_cast<STexcl *>(e1->st->parent))
	return true;
    }
    return false;
  }

  bool st_ancestor(GRCNode *p, GRCNode *c)
  {

    GRCSTNode *pp = dynamic_cast<GRCSTNode *>(p); assert(p);
    GRCSTNode *cc = dynamic_cast<GRCSTNode *>(c); assert(cc);

    STNode *stp = pp ->st;
    STNode *stc = cc ->st;

    while(stc != NULL){
	if(stp==stc) return true;
	stc = stc->parent;
    }
    return false;
  }

};

#line 171 "GRCPDG.nw"
class Dependencies : public Visitor {

protected:
  set<GRCNode *> visited;

  GRCNode *current;
  map<GRCNode *, bool> par_label;

  struct SignalNodes {
    set<GRCNode *> writers;
    set<GRCNode *> readers;
  };

  //writers & readers for signals
  map<SignalSymbol *, SignalNodes> dependencies;

  //writers & readers for variables
  map<VariableSymbol *, SignalNodes> v_dependencies; 

  //procedure calls & function calls
  set<GRCNode *> all_calls;

  
#line 438 "GRCPDG.nw"
void dfs(GRCNode *);
#line 455 "GRCPDG.nw"
Status visit(Action &);
#line 475 "GRCPDG.nw"
Status visit(Emit &);
#line 494 "GRCPDG.nw"
Status visit(Exit &);
#line 513 "GRCPDG.nw"
Status visit(Assign &);
#line 531 "GRCPDG.nw"
Status visit(DefineSignal &);
#line 540 "GRCPDG.nw"
Status visit(Test &t) { 
  t.predicate->welcome(*this); return Status(); 
}
#line 557 "GRCPDG.nw"
Status visit(StartCounter &);
#line 584 "GRCPDG.nw"
Status visit(ProcedureCall &);
#line 604 "GRCPDG.nw"
Status visit(FunctionCall &);
#line 610 "GRCPDG.nw"
Status visit(LoadSignalExpression &e) {
  dependencies[e.signal].readers.insert(current);
  current->dataPredecessors.clear();
  return Status();
}

Status visit(LoadSignalValueExpression &e) {
  dependencies[e.signal].readers.insert(current);
  current->dataPredecessors.clear();
  return Status();
}

Status visit(LoadVariableExpression &e) { 
  v_dependencies[e.variable].readers.insert(current);
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
#line 652 "GRCPDG.nw"
Status visit(Literal &) { return Status(); }
#line 660 "GRCPDG.nw"
Status visit(EnterGRC &) { return Status(); }
Status visit(ExitGRC &) { return Status(); }
Status visit(Nop &) { return Status(); }
Status visit(Switch &) { return Status(); }
Status visit(STSuspend &) { return Status(); }
Status visit(Fork &) { return Status(); }
Status visit(Terminate &) { return Status(); }
Status visit(Enter &) { return Status(); }
Status visit(Sync &s) { return Status(); }
#line 194 "GRCPDG.nw"
  void mark_par(GRCNode* n);
  bool have_comm_pp_gen(GRCNode* n, GRCNode* m);
  bool have_comm_pp(GRCNode* n, GRCNode* m);
  bool have_dps(GRCNode *n, GRCNode *m);
  void find_mra(GRCNode *n, const SignalNodes &sn, bool rw); 
  void find_mra_calls(GRCNode *n);

public:

  Dependencies() {}
  virtual ~Dependencies() {}
  void compute(GRCNode *);
};

#line 210 "GRCPDG.nw"
void Dependencies::compute(GRCNode *root)
{
  assert(root);

  dfs(root);

  //add dps on signals
  for ( map<SignalSymbol *, SignalNodes>::const_iterator i =
	  dependencies.begin() ; i != dependencies.end() ;
	  i++ ) {
    const SignalNodes &sn = (*i).second;
    if (!sn.writers.empty() && !sn.readers.empty()) {
      for ( set<GRCNode*>::const_iterator j = sn.writers.begin() ;
	    j != sn.writers.end() ; j++ ){
	visited.clear();
	par_label.clear();
	mark_par(*j);
	for ( set<GRCNode*>::const_iterator k = sn.readers.begin() ;
	      k != sn.readers.end() ; k++ ){
	  visited.clear();
	  if (have_comm_pp_gen((*k),(*j)) && !have_dps(*j, *k))
	    **k << *j;
	}
      }
    }
  }


  //add dps on variables
  for (map<VariableSymbol *, SignalNodes>::const_iterator j =
       v_dependencies.begin(); j != v_dependencies.end();
       j++ ) {
    //VariableSymbol *var = (*j).first;
    const SignalNodes &sn = (*j).second;
    for (set<GRCNode *>::const_iterator i = sn.writers.begin();
         i != sn.writers.end(); i++){
      //looking for most-recent-ancestor of readers/writer on var
      // and save them in visited set
      visited.clear();
      find_mra(*i,sn,true); 
      for(set<GRCNode *>::const_iterator k = visited.begin();
           k != visited.end(); k++){
        if (!have_dps(*k, *i))
           **i << *k;
      }
    }

    for (set<GRCNode *>::const_iterator i = sn.readers.begin();
         i != sn.readers.end(); i++){
      //looking for most-recent-ancestor of writer on var
      visited.clear();
      find_mra(*i,sn,false); 
      //if(visited.size() > 1)
        //cerr<<"Warning: reader "<<*i<<" has more than one pre-writers\n";
      for(set<GRCNode *>::const_iterator k = visited.begin();
          k != visited.end(); k++){
        if (!have_dps(*k, *i))
          **i << *k;
      }
    }
  }

  //add dps btw function/procedure calls
  for (set<GRCNode *>::const_iterator i = all_calls.begin();
       i != all_calls.end(); i++){
    visited.clear();
    find_mra_calls(*i);
    for(set<GRCNode *>::const_iterator k = visited.begin();
        k != visited.end(); k++){
      if (!have_dps(*k, *i))
        **i << *k;
    }
  }

}
#line 289 "GRCPDG.nw"
void Dependencies::mark_par(GRCNode* n)
  {
    int sz, i;
    
    if (visited.count(n) > 0)
      return;

    sz = n->predecessors.size();
    for (i = 0; i < sz; i++){
      if ( par_label[n->predecessors[i]] == false){
	par_label[n->predecessors[i]] = true;
	mark_par(n->predecessors[i]);
      }
    }

    //also mark n itself as its parent
    par_label[n] = true;
    visited.insert(n);
  }
#line 311 "GRCPDG.nw"
  //test if two nodes n & m have parallel first-comm-parent
  //where m's parents have been labeled
  bool Dependencies::have_comm_pp_gen(GRCNode* n, GRCNode* m)
  {
    if (par_label[n])
      return true;

    return have_comm_pp(n,m);
  }
#line 323 "GRCPDG.nw"
  bool Dependencies::have_comm_pp(GRCNode* n, GRCNode* m)
  {

    assert(n);
    if (visited.count(n)>0)
      return false;

    visited.insert(n);
    
    if (par_label[n]){//found a first_comm_parent
      if ((dynamic_cast<Fork *>(n)) //is it a parallel node?
	  || (n == m))//or, is it the emitter corsp?
	return true;
    }
    else { 
      for (vector<GRCNode *>::iterator it =n->predecessors.begin();
	   it != n->predecessors.end(); it++){
	if (have_comm_pp(*it, m))
	  return true;      
      }
    }

    return false;

  }
#line 351 "GRCPDG.nw"
  //test if two nodes n & m have data dependency already
  bool Dependencies::have_dps(GRCNode* n, GRCNode* m)
  {
    vector<GRCNode *>::iterator i;
    bool found = false;

    for (i = n->dataSuccessors.begin(); i != n->dataSuccessors.end(); i++){
      if (*i == m){
        found = true;
        break;
      }
    }

    return found;
  }
#line 369 "GRCPDG.nw"
  //find the most recent ancestor of n which R/W var
  void Dependencies::find_mra(GRCNode *n, const SignalNodes &sn, bool rw)
  {
    vector<GRCNode *>::const_iterator i;

    if (!n)
      return;

    for (i = n->predecessors.begin(); i != n->predecessors.end(); i++){

      if (sn.writers.find(*i) != sn.writers.end()) {//looking for writer
        if ((!rw) || (visited.size() == 0)){
          visited.insert(*i);
          return;
        }
      }
      else if (rw) {//looking for reader also
        if (sn.readers.find(*i) != sn.readers.end()){
          visited.insert(*i);
        }
      }
      find_mra(*i, sn, rw);
    }
  }
#line 396 "GRCPDG.nw"
  //find most recent ancestor of n which includes a function/procedure call
  void Dependencies::find_mra_calls(GRCNode *n)
  {
    vector<GRCNode *>::const_iterator i;

    if (!n)
      return;

    for (i = n->predecessors.begin(); i != n->predecessors.end(); i++){
      if (all_calls.find(*i) != all_calls.end()) {
          visited.insert(*i);
          return;
      }
      else{
        find_mra_calls(*i);
      }
    }
  }
#line 423 "GRCPDG.nw"
void Dependencies::dfs(GRCNode *n)
{
  if (!n || visited.find(n) != visited.end() ) return;

  visited.insert(n);

  current = n;
  n->welcome(*this);

  for (vector<GRCNode*>::const_iterator i = n->successors.begin() ;
       i < n->successors.end() ; i++ ) dfs(*i);  
}
#line 447 "GRCPDG.nw"
Status Dependencies::visit(Action &act)
{
    act.body->welcome(*this);
    return Status();
}
#line 463 "GRCPDG.nw"
Status Dependencies::visit(Emit &emt)
{
    dependencies[emt.signal].writers.insert(current);
    current->dataSuccessors.clear();
    if (emt.value)
      emt.value->welcome(*this);

    return Status();
}
#line 483 "GRCPDG.nw"
Status Dependencies::visit(Exit &ext)
{
    dependencies[ext.trap].writers.insert(current);
    current->dataSuccessors.clear();
    if (ext.value)
      ext.value->welcome(*this);
    return Status();
}
#line 502 "GRCPDG.nw"
Status Dependencies::visit(Assign &asn)
{
    v_dependencies[asn.variable].writers.insert(current);
    //not sure whether need to clear dataSucc or dataPred yet
    if (asn.value)
      asn.value->welcome(*this);
    return Status();
}
#line 521 "GRCPDG.nw"
Status Dependencies::visit(DefineSignal &ds)
{
  assert(ds.signal);
  dependencies[ds.signal].writers.insert(current);
  current->dataSuccessors.clear();
  return Status();
}
#line 550 "GRCPDG.nw"
Status Dependencies::visit(StartCounter &sct)
{
    return Status();
}
#line 566 "GRCPDG.nw"
Status Dependencies::visit(ProcedureCall &prc)
{
  all_calls.insert(current);

  for(vector<Expression *>::const_iterator i = prc.value_args.begin() ;
       i != prc.value_args.end() ; i++) {
    (*i)->welcome(*this);
  }
  for(vector<VariableSymbol *>::const_iterator i = prc.reference_args.begin() ;
       i != prc.reference_args.end() ; i++) {
    v_dependencies[*i].readers.insert(current);
    v_dependencies[*i].writers.insert(current);
  }
  return Status();
}
#line 592 "GRCPDG.nw"
Status Dependencies::visit(FunctionCall &func)
{
  all_calls.insert(current);
  for (vector<Expression*>::const_iterator i = func.arguments.begin() ;
       i != func.arguments.end() ; i++) {
    (*i)->welcome(*this);
  }
  return Status();
}

#line 675 "GRCPDG.nw"
class GRC2PDG {

  CFGmap &dotrefmap;
  
  map<GRCNode *, int> nodenum; // RDFS numbering (index) of each node
  vector<GRCNode*> vert; // nodes in RDFS order

  vector<int> parent; // index of the RDFS spanning tree parent of
		      // each node

  vector<int> ancestor;
  vector<int> semi; // Semi-dominator of each node

  vector<int> idom; // The immediate dominator of each node
  vector<set<int> > ichild; // The nodes immediately dominated by each node

  vector<set<int> > df; // Dominance frontier for each node
  vector<set<int> > cd; // Nodes control dependent on each node

  map<int, vector<int> > succmap;
  map<int, vector<int> > predmap;
  map<int, bool> reachability;
  set<int> visited;
  int N; // Total number of nodes
  int nullnum;

  EnterGRC *enternode;
  ExitGRC *exitnode;

  int debug, debug2;
  
public:
  
#line 719 "GRCPDG.nw"
GRC2PDG(GRCNode *top, CFGmap &dotrefmap) : dotrefmap(dotrefmap)
{
  debug=0;debug2=0;

  assert(top);
  enternode = dynamic_cast<EnterGRC *>(top);
  assert(enternode);
  exitnode = dynamic_cast<ExitGRC *>(enternode->successors[0]);
  assert(exitnode);

  N = 0; // Used to number the nodes during reverse DFS
  reverse_dfs(NULL, exitnode);
 
  build_dominance_tree();

  df.resize(N);
  compute_dominance_frontier(nodenum[exitnode]);
  //print_df();

  cd.resize(N);
  compute_control_dependence();
  //print_CD();

  //cerr<<"start building pdg\n";
  build_pdg();
  //print_PDG();

  visited.clear();
  removeJunkNull(enternode);
  visited.clear();
  removeJunkFork(enternode);
  //cerr<<"finished\n";
}
#line 759 "GRCPDG.nw"
void reverse_dfs(GRCNode *p, GRCNode *n)
{
  if (!n || contains(nodenum,n) ) return;

  nodenum[n] = N;
  vert.push_back(n);
  parent.push_back(p ? nodenum[p] : -1);
  N++;

  if ( n != enternode )
    for (vector<GRCNode*>::iterator i = n->predecessors.begin() ;
	 i != n->predecessors.end() ; i++)
      reverse_dfs(n, *i);
}
#line 780 "GRCPDG.nw"
void build_dominance_tree()
{
  ancestor.resize(N,-1);
  semi.resize(N,-1);
  idom.resize(N,-1);
  vector<int> samedom;
  samedom.resize(N,-1);

  vector<set<int> > bucket;
  bucket.resize(N);

  ichild.resize(N);

  for ( int n = N-1 ; n > 0 ; n-- ) {

    assert(dotrefmap.count(vert[n])>0); // FIXME: ??

    int p = parent[n];
    int s = p;

    for( vector<GRCNode*>::iterator iv = vert[n]->successors.begin() ;
	 iv != vert[n]->successors.end() ; iv++ ) {
      if (*iv) {
	int v = nodenum[*iv];
	int s1 = (v <= n) ? v : semi[ancestor_lowest_semi(v)];
	if ( s1 < s ) s = s1;
      }
    }

    semi[n] = s;
    if ( !contains(bucket[s], n) ) bucket[s].insert(n);
    ancestor[n] = p;
      
    for( set<int>::iterator iv = bucket[p].begin() ;
	 iv != bucket[p].end() ; iv++ ) {
      int v = *iv;
      int y = ancestor_lowest_semi(v);
      if (semi[y] == semi[v]) idom[v] = p; 
      else samedom[v] = y;
    }

    bucket[p].clear();
  }

  for (int n = 1 ; n < N ; n++ )
    if ( samedom[n] != -1 )
      idom[n] = idom[samedom[n]];

  for (int n = 1 ; n < N ; n++)
    if ( idom[n] != -1 ) 
      ichild[idom[n]].insert(n);
}
#line 837 "GRCPDG.nw"
int ancestor_lowest_semi(int v)
{
  int u = v;
  while ( ancestor[v] != -1 ) {
    if ( semi[v] < semi[u] ) u = v;
    v = ancestor[v];
  }

  return u;
} 
#line 855 "GRCPDG.nw"
void compute_dominance_frontier(int n)
{    
  for(set<int>::iterator iz = ichild[n].begin(); iz != ichild[n].end() ; iz++)
    compute_dominance_frontier(*iz);

  int enternodeidx = nodenum[enternode];
   
  if ( n != enternodeidx ) {
    for (vector<GRCNode*>::iterator i = vert[n]->predecessors.begin() ;
	 i != vert[n]->predecessors.end(); i++ ) {
      assert(contains(nodenum, *i));
      int y = nodenum[*i];
      if ( idom[y] != n && !contains(df[n], y) ) {
	assert( contains(dotrefmap, *i) );
	df[n].insert(y);
      }
    }
  }

  for( set<int>::iterator iz = ichild[n].begin() ;
       iz != ichild[n].end() ; iz++) {
    int z = *iz;
    for( set<int>::iterator iy = df[z].begin() ; iy != df[z].end() ; iy++ ) {
      int y = *iy;
      if(idom[y] != n && !contains(df[n], y) ) df[n].insert(y);
    }
  }
}
#line 891 "GRCPDG.nw"
void compute_control_dependence()
{
  for( int y = 0 ; y < N ; y++ )
    for(set<int>::iterator ix=df[y].begin() ; ix!=df[y].end() ; ix++) {
      int x = *ix;
      if ( !contains(cd[x], y) ) cd[x].insert(y);
    }

  //a trick - force EnterGRC's child[1] to be CD of EnterGRC
  cd[nodenum[enternode]].insert(nodenum[enternode->successors[1]]);
} 
#line 907 "GRCPDG.nw"
void build_pdg()
{
  copy_conn();
  remove_conn();

  int counter = N;

  //for each node i
  for (int i = 0; i < N; i++ ) {
    if(debug) cerr<<"for node "<<dotrefmap[vert[i]]<<"\n";
    GRCNode *n = vert[i];

    assert(dotrefmap.count(vert[i])>0);

    if ( n == exitnode ) {

      // n is ExitGRC; ignore it

    } else if ((dynamic_cast<Fork *>(n))
	       ||
	       (n == enternode && (cd[i].size() < 2)) ) {

      // A parallel node or EnterGRC with a single child:
      // Make each CD member a child, disregard its original child number
      // If n is EnterGRC with 1 child, take it as a parallel node
      // **** something may happen, if one can exit in two branches

      for( set<int>::iterator iy = cd[i].begin() ; iy != cd[i].end() ; iy++) {
	GRCNode *y = vert[*iy];
	if ( y != exitnode && ((*iy) != i) ) {
	  n->successors.push_back(y);
	  y->predecessors.push_back(n);
	}
      }

    } else if ( n == enternode ) {     

      // EnterGRC with more than 1 child
  
      Fork *reg = new Fork();
      for (set<int>::iterator iy = cd[i].begin() ; iy != cd[i].end() ; iy++) {
	GRCNode *y = vert[*iy];
	if (y != exitnode && ((*iy) != i)) {
	  reg->successors.push_back(y);
	  y->predecessors.push_back(reg);
	}
      }

      //new region node
      nodenum[reg] = counter++;
      vert.push_back(reg);
      n->successors.push_back(reg);
      reg->predecessors.push_back(n);

    } else {

      // else, for each successor ic of i, make a region node reg

      if(debug) cerr<<" build regions for ic succ:\n";
      for(vector<int>::iterator ic = succmap[i].begin();	  
	  ic != succmap[i].end(); ic++) {

	// NULL node
	if (*ic == -1){
	  n->successors.push_back(NULL);
	  if(debug) cerr<<"  null succ\n";
	  continue;
	}
	if (dynamic_cast<ExitGRC *>(vert[*ic])){
	  if(debug) cerr<<"  exit grc succ\n";
	  continue;
	}

	Fork *reg = new Fork();

	if(debug) cerr<<"  real succ IC "<<dotrefmap[vert[*ic]]<<"\n";

	//for each node iy in CD set of node i, 
	// check if iy is reachable from brunch ic
	for(set<int>::iterator iy=cd[i].begin(); iy!=cd[i].end(); iy++){
	  if ((dynamic_cast<ExitGRC *>(vert[*iy])) || ((*iy) == i))
	    continue;

	  if(debug) cerr<<" IY "<<dotrefmap[vert[*iy]]<<"\n";
	  
	  reachability.clear();
	  if(debug) cerr<<"testing reachablility...";
	  if (reachable((*ic), (*iy))) {	
	    // if yes, add it as a child of the brunch region node reg
	    reg->successors.push_back(vert[*iy]);
	    vert[*iy]->predecessors.push_back(reg);
	  }
	  if(debug) cerr<<" finshed\n";
	}

	//place the region node reg as n's child
	// if reg only has one child, add this child directly
	switch (reg->successors.size()){
	case 0: 
	  //if n is sync|switch|test,instead of reg, place a null node there
	  if ((dynamic_cast<Switch *>(n)) || (dynamic_cast<Sync *>(n))
	      || (dynamic_cast<Test *>(n)))
	    n->successors.push_back(NULL);
	  break;
	case 1: 
	  n->successors.push_back(reg->successors[0]);
	  reg->successors[0]->predecessors.pop_back();
	  reg->successors[0]->predecessors.push_back(n);
	  reg->successors.clear();
	  break;
	default:
	  if(debug) cerr<<"add new reg node: "<<counter<<"\n";
	  nodenum[reg] = counter++;
	  vert.push_back(reg); 
	  n->successors.push_back(reg);
	  reg->predecessors.push_back(n);
	  break;
	}	
      }
      if(debug) cerr<<"N"<<dotrefmap[vert[i]]<<" is finished\n";
    }
  }
}
#line 1035 "GRCPDG.nw"
void copy_conn()
{
  nullnum = 0;

  for (int i = 0; i < N; i++){
    for (vector<GRCNode *>::iterator ic = vert[i]->successors.begin();
	 ic != vert[i]->successors.end(); ic++){
      if (*ic)
	succmap[i].push_back(nodenum[*ic]);
      else{
	succmap[i].push_back(-1);
	nullnum++;
      }
    }
    for (vector<GRCNode *>::iterator ip = vert[i]->predecessors.begin();
	 ip != vert[i]->predecessors.end(); ip++){
      predmap[i].push_back(nodenum[*ip]);
    }
  }
}
#line 1060 "GRCPDG.nw"
void remove_conn()
{
  for (int i = 0; i < N; i++){
    vert[i]->successors.clear();
    if (vert[i] != enternode)
      vert[i]->predecessors.clear();
  }
}
#line 1073 "GRCPDG.nw"
bool reachable(int from, int to)
{
  if(debug2) cerr<<" dfs "<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"\n";

  if (reachability.count(from) > 0)
    return reachability[from];
      
  if (from == 0){
    if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  NO2\n";
    reachability[from] = false;
    return false;
  }

  if (from == -1){
    if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  YES2\n";
    reachability[from] = true;
    return true;
  }

  if (to == from){
    if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  YES1\n";
    reachability[from] = true;
    return true;
  }

  assert(vert[from]);

  //for fork node, reachable from any one of the children is reable
  if (dynamic_cast<Fork *>(vert[from])){
    for (vector<int>::iterator ic = succmap[from].begin();
	 ic != succmap[from].end(); ic++){
      if (reachable((*ic), to)){
	if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  YES3\n";
	reachability[from] = true;
	return true;
      }
    }
    if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  NO3\n";
    reachability[from] = false;
    return false;
  }

  //for else node, reachable means can be reached from all of the children
  for (vector<int>::iterator ic = succmap[from].begin();
       ic != succmap[from].end(); ic++){
    if (!reachable((*ic), to)){
      if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  NO4\n";
      reachability[from] = false;
      return false;
    }
  }
  if(debug2) cerr<<dotrefmap[vert[from]]<<"->"<<dotrefmap[vert[to]]<<"  YES4\n";
  reachability[from] = true;
  return true;
}
#line 1133 "GRCPDG.nw"
void removeJunkNull(GRCNode *n)
{
  vector<GRCNode *>::iterator i;
  vector<GRCNode *> newch;
  bool isfork = false;

  if (!n)
    return;

  if (visited.count(nodenum[n]) > 0)
    return;  
  visited.insert(nodenum[n]);

  for (i = n->successors.begin(); i != n->successors.end(); i++)
    removeJunkNull(*i);         

  if (dynamic_cast<Fork *>(n))
    isfork = true;

  if (n->successors.size() == 0)
    return;

  for (i = n->successors.begin(); i != n->successors.end(); i++){
    if (!*i){
      if (isfork){
        rm_invect((*i)->predecessors, n);
        continue;
      }
    }
    else if ((dynamic_cast<Fork *>(*i)) && ((*i)->successors.size() == 0)){
      rm_invect((*i)->predecessors, n);
      rm_datadps(*i);
      continue;
    }
    else if (all_child_null(*i)){
      if (isfork){
        rm_invect((*i)->predecessors, n);
        rm_datadps(*i);
        continue;
      }
      else
	*i = NULL;
    }
    newch.push_back(*i);
  }

  n->successors = newch;
                
}
#line 1187 "GRCPDG.nw"
void removeJunkFork(GRCNode *n)
{
  vector<GRCNode *>::iterator i,j;
  vector<GRCNode *> newch;

  if (!n)
    return;

  if (visited.count(nodenum[n]) > 0)
    return;  
  visited.insert(nodenum[n]);

  for (i = n->successors.begin(); i != n->successors.end(); i++)
    removeJunkFork(*i);  

  if (dynamic_cast<Fork *>(n)){
    assert(n->successors.size()>0);
    for (i = n->successors.begin(); i != n->successors.end(); i++)
      if ((dynamic_cast<Fork *>(*i)) && ((*i)->predecessors.size() == 1)){
        for (j = (*i)->successors.begin(); j != (*i)->successors.end(); j++){  
	  newch.push_back(*j);
	  rm_invect((*j)->predecessors, *i);
          (*j)->predecessors.push_back(n);
        }
	(*i)->predecessors.clear();
	(*i)->successors.clear();
      }
      else 
	newch.push_back(*i);
    n->successors = newch;
  }
}
#line 1224 "GRCPDG.nw"
void rm_invect(vector<GRCNode *> &vec, GRCNode *n)
{
  assert(n);
  vector<GRCNode *>::iterator i = vec.begin();
    while (i != vec.end()){
      if ((*i) == n)
	i = vec.erase(i);
      else
	i++;
    }
}
#line 1240 "GRCPDG.nw"
void rm_datadps(GRCNode *n)
{
  vector<GRCNode *>::iterator i;

  for (i = n->dataPredecessors.begin(); i != n->dataPredecessors.end(); i++){
    rm_invect((*i)->dataSuccessors, n);
  }

  for (i = n->dataSuccessors.begin(); i != n->dataSuccessors.end(); i++){
    rm_invect((*i)->dataPredecessors, n);
  }

  n->dataPredecessors.clear();
  n->dataSuccessors.clear();
}
#line 1260 "GRCPDG.nw"
bool all_child_null(GRCNode *n)
{
  int sz;

  assert(n);
  sz = n->successors.size();

  if (sz == 0)
    return false;

  for(vector<GRCNode *>::iterator i = n->successors.begin();
      i != n->successors.end(); i++)
    if (*i)
      return false;
	
  return true;
}
#line 708 "GRCPDG.nw"
};


#line 1351 "GRCPDG.nw"
int main(int argc, char* argv[])
{
  IR::XMListream f(std::cin);
  IR::Node *n;
  f >> n;

  Modules *mods = dynamic_cast<AST::Modules*>(n);
  if (!mods) {
    std::cerr<<"Root node is not a module object\n";
    exit(-2);
  }
  
  for( vector<AST::Module*>::iterator i = mods->modules.begin(); 
       i != mods->modules.end(); i++){
    assert(*i);

    GRCgraph *gf = dynamic_cast<GRCgraph*>((*i)->body);
    assert(gf);  
    GRCNode *top = gf->control_flow_graph;
    
    CFGmap dotrefmap;
    STmap strefmap;

    EnterGRC *engrc = dynamic_cast<EnterGRC*>(top);
    assert(engrc);
    
    // compute the data dependencies between Enter & STsuspend nodes
    //   remove & recompute dps between variables
    Dependencies vardps;
    vardps.compute(engrc);

    STDPS compdps(engrc);
    compdps.execute();

    // Convert the GRC graph into a PDG
    gf->enumerate(dotrefmap, strefmap);
    GRC2PDG converter(top, dotrefmap);
  }

  IR::XMLostream o(std::cout);
  o << n;

  return 0;
}
