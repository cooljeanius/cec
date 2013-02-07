#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <vector>
#include "IR.hpp"
#include "AST.hpp"

using namespace AST;
using namespace std;

template <class T> bool contains(set<T> &s, T o) {
  return s.find(o) != s.end();
}

template <class T, class U> bool contains(map<T, U> &m, T o) {
  return m.find(o) != m.end();
}

bool contains(vector<GRCNode *> &v, GRCNode * n) {
  vector<GRCNode *>::iterator i;
  for(i = v.begin(); i != v.end(); i++)
    if(*i == n)
      return true;
  return false;
}

#define T_FORK 1
#define T_TEST 2
#define T_EMIT 3
#define T_TERM 4
#define T_SYNC 5
#define T_ENTER 6
#define T_SWITCH 7
#define T_FINAL_TERM 8

#define DEBUG 0

class TriValConstructor : public Visitor{
private:
  GRCNode *top;
  GRCNode *current;
  GRCNode::NumMap &refmap;
  set<GRCNode *> visited;
  map<GRCNode *, set<GRCNode *> > subgraph; //subgraphs under the fork
  set<GRCNode *> forks;
  map<GRCNode *, GRCNode *> xcopy; // reconstructed x graph
  map<GRCNode *, vector<GRCNode *> > conn_succ;

  int type;

public:
  TriValConstructor(GRCNode *top, GRCNode::NumMap &refmap): top(top), refmap(refmap) {}
  virtual ~TriValConstructor() {}

  void execute(){
    //init
    Dfs(top);

    //collect each successor's subgraph of fork
    for(set<GRCNode *>::iterator i = forks.begin();
	i != forks.end(); i++){
      Fork *f = dynamic_cast<Fork *>(*i);
      assert(f);
      for(vector<GRCNode *>::iterator j = f->successors.begin();
	  j != f->successors.end(); j++){
	assert((*j)->predecessors.size()<2);
	Dfs_Subgraph(*j, f->sync, subgraph[*j]);
	if(DEBUG) cerr<<"fork "<<refmap[f]<<" succ "<<refmap[*j]<<" has "
		      <<subgraph[*j].size()<<" decs.\n";
      }
    }

    //construction
    set<GRCNode *> black;
    Visit(top, black);
    
    //copy subgraphs which has fork ancestor
    visited.clear();
    CheckGraph(top);
    
    visited.clear();
    CheckForks(top);
   
  }

  void Dfs(GRCNode *n){
    if(!n)
      return;
    if(contains(visited, n))
      return;
    visited.insert(n);

    if(dynamic_cast<Fork *>(n))
      forks.insert(n);

    for(vector<GRCNode *>::iterator i = n->successors.begin();
	i != n->successors.end(); i++)
      Dfs(*i);
  }

  void Dfs_Subgraph(GRCNode *n, GRCNode *stop, set<GRCNode *> &a_subgraph){
    if(!n)
      return;
    if(n == stop){
      a_subgraph.insert(n);
      return;
    }
    if(contains(a_subgraph, n))
      return;
    a_subgraph.insert(n);

    for(vector<GRCNode *>::iterator i = n->successors.begin();
	i != n->successors.end(); i++)
      Dfs_Subgraph(*i, stop, a_subgraph);
  }

  void Visit(GRCNode *n, set<GRCNode *> &black){
    if(!n)
      return;
    if(contains(black, n))
      return;
    black.insert(n);

    for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	 i != n->successors.end() ; i++ ){
      Visit(*i, black);
    }

    if(dynamic_cast<Test *>(n)){
      vector<GRCNode *> xsuccs;
      GRCNode *f = new Fork();
      if(n->successors.size() != 2){
	if(DEBUG) cerr<<"test "<<refmap[n]<<" has "<<n->successors.size()<<" succs\n";
	assert(0);
      }
      for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	   i != n->successors.end() ; i++ ){
	GRCNode * succ = Construct(*i, visited);
	if(succ)
	  xsuccs.push_back(succ);
      }      
      if(xsuccs.size() > 1){
	for(vector<GRCNode *>::iterator i = xsuccs.begin();
	    i != xsuccs.end(); i++)
	  *f >> *i; // Link f to xcopy[*i][x]
	Link(n, f);
	if(DEBUG) cerr<<"adding a new fork under "<<refmap[n]<<"\n";
      }
      else	
	Link(n, *(xsuccs.begin()));  
      assert(n->successors.size() == 3);
    }
   
    
  }

  GRCNode * Construct(GRCNode *n, set<GRCNode *> scope){
    
    if(!n)
      return NULL;

    if(!contains(scope, n))
      return NULL;
    
    if(DEBUG) cerr<<"constructing "<<refmap[n];

    type = -1;
    current = n;
    n->welcome(*this);  //copy n
    CopyDataLink(n, xcopy[n]);      //copy data links

    if(DEBUG) cerr<<": type = "<<type<<"\n";

    switch(type){
    case T_FORK:
      for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	   i != n->successors.end() ; i++ ){
	GRCNode *succ = Construct(*i, subgraph[*i]);
	if(succ)
	  Link(xcopy[n], succ);
      }
      break;
    case T_TEST:{
      GRCNode *f = new Fork();
      int si = 0;
      vector<GRCNode *> xsuccs;
      vector<GRCNode *>::const_iterator ns_end;
      if(n->successors.size() == 3)
	ns_end = n->successors.end()-1;
      else
	ns_end = n->successors.end();
      //construct succ 0/1 for n[x]
      assert(n->successors.size() > 1);
      for (vector<GRCNode*>::const_iterator i = n->successors.begin();
	   i != ns_end; i++ ){
	GRCNode *succ = Construct(*i, scope);
	Link(xcopy[n], succ);
	if(DEBUG) cerr<<refmap[n]<<"'s succ "<<si++<<" constructed \n";
      }
      //construct succ x for n[x]
      for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	   i != n->successors.end()-1; i++ ){
	GRCNode *succ = Construct(*i, scope);
	if(succ)
	  xsuccs.push_back(succ);
      }
      if(xsuccs.size() > 1){
	for(vector<GRCNode *>::iterator i = xsuccs.begin();
	    i != xsuccs.end(); i++)
	  *f >> *i;
	*xcopy[n] >> f;
      }
      else
	*xcopy[n] >> *(xsuccs.begin());
      assert(xcopy[n]->successors.size() == 3);
      if(DEBUG) cerr<<"new fork ADDed under "<<refmap[n]<<"\n";
      break;
    }
    case T_SYNC:{
      break;
    }
    case T_FINAL_TERM:{
      break;
    }
    default:
      for (vector<GRCNode*>::iterator i = n->successors.begin() ;
	   i != n->successors.end() ; i++ ){
	xcopy[n] = Link(xcopy[n], Construct(*i, scope));
      }
      break;
    }
    return xcopy[n];
  }

  void CheckGraph(GRCNode *n){
    vector<GRCNode *> new_preds;
    if(!n)
      return;
    if(contains(visited, n))
      return;
    visited.insert(n);

    if(DEBUG) cerr<<"clear subgraph under "<<refmap[n]<<"\n";

    if( (!dynamic_cast<Sync *>(n)) && (!dynamic_cast<ExitGRC *>(n))
	&& (n->predecessors.size() > 1)){
      if(DEBUG) cerr<<" it has "<<n->predecessors.size()<<" preds\n";
      for(vector<GRCNode *>::const_iterator i = n->predecessors.begin();
	  i != n->predecessors.end()-1; i++){
	bool conflict = false;
	for(vector<GRCNode *>::const_iterator j = i+1; 
	    j != n->predecessors.end(); j++){
	  if(DEBUG) cerr<<" comparing pred "<<refmap[*i]<<" and " <<refmap[*j]<<"\n";
	  if(LCAisFork(*i, *j)){
	    conflict = true;
	    break;
	  }
	}
	if(conflict){
	  if(DEBUG) cerr<<" pred "<<refmap[*i]<<" is conflict with "
			<<" another pred\n";
	  assert(0);
	  current = n;
	  n->welcome(*this); //copy n, and its successes if need
	  assert(xcopy[n] != n);
	  CopyDataLink(n, xcopy[n]);

	  for(vector<GRCNode *>::iterator k = n->successors.begin();
	      k != n->successors.end(); k++)
	    *xcopy[n] >> *k;

	  for(vector<GRCNode *>::iterator k = (*i)->successors.begin();
	      k != (*i)->successors.end(); k++){
	    if(*k == n)
	      *k = xcopy[n];
	  }

	  xcopy[n]->predecessors.push_back(*i);

	}
	else
	  new_preds.push_back(*i);
      }
      new_preds.push_back(*(n->predecessors.end()-1));
      n->predecessors.swap(new_preds);
      if(DEBUG) cerr<<" now "<<refmap[n]<<" has "<<n->predecessors.size()<<" preds\n";

    }

    for(vector<GRCNode *>::iterator j = n->successors.begin();
	j != n->successors.end(); j++)
      CheckGraph(*j);

  }

  void CheckForks(GRCNode *n){
    vector<GRCNode *> new_succs;

    if(!n)
      return;
    if(contains(visited, n))
      return;
    visited.insert(n);

    if(DEBUG) cerr<<"checking forks under "<<n<<"\n";

    for(vector<GRCNode *>::const_iterator i = n->successors.begin();
	i != n->successors.end(); i++){
      if(DEBUG) cerr<<"checking its succ "<<*i<<"...\n";
      CheckForks(*i);
      if(DEBUG) cerr<<"done\n";
    }

    if(dynamic_cast<Fork *>(n)){
      if(DEBUG) cerr<<"it is a fork\n";
      for(vector<GRCNode *>::iterator i = n->successors.begin();
	  i != n->successors.end(); i++){
	assert(*i);
	if(dynamic_cast<Fork *>(*i) && (*i)->predecessors.size() == 1){
	  if(DEBUG) cerr<<"its succ "<<*i<<" also is a fork\n";
	  new_succs.insert(new_succs.end(), (*i)->successors.begin(),
			   (*i)->successors.end());
	  for(vector<GRCNode *>::iterator j = (*i)->successors.begin();
	      j != (*i)->successors.end(); j++)
	    for(vector<GRCNode *>::iterator k = (*j)->predecessors.begin();
		k != (*j)->predecessors.end(); k++){	    
	      if(*k == *i)
		*k = n;
	    }
	  (*i)->successors.clear();
	  (*i)->predecessors.clear();
	}
	else
	  new_succs.push_back(*i);      
      }
      n->successors.swap(new_succs);
    }
    
  }

  void CopyDataLink(GRCNode *n, GRCNode *m){
    if( (!n) || (!m) || (n==m))
      return;
    assert(m->dataSuccessors.empty());
    assert(m->dataPredecessors.empty());
    for(vector<GRCNode *>::const_iterator i = n->dataSuccessors.begin();
	i != n->dataSuccessors.end(); i++)
      **i << m;
    for(vector<GRCNode *>::const_iterator i = n->dataPredecessors.begin();
	i != n->dataPredecessors.end(); i++)
      *m << *i;	

  }

  bool LCAisFork(GRCNode *n, GRCNode *m){
    set<GRCNode *> ancs;
    set<GRCNode *> black;
    collect_ancs(n, ancs);
    return lcaISfork(m, ancs, black);
  }

  void collect_ancs(GRCNode *n, set<GRCNode *> &ancs){
    if(!n)
      return;
    if(contains(ancs, n))
      return;
    ancs.insert(n);
    
    for(vector<GRCNode *>::iterator i = n->predecessors.begin();
	i != n->predecessors.end(); i++)
      collect_ancs(*i, ancs);
  }

  bool lcaISfork(GRCNode *m, set<GRCNode *> ancs, set<GRCNode *> &black){
    if(!m)
      return false;
    if(contains(black, m))
      return false;
    black.insert(m);
    

    if(contains(ancs, m)){
      if(dynamic_cast<Fork *>(m))
	return true;
    }
    else{
      for(vector<GRCNode *>::iterator i = m->predecessors.begin();
	  i != m->predecessors.end(); i++)
	if(lcaISfork(*i, ancs, black))
	  return true;
	
    }
    return false;
  }

  GRCNode *newEmitX(SignalSymbol *s){
    Emit *emit = new Emit(s, NULL);
    emit->unknown = true;
    GRCNode *ret = new Action(emit);
    return ret;
  }

  GRCNode * Link(GRCNode *n, GRCNode *s){
    if(!n)
      return s;

    *n >> s;
    return n;
  }

  Status visit(Fork &f){
    xcopy[current] = new Fork(f.sync);
    type = T_FORK;
    return Status();
  }
  
  Status visit(Action &act){
    act.body->welcome(*this);
    return Status();
  }

  Status visit(Emit &emit){    
    xcopy[current] = newEmitX(emit.signal);
    type = T_EMIT;
    return Status();
  }

  Status visit(Test &t) {
    assert(current->successors.size() >= 2);
    xcopy[current] = new Test(t.st, t.predicate);
    type = T_TEST;
    return Status();
  }

  Status visit(Switch &s){
    xcopy[current] = new Switch(s.st);
    type = T_SWITCH;
    return Status();
  }

  Status visit(Sync &s){
    xcopy[current] = current; //FIXME: not sure yet
    type = T_SYNC;
    return Status();
  }

  Status visit(Terminate &t){
    if(current->dataSuccessors.size() > 0){
      xcopy[current] = new Terminate(t.code, t.index);
    }
    else{ //final term - does nothing
      xcopy[current] = NULL;
      type = T_FINAL_TERM;
      assert(current->successors.size() == 1);
      assert(dynamic_cast<ExitGRC *>(*(current->successors.begin())));
    }
    return Status();
  }

  Status visit(Enter &e){
    xcopy[current] = NULL;
    type = T_ENTER;
    return Status();
  }

  Status visit(ExitGRC &e) {//FIXME: have to confirm
    xcopy[current] = current;
    return Status();
  }

  Status visit(DefineSignal &d){
    xcopy[current] = NULL;
    return Status();
  }

  Status visit(Assign &a){return Status();}
  Status visit(Exit &e){return Status();}
  Status visit(EnterGRC &e) {return Status();}


};

int main()
{
  try {
    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;

    AST::Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    for ( vector<AST::Module*>::iterator i = mods->modules.begin() ;
	  i != mods->modules.end() ; i++ ) {
      GRCNode::NumMap refmap;
      STNode::NumMap strefmap;

      assert(*i);

      GRCgraph *gf = dynamic_cast<GRCgraph*>((*i)->body);
      assert(gf);  
      GRCNode *top = gf->control_flow_graph;

      AST::SymbolTable *sigs = dynamic_cast<AST::SymbolTable*>((*i)->signals);
      assert(sigs);
      AST::SymbolTable *vars = dynamic_cast<AST::SymbolTable*>((*i)->variables);
      assert(vars);

      //make contructive GRC
      gf->enumerate(refmap,strefmap);

      TriValConstructor triconstructor(top,refmap);
      triconstructor.execute();
    }
    
    IR::XMLostream o(std::cout);
    o << n;
    
  } catch (IR::Error &e) {
    cerr << e.s << endl;
    exit(-1);
  }

  return 0;

}
