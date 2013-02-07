#include "IR.hpp"
#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <set>

using namespace AST;
using namespace std;

class BITMAP{
public:
  unsigned int *bitmap;
  int sz, nwords;

  BITMAP(int sz): sz(sz), nwords(1 + (sz-1)/32)
  {
    bitmap = new unsigned int [nwords];
  }

  BITMAP(int sz, bool val): sz(sz), nwords(1 + (sz-1)/32)
  {
    bitmap = new unsigned int [nwords];

    if (val)
      for(int i = 0; i < nwords; i++){
	bitmap[i] = 0xffffffff;
      }
    else
       for(int i = 0; i < nwords; i++){
	bitmap[i] = 0;
       }
  }
  
  void And(BITMAP *in)
  {
    for(int i = 0; i< nwords; i++)
      bitmap[i] &= in->bitmap[i];
  }

  void Or(BITMAP *in)
  {
    for(int i = 0; i< nwords; i++)
      bitmap[i] |= in->bitmap[i];
  }

  void SetBit(int pos, unsigned int val)
  {
    int ipart,lpart;
    assert(pos < sz);
    ipart = pos / 32;
    lpart = pos % 32;
    bitmap[ipart] |= (val<<lpart);
  }

  int GetBit(int bit)
  {
    int RET;
    RET = (bitmap[bit/32]>>(bit%32)) & 0x1;
    return RET;
  }

  BITMAP *copy()
  {
    BITMAP *RET = new BITMAP(sz,0);
    for (int i = 0; i < nwords; i++)
      RET->bitmap[i] |= bitmap[i];
    return RET;
  }

  void print() //for debug
  {
    unsigned int bitval;

    //cerr<<"BITMAP = ";
    for (int i = 0; i < sz; i++){
      bitval = (bitmap[i/32]>>(i%32)) & 0x1;
      cerr<<bitval;
    }
    cerr<<"\n";
  }
};

class Edge
{
public:
  GRCNode *begin;
  GRCNode *end;
  Edge(GRCNode *begin, GRCNode *end):begin(begin),end(end){}
};

class ExEdge
{
public:
  //GRCNode *head;
  set<GRCNode *> descends;
  set<Edge *> exedges;

  //ExEdge(GRCNode *head):head(head)
  ExEdge()
  {
  }

  void AddDesc(GRCNode *n)
  {
    if(n)
      descends.insert(n);
  }

  void AddDesc(set<GRCNode *> nset)
  {
    descends.insert(nset.begin(), nset.end());   
  }

  void FindExedges(GRCNode *head)
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    for(i = descends.begin(); i != descends.end(); i++){
      if((*i) == head)
	continue;
      for (j = (*i)->predecessors.begin(); j != (*i)->predecessors.end(); j++){
	assert(*j);
	if (descends.count(*j)==0){
	  Edge *edge = new Edge(*j,*i);
	  exedges.insert(edge);
	}
      }
    }
  }

};

class EEC{

  GRCNode *top;
  SymbolTable *sigs;
  GRCNode::NumMap &refmap; 
  GRCNode *toppdg;
  EnterGRC *entergrc;

  set<GRCNode *> visited;
  set<GRCNode *> par_visit;
  vector<GRCNode *> topo;
  map<GRCNode *, int> bitpos;
  map<GRCNode *, BITMAP *> region;
  map<GRCNode *, BITMAP *> eec;
  map<GRCNode *, set<GRCNode *> > dpsnodes;
  map<GRCNode *, vector<GRCNode *> > conn_pred;
  map<GRCNode *, vector<GRCNode *> > conn_succ;
  map<GRCNode *, GRCNode *> start; //start of a subgraph under n
  map<GRCNode *, vector<GRCNode *> > ends; //ends of a subgraph under n
  
  int debug;
  int pos_cnt;
  int var_cnt; //counter for naming new guard-variable when making cut
  int mxnode;

public:
  EEC(GRCNode *top, SymbolTable *sigs, GRCNode::NumMap &refmap, int mxnode): top(top), sigs(sigs), refmap(refmap), mxnode(mxnode)
  {
    debug = 0;
    
    entergrc = dynamic_cast<EnterGRC*>(top);
    assert(entergrc);
    assert(entergrc->successors.size() == 1);
    toppdg = dynamic_cast<GRCNode*>(entergrc ->successors[0]); 
    assert (toppdg);
  }

  void execute()
  {   
    DFS(entergrc);

    // Starts here?
    CopyConn();
    RemoveConn();

    visited.clear();
    ReplaceNull2Nop(entergrc);

    //due to Ferrante's alg, only fork nodes may have multi-parents
    ParForking();

    RecoverConn();

    visited.clear();
    pos_cnt = 0;
    DFSCounter(entergrc);
    //printTopo();

    ComputeRegion();

    visited.clear();
    DFSEec(entergrc);
    //BitmapPrint();

    var_cnt = 0;
    OrderChildren();

    RemoveConn();

    visited.clear();    
    DFSRemoveForks(entergrc);

    ReplaceNop2Null();
    // Ends here?
    
  }

  void removeDPS()
  {
    set<GRCNode *>::iterator i;

    for (i = visited.begin(); i != visited.end(); i++){

      if (!dynamic_cast<Terminate *>(*i))
	(*i)->dataSuccessors.clear();
      
      if (!dynamic_cast<Sync *>(*i))
	(*i)->dataPredecessors.clear();
    }
  }

  void CopyConn()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    for(i = visited.begin(); i != visited.end(); i++){
      assert(*i);
      for(j = (*i)->successors.begin(); j != (*i)->successors.end(); j++){
	conn_succ[*i].push_back(*j);
	if(*j)
	  conn_pred[*j].push_back(*i);
      }
    } 
  }

  void PrintConn()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    cerr<<"--------Conn are-------\n";
    for(i = visited.begin(); i != visited.end(); i++){
      cerr<<"N"<<refmap[*i]<<"-> ";
      for(j = conn_succ[*i].begin(); j != conn_succ[*i].end(); j++)
	cerr<<refmap[*j]<<" ";
      cerr<<"\n";
      cerr<<"N"<<refmap[*i]<<"<- ";
      for(j = conn_pred[*i].begin(); j != conn_pred[*i].end(); j++)
	cerr<<refmap[*j]<<" ";
      cerr<<"\n";
    }
  }

  void RemoveConn()
  {
    set<GRCNode *>::iterator i;
    for(i = visited.begin(); i != visited.end(); i++){
      (*i)->successors.clear();
      (*i)->predecessors.clear();
    }
  }

  void RecoverConn()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;

    for(i = visited.begin(); i != visited.end(); i++){
      for(j = conn_succ[*i].begin(); j != conn_succ[*i].end(); j++){
	(*i)->successors.push_back(*j);
	(*j)->predecessors.push_back(*i);
      }
    }
    conn_succ.clear();
    conn_pred.clear();
  }

  void ReplaceNull2Nop(GRCNode *n)
  {
    assert(n);

    if (visited.find(n) != visited.end())
      return;
    visited.insert(n);
    
    vector<GRCNode *>::iterator i;
    for (i = conn_succ[n].begin(); i != conn_succ[n].end(); i++)
      if (!*i){
	*i = new Nop();
	conn_pred[*i].push_back(n);
	refmap[*i] = mxnode++;
	visited.insert(*i);
      }
      else
	ReplaceNull2Nop(*i);
  }

  /*void ReplaceNop2Null(GRCNode *n)
  {
    if(debug) cerr<<"N"<<refmap[n]<<"\n";
    assert(n);
    if (visited.find(n) != visited.end())
      return;
    visited.insert(n);
    if(debug) cerr<<"after visited insert\n";
    if(debug) cerr<<"for N"<<refmap[n]<<"'s successors: \n";
    vector<GRCNode *>::iterator i;
    for (i = n->successors.begin(); i != n->successors.end(); i++){
      assert(*i);
      GRCNode *t = dynamic_cast<Nop *>(*i);
      if (t){
	if(debug) cerr<<refmap[*i]<<" is a nop\n";
	assert((*i)->successors.size() == 0);
	//assert((*i)->predecessors.size() <= 1); //!!!!tcint sth wrong here
	(*i)->predecessors.clear();
	*i = NULL;
      }
      else{
	if(debug) cerr<<refmap[*i]<<" is NOT a nop\n";
	ReplaceNop2Null(*i);
      }
    }      
    if(debug) cerr<<"finished N"<<refmap[n]<<"\n";
    }*/

  void ReplaceNop2Null()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;

    conn_succ.clear();

    for (i = visited.begin(); i != visited.end(); i++){
      if (!dynamic_cast<Nop *> (*i)){
	for (j = (*i)->successors.begin(); 
	     j != (*i)->successors.end(); j++)
	  if (dynamic_cast<Nop *> (*j)){
	    int sz = (int)(*j)->successors.size();
	    assert(sz < 2);
	    if (sz == 0)	    
	      conn_succ[*i].push_back(NULL);
	    else //sz == 1
	      conn_succ[*i].push_back((*j)->successors[0]);
	  }
	  else
	    conn_succ[*i].push_back(*j);
      }
    }

    for (i = visited.begin(); i != visited.end(); i++){
      (*i)->successors.clear();
      (*i)->predecessors.clear();
    }

    for (i = visited.begin(); i != visited.end(); i++){
       if (dynamic_cast<Nop *> (*i))
	continue;
       if (conn_succ.find(*i) == conn_succ.end())
	 continue;
       for (j = conn_succ[*i].begin();
	    j != conn_succ[*i].end(); j++){
	 assert(!dynamic_cast<Nop *> (*j));
	 (*i)->successors.push_back(*j);
	 if (*j)
	   (*j)->predecessors.push_back(*i);
       }
    }
	  
  }

  void printTopo()
  {
    vector<GRCNode *>::iterator i;

    cerr<<"TOPO: ";
    for (i = topo.begin(); i != topo.end(); i++)
      cerr<<refmap[*i]<<" ";
    cerr<<"\n";
  }

  void DFSCounter(GRCNode *n)
  {
    vector<GRCNode *>::iterator i;

    if(!n) return;
    
    if(visited.count(n)>0) 
      return;
    visited.insert(n);

    bitpos[n] = pos_cnt;
    pos_cnt++;
    for (i = n->successors.begin(); i != n->successors.end(); i++){
      DFSCounter(*i);
      if(dpsnodes.count(*i) > 0)
	dpsnodes[n].insert(dpsnodes[*i].begin(), dpsnodes[*i].end());
    }

    if ((n->dataSuccessors.size()>0)||(n->dataPredecessors.size()>0))
      dpsnodes[n].insert(n);

    topo.insert(topo.begin(),n);
  }

  void topoDFS(GRCNode *n)
  {
    vector<GRCNode *>::iterator i;
    
    if(!n) return;
    
    if(visited.count(n)>0) 
      return;
    
    visited.insert(n);
    
    for (i = n->successors.begin(); i != n->successors.end(); i++){
      topoDFS(*i);
    }
    
    if (n->dataSuccessors.size()>0){
      for (i = n->dataSuccessors.begin();
	   i != n->dataSuccessors.end(); i++)
	topoDFS(*i);
    }
    
    topo.insert(topo.begin(),n);
    
    return;
  }

  void DFS(GRCNode *n)
  {
    vector<GRCNode *>::iterator i;

    if(!n) return;
    
    if(visited.count(n)>0) 
      return;
    visited.insert(n);
    for (i = n->successors.begin(); i != n->successors.end(); i++)
      DFS(*i);
  }

  void ParForking()
  {
    set<GRCNode *>::iterator i;

    for(i = visited.begin(); i != visited.end(); i++){

      if (TypeCheck(*i) != 3){ //if it's not a fork
	if (conn_pred[*i].size() > 1){
	  assert(TypeCheck(*i) != 4); //it can not be a nop - null node
	  if (debug)
	    cerr<<"parents forking: N"<<refmap[*i];

	  GRCNode *new_fork = new Fork();
	  visited.insert(new_fork);
	  InsertPar(new_fork, *i);
	  refmap[new_fork] = mxnode++;

	  if (debug)
	    cerr<<" with new parent N"<<refmap[new_fork]<<"\n";
	}
      }
    }
  }

  //insert new_par btw ch & ch's predecessors
  void InsertPar(GRCNode *new_par, GRCNode *ch)
  {
    vector<GRCNode *>::iterator i,j;
    
    conn_succ[new_par].push_back(ch);
    for (i = conn_pred[ch].begin(); i != conn_pred[ch].end(); i++){
      conn_pred[new_par].push_back(*i);
      for (j = conn_succ[*i].begin(); j != conn_succ[*i].end(); j++)
	if ((*j) == ch)
	  (*j) = new_par;
    }
    conn_pred[ch].clear();
    conn_pred[ch].push_back(new_par);
  }

  void BitmapPrint()
  {
    set<GRCNode *>::iterator i;
    set<GRCNode *>::iterator j;
    //set<Edge *>::iterator j;
    int t;
    //set<GRCNode *>::iterator k;

    for(i = visited.begin(); i != visited.end(); i++){
      cerr<<"N"<<refmap[*i]<<"\n";
      cerr<<"  reg: ";
      for (t = 0; t < pos_cnt; t++){
	if (region[*i]->GetBit(t) == 1)
	  for (j = visited.begin(); j != visited.end(); j++){
	    if (t == bitpos[*j]){
	      cerr<<refmap[*j]<<" ";
	      break;
	    }
	  }
      }
      cerr<<"\n";

      cerr<<"  eec: ";
      for (t = 0; t < pos_cnt; t++){
	if (eec[*i]->GetBit(t) == 1)
	  for (j = visited.begin(); j != visited.end(); j++){
	    if (t == bitpos[*j])
	      cerr<<refmap[*j]<<" ";
	  }
      }
      cerr<<"\n";
      
      /*cerr<<"exedges: ";
      
      for(j = relation[*i]->exedges.begin(); j != relation[*i]->exedges.end(); j++){
	cerr<<refmap[(*j)->begin]<<"->"<<refmap[(*j)->end]<<" ";
      }
      cerr<<"\n";

      cerr<<"descends: ";
      for (k = relation[*i]->descends.begin(); k != relation[*i]->descends.end(); k++){
	cerr<<refmap[*k]<<" ";
      }
      cerr<<"\n";*/
    }
  }

  void DFSRemoveForks(GRCNode *n)
  {
    vector<GRCNode *>::iterator i,j;
    
    if (visited.count(n)>0) 
      return;
    visited.insert(n);

    if(debug) cerr<<"N"<<refmap[n]<<":";

    if (conn_succ.find(n) == conn_succ.end()){
      start[n] = n;
      ends[n].push_back(n);
      if(debug) {
	cerr<<" is a leaf with ";
	cerr<<"start "<<refmap[start[n]]<<", ";
	cerr<<"ends "<<" with ";
	PrintVec(ends[n]);
      }
      return;
    }

    if (conn_succ[n].size() == 0){
      start[n] = n;
      ends[n].push_back(n);
      if(debug) {
	cerr<<" is a leaf with ";
	cerr<<"start "<<refmap[start[n]]<<", ";
	cerr<<"ends "<<" with ";
	PrintVec(ends[n]);
      }
      return;
    }

    //if n is a fork, 
    //  start[n] = start[n->successors[0]], 
    //  ends[n] = ends[n->successors[size-1]]
    if (dynamic_cast<Fork *>(n)){
      if(debug) {
	cerr<<"is a fork with successors: ";
	for (i = conn_succ[n].begin(); i < conn_succ[n].end(); i++)
	  cerr<<refmap[*i]<<" ";
	cerr<<"\n";
	cerr<<"for "<<refmap[n]<<"'s successor "
	    <<refmap[*conn_succ[n].begin()]<<"\n";
      }
      DFSRemoveForks(*conn_succ[n].begin());
      if(debug) cerr<<"succ size = "<<conn_succ[n].size()<<"\n";
      for (i = conn_succ[n].begin()+1; i < conn_succ[n].end(); i++){
	if(debug) cerr<<"for "<<refmap[n]<<"'s successor "<<refmap[*i]<<"\n";
	DFSRemoveForks(*i);

	assert(ends.find(*(i-1))!=ends.end());
	if(debug) 
	  cerr<<"try link "<<refmap[*i]<<"'s start with "<<refmap[*(i-1)]
	      <<"'s ends\n";
	for (j = ends[*(i-1)].begin(); j != ends[*(i-1)].end(); j++){
	  Link(*j, start[*i]);
	}	
      }
      
      start[n] = start[*(conn_succ[n].begin())];
      ends[n] = ends[*(conn_succ[n].end()-1)];

    }
    else{
      if(debug) cerr<<"is NOT a fork\n";
      start[n] = n;

      for (i = conn_succ[n].begin(); i < conn_succ[n].end(); i++){
	if(debug) cerr<<"for "<<refmap[n]<<"'s successor "<<refmap[*i]<<"\n";
	DFSRemoveForks(*i);
	if(debug) 
	  cerr<<"try link "<<refmap[n]<<" with "<<refmap[*i]
	      <<"'s start\n";
	Link(n,start[*i]);
	ends[n].insert(ends[n].end(),ends[*i].begin(), ends[*i].end());
      }
    }
    if(debug) {
      cerr<<"Start N"<<refmap[n]<<" with "<<refmap[start[n]]<<"\n";
      cerr<<"End N"<<refmap[n]<<" with ";
      PrintVec(ends[n]);
    }

  }

  void PrintVec(vector<GRCNode *> v)
  {
    vector<GRCNode *>::iterator i;
    for (i = v.begin(); i != v.end(); i++)
      cerr<<refmap[*i]<<" ";
    cerr<<"\n";
  }

  void Link(GRCNode *par, GRCNode *n)
  {
    vector<GRCNode *>::iterator i;
    bool found = false;

    if ((dynamic_cast<Switch *>(par)) 
	|| (dynamic_cast<Test *>(par)) 
	|| (dynamic_cast<Sync *>(par))){
    }
    else{// if not switch or test or sync, can't have multiple same child, dont add it
      for (i = par->successors.begin(); i != par->successors.end(); i++){
	if ((*i) == n){
	  found = true;
	  break;
	}
      }
    }
    if (!found){      
      par->successors.push_back(n);
      n->predecessors.push_back(par);
      if(debug) cerr<<"CLinked "<<refmap[par]<<"->"<<refmap[n]<<"\n";
    }	
  }
  
  void OrderChildren()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator x,y;
    bool inserted;

    for (i = visited.begin(); i != visited.end(); i++){      
      if(debug) cerr<<"For N"<<refmap[*i]<<":\n";
      if ((dynamic_cast<Fork *>(*i)) && ((*i)->successors.size()>1)){

	vector<GRCNode *> ordered;
	map<GRCNode *, SignalSymbol *> cuthead;

	if(debug) cerr<<"A Fork N"<<refmap[*i]<<":-----------\n";
	
	ordered.push_back(*((*i)->successors.begin()));
	for (x = (*i)->successors.begin()+1; x < (*i)->successors.end(); x++){

	  inserted = false;

	  for (y = ordered.begin(); y != ordered.end(); y++){
	    if(debug) cerr<<"comp N"<<refmap[*x]<<"-N"<<refmap[*y]<<"\n";
	    switch (OrderCheck(*x,*y)){
	    case 1://-
	      if(debug)  cerr<<"c1\n";
	      if (DPSOn(*y,*x)){
		y = ordered.insert(y,*x); 
		inserted = true; //set done
	      }
	      break;
	    case 2:// y < x
	      if(debug) cerr<<"c2\n";
	      if (DPSOn(*y,*x)){ //a cut on y
		CutVarSet(*y,cuthead);
		y = ordered.insert(y,*x);
		inserted = true;
	      }
	      //else, insert it later
	      break;
	    case 3:// y > x
	      if(debug) cerr<<"c3\n";
	      if (DPSOn(*x, *y)){
		CutVarSet(*x, cuthead);//make a cut but insert it later
	      }
	      else{
		y = ordered.insert(y,*x); 
		inserted = true;
	      }
	      break;
	    case 4:// y <> x
	      if(debug) cerr<<"c4\n";
	      if (DPSOn(*y,*x)){ // a cut on y
		CutVarSet(*y, cuthead);
		y = ordered.insert(y,*x);
		inserted = true;
	      }
	      else { 
		/*// a cut on x but insert it later
		  CutVarSet(*x, cuthead);*/	       
		CutVarSet(*y,cuthead); //a cut on y
		y = ordered.insert(y,*x);
		inserted = true;
	      }
	      break;
	    case 5:
	      assert(0);
	      break;
	    }
	    if (inserted)
	      break;
	  }
	  if(!inserted)
	    ordered.push_back(*x);
	}

	assert(ordered.size() == (*i)->successors.size());
	bool cutted = false;
	int j = 0;
	while (j < (int)ordered.size()){
	  // if ordered[j] not need be cutted and no node ordered bf has been cutted
	  if ((cuthead.count(ordered[j]) == 0) && (!cutted)){	 
	    conn_succ[*i].push_back(ordered[j]);
	    conn_pred[ordered[j]].push_back(*i);
	  }
	  else {
	    cutted = true;
	    CutVarSet(ordered[j], cuthead);
	    cerr<<"Make a cut on "<<refmap[ordered[j]]<<"\n";
	    GRCNode *new_set_s = NewSetVar(cuthead[ordered[j]],1);
	    refmap[new_set_s] = mxnode++;  
	    ordered.insert(ordered.begin(), new_set_s);
	    j++;
	    GRCNode *new_test_s = NewTester(cuthead[ordered[j]],ordered[j]);
	    refmap[new_test_s] = mxnode++;

	    conn_succ[*i].push_back(new_test_s);
	    conn_pred[new_test_s].push_back(*i);	 
	  }
	  j++;
	}
      }
      else{
	for (x = (*i)->successors.begin(); x != (*i)->successors.end(); x++){
	  conn_succ[*i].push_back(*x);
	  conn_pred[*x].push_back(*i);
	}
      }
    }
  }

  void CutVarSet(GRCNode *n, map<GRCNode *, SignalSymbol *> &cuthead)
  {
    if (cuthead.count(n) == 0) {//never been cutted before
      SignalSymbol *new_s = NewVariable();
      assert(new_s);
      sigs->enter(new_s);
      cuthead[n] = new_s;
    }
  }

  GRCNode *NewSetVar(SignalSymbol *s, int val)  
  {
    assert(s);
    Emit *em = new Emit(s, NULL);
    return (new Action(em));
  }

  GRCNode *NewTester(SignalSymbol *s, GRCNode *succn)
  {
    GRCNode *RET;
    
    assert(succn);
    assert(s);
    LoadSignalExpression *lde = new LoadSignalExpression(NULL, s);
    RET = new Test(NULL, lde);
    GRCNode *nop = new Nop();
    conn_succ[RET].push_back(nop);
    conn_succ[RET].push_back(succn);
    conn_pred[succn].push_back(RET);
    conn_pred[nop].push_back(RET);

    return RET;
  }

  //FIXME: it's signal currently, should be changed to varialbe
  SignalSymbol *NewVariable()
  {
    char new_sig_name[32];
    char *pref = "_EECCUT_VAR";

    cerr<<"A CUT!!!!!!!!!!!!!!!!\n";
    assert(0);
    sprintf(new_sig_name, "%s%d", pref, var_cnt);
    var_cnt++;

    return (new SignalSymbol(new_sig_name, NULL, SignalSymbol::Input,NULL,NULL, NULL));
  }

  //return the order type of two nodes: 1 -, 2 Y<X, 3 Y>X, 4 Y<>X, 5 N/A
  int OrderCheck(GRCNode *x, GRCNode *y)
  {
    switch (TypeCheck(y)){
    case 1:
      switch (TypeCheck(x)){
      case 1:
	return 1;
      case 2:
	if (eec[x]->GetBit(bitpos[x]) == 1)
	  return 1;
	return 2;
      case 3:
	if (eec[y]->GetBit(bitpos[x]) == 1)
	  return 2;
	return 5;
      }
      break;
    case 2:
      switch (TypeCheck(x)){
      case 1:
	if (eec[y]->GetBit(bitpos[y]) == 1)
	  return 1;
	return 3;
      case 2:
	if ((eec[y]->GetBit(bitpos[y]) == 1) 
	    && (eec[x]->GetBit(bitpos[x]) == 1))
	  return 1;
	if (eec[y]->GetBit(bitpos[y]) == 1)
	  return 2;
	if (eec[x]->GetBit(bitpos[x]) == 1)
	  return 3;
	//cerr<<"here41\n";
	return 4;
      case 3:
	if (eec[y]->GetBit(bitpos[x]) == 1)
	  return 2;
	if (eec[y]->GetBit(bitpos[y]) == 1)
	  return 5;
	//cerr<<"here42\n";
	return 4;
      }
      break;
    case 3:
      switch (TypeCheck(x)){
      case 1:
	if (eec[x]->GetBit(bitpos[y]) == 1)
	  return 3;
	return 5;
      case 2:
	if (eec[x]->GetBit(bitpos[y]) == 1)
	  return 3;
	if (eec[x]->GetBit(bitpos[x]) == 1)
	  return 5;
	//cerr<<"here43\n";
	return 4;
      case 3:
	if (eec[x]->GetBit(bitpos[y]) == 1) {
	  if (eec[y]->GetBit(bitpos[x]) == 1)
	    return 1;
	  else return 3;
	} 
	if (eec[y]->GetBit(bitpos[x]) == 1)
	  return 2;
	//cerr<<"here44\n";
	return 4;
      }
    }
    assert(0);//unknow case
    return 5;
  }
    
  //test if x has data dependence on y: y--->x
  bool DPSOn(GRCNode *x, GRCNode *y)
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;

    for (i = dpsnodes[x].begin(); i != dpsnodes[x].end(); i++){
      for (j = (*i)->dataPredecessors.begin();
	   j != (*i)->dataPredecessors.end(); j++)
	if (dpsnodes[y].count(*j)> 0)
	  return true;
    }
    return false;
  }

  void ComputeRegion()
  {
    vector<GRCNode *>::iterator i,j,k;
    BITMAP *par_reg;

    for (i = topo.begin(); i != topo.end(); i++){
      //cerr<<"N"<<refmap[*i]<<"\n";
      //relation[*i]= new ExEdge(); //need later
      if ((*i) == entergrc){
	region[*i] = new BITMAP(pos_cnt,0);
      }
      else {
	assert((*i)->predecessors.size()>0);
	BITMAP *RET = new BITMAP(pos_cnt,1);
	//cerr<<" init ";RET->print();
	for (j = (*i)->predecessors.begin(); j != (*i)->predecessors.end(); j++){	  
	  //cerr<<" AND par "<<refmap[*j]<<"\n";
	  assert(region.count(*j)>0);
	  if(dynamic_cast<Fork *>(*j)){
	    par_reg = region[*j]->copy();
	    for (k = (*j)->successors.begin(); k != (*j)->successors.end(); k++){
	      par_reg->SetBit(bitpos[*k],1);
	    }
	    //cerr<<"its a fork\n";
	  }
	  else {
	    par_reg = region[*j];
	    //cerr<<"its NOT a fork\n";
	  }
	  //cerr<<"par "<<refmap[*j]<<"->"<<refmap[*i]<<" ";par_reg->print();
	  RET->And(par_reg);
	  //cerr<<"aft AND    ";RET->print();
	}
	region[*i] = RET;   
	//cerr<<"region "<<refmap[*i]<<"  ";region[*i]->print();
      }
    }
  }

  void DFSEec(GRCNode *n)  
  {
    if(!n) return;
    if(visited.count(n)>0) return;
    visited.insert(n);
    
    if (n->successors.size() == 0){
      eec[n] = region[n];
      return;
    }

    BITMAP *tmp = new BITMAP(pos_cnt,1);
    for (vector<GRCNode *>::iterator i = n->successors.begin();
	 i != n->successors.end(); i++){
      if (*i){
	DFSEec(*i);
	
	assert(eec.count(*i)>0);
	tmp->And(eec[*i]);
      }
    }
    eec[n] = tmp;
  }

  int TypeCheck(GRCNode *n)
  {
    if ((dynamic_cast<Test *>(n))
	|| (dynamic_cast<Sync *>(n)) 
	|| (dynamic_cast<Switch *>(n)))
      return 2;

    if (dynamic_cast<Fork *>(n))
      return 3;

    if (dynamic_cast<Nop *>(n))
      return 4;

    if(n->successors.size() != 0){
      assert(dynamic_cast<EnterGRC *>(n));
    }
    return 1;
  }
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

      assert(*i);

      GRCgraph *gf = dynamic_cast<GRCgraph*>((*i)->body);
      assert(gf);  
      GRCNode *top = gf->control_flow_graph;

      AST::SymbolTable *sig = dynamic_cast<AST::SymbolTable*>((*i)->signals);
      assert(sig);

      GRCNode::NumMap refmap;
      STNode::NumMap strefmap;
      
      int mxnode = gf->enumerate(refmap,strefmap);

      EEC eec_computer(top, sig, refmap, mxnode);
      eec_computer.execute();

    }
    
    IR::XMLostream o(std::cout);
    o << n;
  } catch (IR::Error &e) {
    cerr << e.s << endl;
    exit(-1);
  }

  return 0;

}
