#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include "IR.hpp"
#include "AST.hpp"

using namespace AST;
using namespace std;

const int MININT=std::numeric_limits<int>::min();
// const int MININT=(-1)<<(2^32-1);

//for schedule
class DPSINFO
{
public:
  map<GRCNode *,set<GRCNode *> > dpsnodes; //successors & correspond dps nodes
  bool selfdps; // whether the node itself has dps

  DPSINFO(bool selfdps):selfdps(selfdps)
  {
  }
  
  void insert(GRCNode *n, DPSINFO *ndps)
  {
    map<GRCNode *,set<GRCNode *> >::iterator i;
    for (i = ndps->dpsnodes.begin(); i != ndps->dpsnodes.end(); i++){
      dpsnodes[n].insert((*i).second.begin(), (*i).second.end());
    }
    if (ndps->selfdps)
      dpsnodes[n].insert(n);
  }

  GRCNode *FindBr(GRCNode *n)
  {
    map<GRCNode *,set<GRCNode *> >::iterator i;
    GRCNode *RET = NULL;
    for (i = dpsnodes.begin(); i != dpsnodes.end(); i++){
      if((*i).second.find(n) != (*i).second.end())
	return ((*i).first);
    }
    return RET;
  }

  set<GRCNode *> FindSet(GRCNode *n)
  {
    set<GRCNode *> RET;

    if (dpsnodes.find(n) != dpsnodes.end())
      RET = dpsnodes[n];
    return RET;
  }

  set<GRCNode *> GetSet()
  {
    set<GRCNode *> RET;
    map<GRCNode *,set<GRCNode *> >::iterator i;
    for (i = dpsnodes.begin(); i != dpsnodes.end(); i++)
      RET.insert((*i).second.begin(), (*i).second.end());
    return RET;
    
  }

};

class PRIORITY
{
public:
  int prime; //primary priority - defined by input-output dps btw branches
  int second; //secondary priority - defined by parent-child constrains
  int third;  //third priority - defined by arbitary input-output dps

  PRIORITY(int prime, int second, int third):prime(prime), second(second), third(third)
  {
  }

  // 0: = p, 1: > p, -1: < p
  int Compare(PRIORITY *p)
  {
    if ((prime == p->prime) && (second == p->second) && (third == p->third))
      return 0;
    if (prime > p->prime) 
      return 1;
    else if (second > p->second)
      return 1;
    else if (third > p->third)
      return 1;
    return -1;
  }

  void Set(int p1, int p2, int p3){
    prime = p1;
    second = p2;
    third = p3;
  }
};


class PDG2CCFG
{
  GRCNode *top;  //EnterGRC
  SymbolTable *symtable;
  GRCNode::NumMap &refmap;
  int mxnode;

  int newvar_counter;  //new signals counter
  const char *newvar_prefix; //unique prefix for new signals

  set<GRCNode *> visited;
  int debug;

  vector<GRCNode *> scheduled_list;

  //for cut
  map<GRCNode *, VariableSymbol *> varmap;
  map<GRCNode *, GRCNode *> lcopy;
  map<GRCNode *, set<GRCNode *> >cached;
  map<GRCNode *, GRCNode *> active;
  set<GRCNode *> csted;

  map<GRCNode *, GRCNode *> tvmap;
  map<GRCNode *, set<GRCNode *> > desc; //descendent
  map<GRCNode *, vector<GRCNode *> > conn_succ;
  map<GRCNode *, int> conn_prednum;
  
public:
  PDG2CCFG(GRCNode *top, SymbolTable *symtable, GRCNode::NumMap &refmap, int mxnode):
    top(top),symtable(symtable), refmap(refmap), mxnode(mxnode), newvar_prefix("_DPSCUT_VAR")
  {
    debug = 0;

    newvar_counter = 1;
//    newvar_prefix = "_DPSCUT_VAR";

  }

  int Execute()
  {
    ReplaceNull2Nop(top);
    //MergeForkFlow();

    CollectDesc();
    ClearUselessDps();

    ScheduleList();

    visited.clear();
    Restructure();
   
    Fucing();

    // cerr<<"Used "<<newvar_counter<<" vars for cut\n";
    
    if(debug) cerr<<"here!\n";
    RecoverConn();
    if(debug) cerr<<"here2\n";
   
    visited.clear();
    ReplaceNop2Null(top);

    return mxnode;
  }

 /***************************************
   Functions for Ordering nodes successors
   and Making schedule list
  ****************************************/
  void ScheduleList()
  {
    map<GRCNode *, vector<GRCNode *> > vorder_succ; //connection, for schedule only
    map<GRCNode *, DPSINFO *> dpsinfo; //dps nodes informations    

    visited.clear();
    OrderingDFS(top, vorder_succ, dpsinfo);

    if (debug)
      PrintDPSInfo(dpsinfo);

    if (debug)
      PrintOrderSucc(vorder_succ);

    visited.clear();
    TopoVisit(vorder_succ);

    if (debug)
      PrintScheduledList();
    
  }
  
  void TopoVisit(map<GRCNode *, vector<GRCNode *> > &vorder_succ)
  {
    vector<GRCNode *> list_normal;
    set<GRCNode *> list_forks;

    topoDFS(top, list_normal, list_forks, vorder_succ);
    scheduled_list = mergeList(list_normal, list_forks); 
  }

  void topoDFS(GRCNode *n, vector<GRCNode *> &topo_comm, set<GRCNode *> &topo_fork,
	       map<GRCNode *,vector<GRCNode *> > &vorder_succ)
  {
    vector<GRCNode *>::iterator i;
  
    if(!n) return;
    
    if(visited.count(n)>0) 
      return;

    visited.insert(n);

    for (i = vorder_succ[n].begin(); i != vorder_succ[n].end(); i++){
      topoDFS(*i, topo_comm, topo_fork, vorder_succ);
    }

    if (n->dataSuccessors.size()>0){
      for (i = n->dataSuccessors.begin();
	   i != n->dataSuccessors.end(); i++)
	topoDFS(*i, topo_comm, topo_fork, vorder_succ);
    }
    
    if (dynamic_cast<Fork *>(n)) 
      //forks will be inserted later, ahead of its first successor
      topo_fork.insert(n);
    else
      topo_comm.push_back(n);

  }

  //insert forks to the topo list while reverse the topo
  vector<GRCNode *> mergeList(vector<GRCNode *> topo_comm, set<GRCNode *> topo_fork)
  {
    vector<GRCNode *> RET;
    vector<GRCNode *>::iterator i,j;

    for (i = topo_comm.end()-1; i >= topo_comm.begin(); i--){
      for (j = (*i)->predecessors.begin(); j != (*i)->predecessors.end(); j++){
	if (topo_fork.count(*j) != 0){
	  RET.push_back(*j);
	  topo_fork.erase(*j);
	}
      }	
      RET.push_back(*i);
    }
    return RET;
  }
  
  void OrderingDFS(GRCNode *n, map<GRCNode *, vector<GRCNode *> > &vorder_succ,
		    map<GRCNode *, DPSINFO *> &dpsinfo)
  {
    //if (!n) return;
    assert(n);
    if (visited.count(n) >0)
      return;
    visited.insert(n);

    if ((n->dataSuccessors.size()>0)||(n->dataPredecessors.size()>0))
      dpsinfo[n] = new DPSINFO(true);
    else 
      dpsinfo[n] = new DPSINFO(false);

    for (vector<GRCNode *>::iterator i = n->successors.begin();
	 i != n->successors.end(); i++){
      OrderingDFS(*i, vorder_succ, dpsinfo);
      dpsinfo[n]->insert(*i,dpsinfo[*i]);
    }
    
    OrderChildren(n, dpsinfo, vorder_succ);

  }
  
  void OrderChildren(GRCNode *n, map<GRCNode *, DPSINFO *> dpsinfo,
		     map<GRCNode *, vector<GRCNode *> > &vorder_succ)
  {
    vector<GRCNode *>::iterator i,k;
    set<GRCNode *>::iterator j,t;

    //higher a branch's priority is, quicker it will be visited
    map<GRCNode *, PRIORITY *> priority;

    assert(n);
    assert(vorder_succ[n].size() == 0);

    if (n->successors.size()<2){
       for (i = n->successors.begin(); i != n->successors.end(); i++)
	 vorder_succ[n].push_back(*i);
      return;
    }
    
    //compute priorities
    for (i = n->successors.begin(); i != n->successors.end(); i++){
      vorder_succ[n].push_back(*i);

      priority[*i] = new PRIORITY(0,0,0);

      set<GRCNode *> dpsset = dpsinfo[n]->FindSet(*i);   
      if (dpsset.size() == 0) {//no dps related
	priority[*i]->Set(MININT, MININT, MININT);
	continue;
      }

      for (j = dpsset.begin(); j != dpsset.end(); j++){

	int a = 0, b = 0;

	for (k = (*j)->dataPredecessors.begin(); 
	     k != (*j)->dataPredecessors.end(); k++){
	  GRCNode *ret = dpsinfo[n]->FindBr(*k);
	  if (ret){
	    priority[*i]->prime++;	  
	    if (ret != (*i))
	      a++;	      
	  }
	  priority[*i]->third--;
	}

	for (k = (*j)->dataSuccessors.begin(); 
	     k != (*j)->dataSuccessors.end(); k++){
	  if (dpsinfo[n]->FindBr(*k))
	    priority[*i]->prime--;	  
	  priority[*i]->third++;
	}
	
	if (a != 0){
	  GRCNode *par = (*((*j)->predecessors.begin()));
	  assert(par);
	  //j is any node that's under branch i with dps, including i
	  set<GRCNode *> dps_under_j = dpsinfo[*j]->GetSet();	  
	  if (dpsinfo[*j]->selfdps)
	    dps_under_j.insert(*j);
	  
	  //t is any node that's under j with dps, including j
	  for(t = dps_under_j.begin(); t != dps_under_j.end(); t++){
	    for (k = (*t)->dataSuccessors.begin(); 
		 k != (*t)->dataSuccessors.end(); k++){
	      GRCNode *ret = dpsinfo[n]->FindBr(*k);
	      if (ret)
		if (ret != (*i))
		  b++;
	    }
	  }	
	  priority[*i]->second -= a*b;
	}
      }
    }

    //order successors    
    for (i = vorder_succ[n].end()-1; i >= vorder_succ[n].begin()+1; i--){      
      for (k = vorder_succ[n].begin(); k <= (i-1); k++){
	if (priority[*k]->Compare(priority[*(k+1)]) == -1){
	  GRCNode *tmp = *k;
	  *k = *(k+1);
	  *(k+1) = tmp;
	}
      }
    }

  }

  void PrintScheduledList() //for debug only
  {
    vector<GRCNode *>::iterator i;

    cerr<<"******Scheduled List********\n";
    for (i = scheduled_list.begin(); i != scheduled_list.end(); i++)
      cerr<<refmap[*i]<<" ";
    cerr<<"\n";
  }

  void PrintOrderSucc(map<GRCNode *, vector<GRCNode *> > ordered) //for debug only
  {
    vector<GRCNode *>::iterator i;
    map<GRCNode *, vector<GRCNode *> >::iterator j;

    cerr<<"********Ordered Successors*******:\n";
    for (j = ordered.begin(); j != ordered.end(); j++){
      cerr<<"N"<<refmap[(*j).first]<<": ";
      for (i = (*j).second.begin(); i != (*j).second.end(); i++)
	cerr<<refmap[*i]<<" ";
      cerr<<"\n";
    }
  }

  void PrintDPSInfo(map<GRCNode *,DPSINFO *> infos) //for debug only
  {
    map<GRCNode *,DPSINFO *>::iterator k;
    map<GRCNode *,set<GRCNode *> >::iterator i;
    set<GRCNode *>::iterator j;
    
    cerr<<"********DPS Information:********\n";
    for (k = infos.begin(); k != infos.end(); k++){
      cerr<<"N"<<refmap[(*k).first]<<"::\n";
      for (i = (*k).second->dpsnodes.begin(); 
	   i != (*k).second->dpsnodes.end(); i++){
	cerr<<refmap[(*i).first]<<": ";
	for (j = (*i).second.begin(); j != (*i).second.end(); j++)
	  cerr<<refmap[*j]<<" ";
      }
      if ((*k).second->selfdps)
	cerr<<refmap[(*k).first];
      cerr<<"\n";
    }
  }

  /***************************************
   Functions for Restructuring PDG
  ****************************************/
  void Restructure()
  {
    vector<GRCNode *>::iterator i;
    set<GRCNode *> D;
    set<GRCNode *>::iterator j;


    for (i = scheduled_list.begin(); i != scheduled_list.end(); i++){
      if(debug) cerr<<"Restructing "<<refmap[*i]<<"\n";
      assert(*i);

      D = DupCheck(*i);
      D.insert(*i);
      for (j = D.begin(); j != D.end(); j++){
	if(debug) cerr<<"Duplicate "<<refmap[*j]<<"...\n";
	Duplicate(*j, D);	
	if(debug) cerr<<"duplicated "<<refmap[*j]<<"...\n";
      }

      for (j = D.begin(); j != D.end(); j++){
	if(debug) cerr<<"Connecting "<<refmap[*j]<<"...\n";
	Connect(*j);
	if(debug) cerr<<"connected "<<refmap[*j]<<"...\n";
      }
      
    }          
  }

  set<GRCNode *> DupCheck(GRCNode *n)
  {
    set<GRCNode *> D;
    set<GRCNode *> black;
    DupCheckDFS(n, D, black);
    return D;
  }
  
  bool DupCheckDFS(GRCNode *n, set<GRCNode *> &D, set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i;

    assert(n);
    if (black.find(n) == black.end()){
      black.insert(n);
    
      for(i = n->predecessors.begin(); i != n->predecessors.end(); i++){
	if (dynamic_cast<Fork *>(*i))
	  if (active[*i] != n)
	    D.insert(n);
	assert(lcopy.find(*i) != lcopy.end());
	if (lcopy[*i] == NULL)
	  D.insert(*i);
	if (DupCheckDFS(*i, D, black))
	  D.insert(n);      
      }
    }
    if (D.find(n) != D.end())
      return true;
    return false;
  }

  void Duplicate(GRCNode *n, set<GRCNode *> D)
  {
    GRCNode *np;
    int i, type;
    set<GRCNode *>::iterator j;
    vector<GRCNode *>::iterator k;

    type = TypeCheck(n);
    if (csted.find(n) == csted.end()){ // a new node copied to the graph
      assert(conn_succ.find(n) == conn_succ.end());  
      csted.insert(n);
      visited.insert(n);
      lcopy[n] = n;
      if (type == 2)//predicate
	for (i = 0; i < (int)n->successors.size(); i++){	  
	  GRCNode *nf = new Fork();	  
	  conn_succ[n].push_back(nf);
	  visited.insert(nf);
	  refmap[nf] = mxnode++;
	}
      return;
    }

    if (type == 1)
      np = new Fork();    
    else{
      if(debug) cerr<<refmap[n]<<"\n";
      assert(type == 2); //predicate

      VariableSymbol *v = GetVariable(n);
      np = NewTester(v);
      assert(np);

      for (i = 0; i < (int)n->successors.size(); i++){
	GRCNode *nf = new Fork();
	conn_succ[np].push_back(nf);
	visited.insert(nf);
	refmap[nf] = mxnode++;
      }

      if (cached.find(n) == cached.end()){ //never cached
	if (lcopy[n])
	  assert(lcopy[n] == n);
	for (i = 0; i < (int)n->successors.size(); i++){
	  GRCNode *si = conn_succ[n][i];
	  assert(dynamic_cast<Fork *>(si));

	  GRCNode *nass = NewAssignment(v, i);
	  cached[n].insert(nass);
	  conn_succ[si].push_back(nass);
	}
      }

      for (j = cached[n].begin(); j != cached[n].end(); j++)
	*np << *j;
    }

    for (k = n->successors.begin(); k != n->successors.end(); k++)
      if (D.find(*k) == D.end())
	lcopy[*k] = NULL;

    refmap[np] = mxnode++;
    visited.insert(np);
    lcopy[n] = np;
    
  }

  void Connect(GRCNode *n)
  {
    vector<GRCNode *>::iterator i;
    int j;
    int type;

    assert(n);
    GRCNode *np = lcopy[n];
    assert(np);

    for (i = n->predecessors.begin(); i != n->predecessors.end(); i++){
      if(debug) cerr<<" for pred "<<refmap[*i]<<"\n";
      assert(lcopy.find(*i) != lcopy.end());
      GRCNode *pp = lcopy[*i];
      assert(pp);
      type = TypeCheck(*i);
      if (type == 1){
	conn_succ[pp].push_back(np);
	active[*i] = n;
      }
      else {
	assert(type == 2);
	for(j = 0; j < (int)(*i)->successors.size(); j++)
	  if ((*i)->successors[j] == n){
	    GRCNode *af = conn_succ[pp][j];
	    assert(af);
	    assert(TypeCheck(af) == 1);
	    conn_succ[af].push_back(np);
	  }
      }
    }
  }

  GRCNode *NewAssignment(VariableSymbol *v, int value)
  {
    char valstr[32];

    assert(v);
    sprintf(valstr, "%d", value);
    TypeSymbol *tp = new BuiltinTypeSymbol("integer");
    Expression *exp = new Literal(valstr, tp);
    Assign *assign = new Assign(v, exp);
    GRCNode *RET = new Action(assign);
    refmap[RET] = mxnode++;
    return RET;
  }

  GRCNode *NewTester(VariableSymbol *v)
  {
    assert(v);
    Expression *lve = new LoadVariableExpression(v);
    GRCNode *tv = new Test(NULL, lve);
    refmap[tv] = mxnode++;
    return tv;    
  }

  VariableSymbol *GetVariable(GRCNode *n)
  {
    VariableSymbol *v;
    assert(dynamic_cast<Switch *>(n) ||
	   dynamic_cast<Test *>(n) ||
	   dynamic_cast<Sync *>(n));
    if (varmap.find(n) == varmap.end()){
      v = NewVariable();
      assert(v);
      symtable->enter(v);
      varmap[n] = v;
    }
    else
      v = varmap[n];

    return v;
  }

  VariableSymbol *NewVariable()
  {
    char new_var_name[32];

    newvar_counter++;

    sprintf(new_var_name, "%s%d", newvar_prefix, newvar_counter);
    TypeSymbol *tp = new BuiltinTypeSymbol("integer");
    VariableSymbol *RET = new VariableSymbol(new_var_name,
					     tp,NULL);
    return RET;
  }

  int TypeCheck(GRCNode *n)
  {
    assert(n);
    if (dynamic_cast<Fork *>(n))
      return 1;//fork
    if (dynamic_cast<Switch *>(n) ||
	dynamic_cast<Test *>(n) ||
	dynamic_cast<Sync *>(n) ||
	dynamic_cast<EnterGRC *>(n))
      return 2;//predicate
    assert(n->successors.size() == 0);
    return 0; //leave
  }
  
  /***************************************
   Functions for Fucing
  ****************************************/

  //for the branch with only 1 cut point under, only use 1 test-var 
  // instead of structure-duplicate var;
  //for consequencial forks & tests guards, merge them
  // --THIS PART IS ONLY DONE ON FORKS CURRENTLY
  void Fucing()
  {
    set<GRCNode *> black;

    ClearJunkFork(top, NULL, black);

    black.clear();
    MergeConsqFork(top, black);

    //MergeSoloVar();

  }

  void ClearJunkFork(GRCNode *n, GRCNode *par,  set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    int k;
     
    assert(n);
    if (black.find(n) != black.end())
      return;
    black.insert(n);
    
    k = 0;
    for (i = conn_succ[n].begin(); i != conn_succ[n].end(); i++){
      ClearJunkFork(*i, n, black);
      if ((dynamic_cast<Fork *>(*i))
	  && (conn_succ[*i].size() == 0)){
	GRCNode *nop = new Nop();
	conn_succ[n][k] = nop;
      }
      k++;
    }
    
    if (dynamic_cast<Fork *>(n)){
      i = conn_succ[n].begin();
      while(i != conn_succ[n].end()){
	bool same_succ = false;
	for(j = conn_succ[n].begin(); j != i; j++){
	  if (*j == *i){
	    same_succ = true;
	    break;
	  }
	}
	if(dynamic_cast<Nop *>(*i) || same_succ)  
	  i=conn_succ[n].erase(i);
	else 
	  i++;
      }
   
      if(conn_succ[n].size() == 1){
	for(j=conn_succ[par].begin();j!=conn_succ[par].end(); j++)
	  if(*j == n) *j = conn_succ[n][0];
	conn_succ[n].clear();
      }
      
    }
  }

  void MergeConsqFork(GRCNode *n, set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i;
    vector<GRCNode *> newchs;
     
    assert(n);
    if (black.find(n) != black.end())
      return;
    black.insert(n);

    for (i = conn_succ[n].begin(); i != conn_succ[n].end(); i++)
	MergeConsqFork(*i, black);
    
    if (dynamic_cast<Fork *>(n)){
      for (i = conn_succ[n].begin(); i != conn_succ[n].end(); i++)
	if((dynamic_cast<Fork *>(*i)) && (ParentNumb(*i) == 1)){
	  newchs.insert(newchs.end(),conn_succ[*i].begin(), conn_succ[*i].end());	
	  conn_succ[*i].clear();
	  visited.erase(*i);
	}
	else 
	  newchs.push_back(*i);
      conn_succ[n] = newchs;
    }
    
  }

  int ParentNumb(GRCNode *n)
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    int RET = 0;

    for (i = visited.begin(); i != visited.end(); i++){
      for (j = conn_succ[*i].begin(); j != conn_succ[*i].end(); j++)
	if ((*j) == n)
	  RET++;
    }
    return RET;
  }

  void TestersDFS(GRCNode *n, set<GRCNode *> &tvset, set<GRCNode *> &black)
  {
    assert(n);
    if(black.find(n) != black.end())
      return;
    black.insert(n);
    
    if (dynamic_cast<Nop *>(n))
      return;
    if (tvmap.find(n) != tvmap.end())
      tvset.insert(n);

    for(vector<GRCNode *>::iterator i = conn_succ[n].begin();
	i != conn_succ[n].end(); i++)
      TestersDFS(*i,tvset,black);
  }

  GRCNode *BranchSearch(GRCNode *n, GRCNode *f)
  {
    set<GRCNode *> black;
    return BranchSearchDFS(n,f,black);
  }

  GRCNode *BranchSearchDFS(GRCNode *n, GRCNode *par, set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i;

    assert(n);
    if (black.find(n)!=black.end()){
      return NULL;
    }
    black.insert(n);

    for (i = n->predecessors.begin(); i != n->predecessors.end(); i++){
      if (*i == par)
	return n;
      else {
	GRCNode *tmp = BranchSearchDFS(*i, par, black);
	if (tmp)
	  return tmp;
      }
    }

    return NULL;
  }


  /***************************************
   Functions for Recoverconn
  ****************************************/
  void RecoverConn()
  {
    set<GRCNode *>::iterator i;
    vector<GRCNode *>::iterator j;
    for(i = visited.begin(); i != visited.end(); i++){
      assert(*i);
      (*i)->successors.clear();
      (*i)->predecessors.clear();
    }
    for(i = visited.begin(); i != visited.end(); i++){
      if(debug) cerr<<"N"<<refmap[*i]<<":";
      if (conn_succ.find(*i)!=conn_succ.end())
	for (j = conn_succ[*i].begin(); j != conn_succ[*i].end(); j++){
	  assert(*j);
	  if(debug) cerr<<refmap[*j]<<" ";
	  (*i)->successors.push_back(*j);	  
	  (*j)->predecessors.push_back(*i);
	}
      if(debug) cerr<<"\n";
    }
  }

  /***************************************
   Functions Else
  ****************************************/
  void ReplaceNull2Nop(GRCNode *n)
  {
    assert(n);

    if (visited.find(n) != visited.end())
      return;
    visited.insert(n);
    
    vector<GRCNode *>::iterator i;
    for (i = n->successors.begin(); i != n->successors.end(); i++)
      if (!*i){
	*i = new Nop();
	(*i)->predecessors.push_back(n);
	refmap[*i] = mxnode++;
	visited.insert(*i);
      }
      else
	ReplaceNull2Nop(*i);
  }
  
  void ReplaceNop2Null(GRCNode *n)
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
	  assert((*i)->predecessors.size() <= 1); //!!!!tcint sth wrong here
	  (*i)->predecessors.clear();
	  *i = NULL;
	}
	else{
	  if(debug) cerr<<refmap[*i]<<" is NOT a nop\n";
	  ReplaceNop2Null(*i);
	}
      }      
      if(debug) cerr<<"finished N"<<refmap[n]<<"\n";
  }

  void CollectDesc()
  {
    set<GRCNode *> black;
    DescentDFS(top,black);
  }

  void DescentDFS(GRCNode *n, set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i;
    
    assert(n);
    if (black.find(n) != black.end())
      return;
    black.insert(n);
    desc[n].insert(n);
    for (i = n->successors.begin();
	 i != n->successors.end(); i++){
      DescentDFS(*i,black);
      assert(desc.find(*i) != desc.end());
      desc[n].insert(desc[*i].begin(), desc[*i].end());
    }
  }

  void ClearUselessDps()
  {
    set<GRCNode  *>::iterator i;
    vector<GRCNode *>::iterator j;

    for(i = visited.begin(); i != visited.end(); i++){
      if (dynamic_cast<Switch *>(*i)){
	j = (*i)->dataSuccessors.begin();
	while( j != (*i)->dataSuccessors.end()){
	  assert(FindinVec((*j)->dataPredecessors, *i));
	  if(desc[*i].find(*j) != desc[*i].end()){
	    RminVec((*j)->dataPredecessors, *i);
	    j = (*i)->dataSuccessors.erase(j);	   
	  }
	  else
	    j++;
	}
      }
    }
  }

  bool FindinVec(vector<GRCNode *> vec, GRCNode *n)
  {
    vector<GRCNode *>::iterator i;
    for (i = vec.begin(); i != vec.end(); i++){
      if ((*i) == n)
	return true;
    }
    return false;
  }

  void RminVec(vector<GRCNode *> &vec, GRCNode *n)
  {
    vector<GRCNode *>::iterator i = vec.begin();
    while (i != vec.end()){
      if ((*i) == n)
	i = vec.erase(i);
      else
	i++;
    }
  }

  void MergeForkFlow()
  {
    set<GRCNode  *>::iterator i,k;
    vector<GRCNode *>::iterator j;
    set<GRCNode *> forks;

    for(i = visited.begin(); i != visited.end(); i++)
      if (dynamic_cast<Fork *>(*i))
	forks.insert(*i);

    for(i = forks.begin(); i != forks.end(); i++){
      assert((*i)->successors.size()>0);

      map<GRCNode *, int> inchnumb; //the number of successors that k included i's
      for(k = forks.begin(); k != forks.end(); k++)
	inchnumb[*k] = 0;

      for (j = (*i)->successors.begin(); j != (*i)->successors.end(); j++)
	for(k = forks.begin(); k != forks.end(); k++){
	  if (i == k)
	    continue;
	  if (FindinVec((*k)->successors, *j))
	    inchnumb[*k]++;
	}

      for(k = forks.begin(); k != forks.end(); k++){
	if (i == k)
	  continue;
	if (inchnumb[*k] == (int)(*i)->successors.size()){
	  for (j = (*i)->successors.begin(); j != (*i)->successors.end(); j++){
	    RminVec((*k)->successors, *j);
	    RminVec((*j)->predecessors, *k);
	  }
	  (*k)->successors.push_back(*i);
	  (*i)->predecessors.push_back(*k);
	}
      }
    }

    set<GRCNode *> black;
    ClearSolochFork(top, black);
      
  }

  void ClearSolochFork(GRCNode *n, set<GRCNode *> &black)
  {
    vector<GRCNode *>::iterator i,j;
     
    assert(n);
    if (black.find(n) != black.end())
      return;
    black.insert(n);

    for (i = n->successors.begin(); i != n->successors.end(); i++)
	ClearSolochFork(*i, black);
    
    if ((dynamic_cast<Fork *>(n)) && (n->successors.size() == 1)){

      for (i = n->predecessors.begin(); i != n->predecessors.end(); i++){
	for(j = (*i)->successors.begin(); j != (*i)->successors.end(); j++)
	  if(*j == n){
	    *j = n->successors[0];
	    RminVec((*j)->predecessors, n);
	    (*j)->predecessors.push_back(*i);
	  }
      }
	
      n->predecessors.clear();
      n->successors.clear();
      visited.erase(n);
    }
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

      AST::SymbolTable *vars = dynamic_cast<AST::SymbolTable*>((*i)->variables);
      assert(vars);

      GRCNode::NumMap refmap;
      STNode::NumMap strefmap;
      
      int mxnode = gf->enumerate(refmap,strefmap);

      PDG2CCFG pdg2ccfg(top, vars, refmap, mxnode);
      mxnode = pdg2ccfg.Execute();
    }
    
    IR::XMLostream o(std::cout);
    o << n;
    
  } catch (IR::Error &e) {
    cerr << e.s << endl;
    exit(-1);
  }

  return 0;

}
