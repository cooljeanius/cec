#line 1306 "GRCOpt.nw"
#include "IR.hpp"
#include "AST.hpp"
#include <set>
#include <vector>

namespace GRCOpt {

  using namespace AST;

  using std::set;
  using std::vector;

  
#line 37 "GRCOpt.nw"
template <class T> typename vector<T*>::iterator find(vector<T*> &v, T *e)
{
  for (typename vector<T*>::iterator i = v.begin() ; i != v.end() ; i++)
    if ( (*i) == e ) return i;
  return v.end();
}
#line 50 "GRCOpt.nw"
template <class T> void erase(vector<T*> &v, T *e) {
  v.erase(find(v, e));
}
#line 60 "GRCOpt.nw"
template <class T> bool contains(set<T> &s, T o) {
  return s.find(o) != s.end();
}
#line 66 "GRCOpt.nw"
template <class T, class U> bool contains(map<T, U> &m, T o) {
  return m.find(o) != m.end();
}
#line 76 "GRCOpt.nw"
void delete_node(GRCNode *);
#line 104 "GRCOpt.nw"
void delete_node(STNode *);
#line 133 "GRCOpt.nw"
void bypass(GRCNode *);
#line 166 "GRCOpt.nw"
void bypass(STNode *);
#line 1319 "GRCOpt.nw"
  
#line 211 "GRCOpt.nw"
class Simulator : public Visitor {
  GRCgraph &g;

  // Reached nodes in the control-flow graph and selection tree
  std::set<GRCNode *> cfg;
  std::set<STNode *> st;

  // Switch statement in the CFG for each exclusive node in the ST
  std::map<STexcl *, Switch *> switch_for_excl;

  // The set of known-reachable children of each sync node
  std::map<Sync *, set<int> > sync_levels;

  // Nodes scheduled to be analyzed
  std::set<GRCNode *> pending;

  GRCNode *entergrc;

  // All cfg and ST nodes
  std::set<GRCNode *> allcfg;
  std::set<STNode *> allst;

public:
  
#line 244 "GRCOpt.nw"
Simulator(GRCgraph &);
#line 454 "GRCOpt.nw"
Status visit(Switch &);
#line 491 "GRCOpt.nw"
Status visit(Enter &);
#line 511 "GRCOpt.nw"
Status visit(Terminate &);
#line 601 "GRCOpt.nw"
Status visit(Sync &);
#line 621 "GRCOpt.nw"
Status visit(Fork &);
#line 637 "GRCOpt.nw"
Status visit(Test &);
#line 655 "GRCOpt.nw"
Status visit(DefineSignal &);
#line 670 "GRCOpt.nw"
Status visit(Action &);
#line 685 "GRCOpt.nw"
Status visit(Nop &);
#line 700 "GRCOpt.nw"
Status visit(STSuspend &);
#line 708 "GRCOpt.nw"
Status visit(ExitGRC &) { return Status(); }
#line 735 "GRCOpt.nw"
void all_dfs(GRCNode *);
#line 755 "GRCOpt.nw"
void st_walk(STNode *);
#line 235 "GRCOpt.nw"
  virtual ~Simulator() {}
};
#line 1320 "GRCOpt.nw"
  
#line 765 "GRCOpt.nw"
class Pass : public Visitor {
  Status visit(Switch &) { return Status(); }
  Status visit(Test &) { return Status(); }
  Status visit(Terminate &) { return Status(); }
  Status visit(Sync &) { return Status(); }
  Status visit(Fork &) { return Status(); }
  Status visit(Action &) { return Status(); }
  Status visit(Enter &) { return Status(); }
  Status visit(STSuspend &) { return Status(); }
  Status visit(DefineSignal &) { return Status(); }
  Status visit(Nop &) { return Status(); }
  
  std::vector<GRCNode*> topolist;
  std::set<GRCNode*> reachable_nodes;
  std::set<GRCNode*> all_nodes;
  bool forward;
  GRCNode *entergrc;
  GRCNode *exitgrc;
  GRCNode *grcroot;
  STNode *stroot;

  
#line 812 "GRCOpt.nw"
void forward_dfs(GRCNode *);
#line 834 "GRCOpt.nw"
void all_dfs(GRCNode *);
#line 787 "GRCOpt.nw"
  
public:
  Pass(GRCgraph* g, bool f) : forward(f) {
    assert(g);
    stroot   = g->selection_tree;
    assert(stroot);
    entergrc = g->control_flow_graph;
    assert(entergrc);
    assert(entergrc->successors.size() == 2);
    exitgrc  = entergrc->successors[0];
    grcroot  = entergrc->successors[1];
  }
  virtual ~Pass(){}
  
  void transform();
};
#line 1321 "GRCOpt.nw"
  
#line 1093 "GRCOpt.nw"
class DanglingST : public Pass {
    std::set<STNode*> &stkept;
    Status visit(Enter &);
    Status visit(STSuspend &);
  public:
    DanglingST(GRCgraph* g, std::set<STNode*> &stkept) :
	 Pass(g, true), stkept(stkept) {}
};
#line 1322 "GRCOpt.nw"
  
#line 983 "GRCOpt.nw"
class PruneSW : public Pass {
    std::set<STNode*> &stkept;
    Status visit(Switch &);
  public:
    PruneSW(GRCgraph* g, std::set<STNode*> &stkept) :
	 Pass(g, false), stkept(stkept) {}
};
#line 1323 "GRCOpt.nw"
  
#line 1034 "GRCOpt.nw"
class MergeSW : public Pass {
  std::set<STNode*> &stkept;
  Status visit(Switch &);
public:
  MergeSW(GRCgraph* g, std::set<STNode*> &stkept) :
    Pass(g, false), stkept(stkept) {}
};
#line 1324 "GRCOpt.nw"
  
#line 902 "GRCOpt.nw"
class STSimplify {
  GRCgraph *g;
  std::set<STNode*> &stkept;

  STNode *check_st(STNode *, STNode *realpar);
public:
  STSimplify(GRCgraph *g, std::set<STNode*> &stkept) 
    : g(g), stkept(stkept) { assert(g); }
  void simplify() { g->selection_tree = check_st(g->selection_tree, NULL); }
};
#line 1325 "GRCOpt.nw"
  
#line 963 "GRCOpt.nw"
class RemoveNops : public Pass {
    Status visit(Nop &);
  public:
    RemoveNops(GRCgraph *g) : Pass(g, true) {};
};
#line 1326 "GRCOpt.nw"
  
#line 1130 "GRCOpt.nw"
class RedundantEnters : public Pass {
  Status visit(Enter &);
public:
  RedundantEnters(GRCgraph *g) : Pass(g, false) {}
};
#line 1327 "GRCOpt.nw"
  
#line 1157 "GRCOpt.nw"
class UnobservedEmits : public Pass {
  set<SignalSymbol*> observed;
  Status visit(Action &);
  Status visit(DefineSignal &);
public:
  UnobservedEmits(Module *, GRCgraph *);
};
#line 1328 "GRCOpt.nw"
}
