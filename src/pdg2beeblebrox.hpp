#include <iostream>
#include <fstream>

#include "IR.hpp"
#include "AST.hpp"
#include "GRCPrinter.hpp"
#include "cverilog.hpp"

using namespace AST;
using namespace GRCDot;
using namespace CVER;

struct beeble_options {
  bool gensm; ofstream smfile;
  bool gendot; ofstream dotfile;
  bool genver, genxilinx, gentb; ofstream vfile;
  bool ff;
  beeble_options(): gensm(false), gendot(false),
		    genver(false), genxilinx(false), gentb(false), ff(false) {}
		    
};

// state machine
struct pdg2beeblebrox;

struct state_machine {

  int no_states;
  int boot;
  pdg2beeblebrox *owner;
  STNode *st;

  vector<vector<wire*> > enters;
  vector<wire*> checks;
  wire *suspend;

  state_machine(pdg2beeblebrox *owner, STNode *st);
  void setBoot(int no) { boot = no; }
  void addEnter(int no, wire *flow);
  wire *chkState(int no);
  void setSuspend(wire *susp);

  void onehot();
  void lastzero();
  void fixencoding(int no_ff, char **enc);

  void build_circuit();
  void build_circuit_ff();

};

// zaphod signal

struct zaphod_signal {
  wire *signal;
  vector<wire*> sigemit;
  // string realname;
};

struct beeble_counter {
  wire *signal, *alarm;
  vector<wire*> starts, enables;
  int startval;
  beeble_counter();
  void build();
};


// beeble_context

struct beeble_context {
  pdg2beeblebrox *owner;   
  Module *mod;
  BuiltinTypeSymbol *int_type, *bool_type;

  TypeSymbol *t;
  int isconst;
  
  // bottom -> top
  wire *wexpr;
  int intexpr;
  
  // top -> bottom
  wire *flow;
  
  beeble_context(pdg2beeblebrox *beeb, Module *m): owner(beeb), mod(m) {
    int_type = dynamic_cast<BuiltinTypeSymbol*>(mod->types->get("integer"));
    bool_type = dynamic_cast<BuiltinTypeSymbol*>(mod->types->get("boolean"));
    assert( int_type && bool_type);
  }
  
  // these are "macros" for simple expression which accept only one type
   
  void setbool(wire *w){
    t = bool_type;
    wexpr = w;
  }
  
  wire* getbool(Expression *e){
      eval_expr(e);
      assert( t == bool_type );
      return wexpr;
  }
  
  void setint(int n, int cnst){
    t = int_type;
    intexpr = n;
    isconst = cnst;
  }
  
  int getint(Expression *e){
    eval_expr(e);
    assert( t == int_type );
    return intexpr;
  }
  
  int getintconst(Expression *e){
    eval_expr(e);
    assert(t == int_type);
    assert(isconst);
    return intexpr;
  }
  
  wire *getflow() {return flow;}

  void do_statement(Statement *s, wire *flowin);
  void eval_expr(Expression *e);
  
};



// pdg2beeblebrox - the main class

struct  pdg2beeblebrox : public Visitor {

  Module *mod;

  beeble_options &opt;
  Cverilog cv;
  beeble_context bc;

  CFGmap &cfgmap;
  STmap &stmap;

  STNode *stroot, *boot;
  GRCNode *top_flow, *entergrc;
  GRCgraph *top;
  SymbolTable *sigs;

  int debug;

  set<GRCNode*> dfs_black;
  vector<GRCNode*> topo;
  map<GRCNode*, vector<wire*>* > v_flow;
  map<GRCNode*, wire *> or_flow;

  map<STNode*, state_machine*> smachines;

  map<SignalSymbol*, zaphod_signal*> signals;
  map<Counter*, beeble_counter*> counters;
  map<STNode*, vector<wire*> > suspends;

  pdg2beeblebrox(Module *mod, ostream &bliffile, beeble_options &opt, CFGmap &cfgmap, STmap &stmap):
    mod(mod),  opt(opt), cv(opt.vfile, bliffile, opt.dotfile), bc(this, mod), cfgmap(cfgmap), stmap(stmap)
  {
    top = dynamic_cast<GRCgraph*>(mod->body); assert(top);
    stroot = dynamic_cast<STNode*>(top->selection_tree); assert(stroot);
    boot = dynamic_cast<STleaf*>(stroot->children.back()); assert(boot);
    entergrc = dynamic_cast<EnterGRC*>(top->control_flow_graph); assert(entergrc);
    top_flow = entergrc->successors[0]; assert(top_flow);
    sigs = mod -> signals; assert(sigs);

  }

  virtual ~pdg2beeblebrox() {}

  void dfs(GRCNode *);
  void sm_init(STNode *);
  void sm_build(STNode *, wire *susp);

  void init_signals();
  void close_signals();
  void init_counters();
  void close_counters();

  void execute();


  wire *flow_or_parents(GRCNode *n);
  void flow_all_children(GRCNode *n, wire *flow);
  void flow_add_arc(GRCNode *p, GRCNode *c, wire *flow);

  Status visit(Switch &);
  Status visit(Test &);
  Status visit(Terminate &);
  Status visit(Sync &);
  Status visit(Fork &);
  Status visit(Action &);

  Status visit(STSuspend &);
  Status suspend_sm(STSuspend &);

  Status visit(Enter &);
  Status enter_sm(Enter &);

  Status visit(LoadSignalExpression &);
  Status visit(BinaryOp &);
  Status visit(UnaryOp &);
  Status visit(Emit &);
  Status visit(Exit &);

  Status visit(Literal &);
  Status visit(VariableSymbol &s);
  Status visit(LoadVariableExpression &);

  // Currently unsupported
  Status visit(Assign&) { return Status(); }
  
  Status visit(StartCounter &);
  Status visit(CheckCounter &);

  Status visit(DefineSignal &s){flow_or_parents(&s); return Status();}
  
};
