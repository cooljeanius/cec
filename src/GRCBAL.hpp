#ifndef _GRC_BAL_HPP
#  define _GRC_BAL_HPP
#  include "AST.hpp"
#  include "EsterelPrinter.hpp"
#  include <iostream>
#  include <map>
#  include <set>
#  include <list>

namespace GRCBAL {
  using namespace AST;
  using std::map;
  using std::set;
  using std::vector;

  class SGRNode;  
  class DSNode;
  class SwitchThreadNode;
  class ActivePoint;
  class TWBNode;
  class MWBNode;
  class NOPNode;
  class ActionNode;
  class JoinNode;
  class ForkNode;
  class ExitGRCNode;
  class EnterGRCNode;
  class EnterNode;
  class TermNode;
  class InitThreadsNode;
  class SuspendNode;

  // label nodes
  class JmpLabel;
  class JmpStmt;
  class CaseLabel;
  class DoneLabel;

  typedef map<GRCNode *, int> CFGmap;
  typedef map<STNode*, int> STmap;
  typedef map<GRCNode *, SGRNode *> SGmap;
  typedef map<SGRNode *, int> SGRmap;
  typedef map<int, SGRNode *> Nummap;
  typedef vector<SGRNode *> nodeVector;
  typedef std::list<std::list<int> > list_of_lists;

  void drawDot(std::ostream &, nodeVector, nodeVector,
	       list_of_lists &,Module *);
  void convertGRC(std::ostream &, GRCgraph *, Module *, bool, bool);
  void calculateSchedule(GRCNode *, vector<GRCNode*> &);

  /* ************** BAL Graphical DOT Printer ************************** */
  class BALDP {
    std::ostream &o;
    set<SGRNode *> reached; // used during DFS of SGR

    void drawComponent(SGRNode *);
  public:
    BALDP(std::ostream &oo): o(oo) {}
    virtual ~BALDP() {}

    void drawGraph(nodeVector g);
  };

  /* ******************* Convert GRC to BAL *************************** */

  class GVisitor : public Visitor {
  public:
    virtual Status visit(DSNode &) = 0;
    virtual Status visit(SwitchThreadNode &) = 0;
    virtual Status visit(ActivePoint &) = 0;
    virtual Status visit(TWBNode &) = 0;
    virtual Status visit(MWBNode &) = 0;
    virtual Status visit(NOPNode &) = 0;
    virtual Status visit(ActionNode &) = 0;
    virtual Status visit(JoinNode &) = 0;
    virtual Status visit(ForkNode &) = 0;
    virtual Status visit(ExitGRCNode &) = 0;
    virtual Status visit(EnterGRCNode &) = 0;
    virtual Status visit(EnterNode &) = 0;
    virtual Status visit(TermNode &) = 0;
    virtual Status visit(InitThreadsNode &) = 0;

    virtual Status visit(SuspendNode &) = 0;

  };

  // class to convert from GRC to BAL
  class GRC2BAL : public GVisitor {
    std::ostream &o;
    CFGmap tnum;               // Node to thread numbers
    vector<GRCNode *> schedule;// the schedule of the nodes

    SGRmap nodeNumber;
    Nummap sched;   

    int next_label;
    map<SGRNode *, int> node_label;    

    map<SignalSymbol *, string> signal_regs;
    map<STexcl *, string> state_regs;
    map<Sync *, string> term_regs;
    map<Counter*, string> counters; 
    map<VariableSymbol*, string> var;
    map<SignalSymbol*, string> valued_signals;

    int initial_state;

    set<GRCNode *> assignedThread;
    set<SGRNode *> visited; // keep track of already visited nodes

    SGRNode * currentNode;

    int num_threads;
    int myth;
    int lastthread;
    int nextnum;

    int testcount;
    int jmplabel;

    int sig_num;
    int state_num;
    int join_num;
    int var_num;

    void joinThreadNum(GRCNode *);
    void forkThreadNum(GRCNode *);
    void genThreadNum(GRCNode *);

    void testForks();

    void printDFS(SGRNode *);
    void assignLabels(SGRNode *);
    void enumerateJoins(SGRNode *);

  public:
    GRC2BAL(std::ostream &oo): o(oo) { 
      tnum.clear();
      schedule.clear();
      threads.clear();
      nodeNumber.clear();

      signal_regs.clear();
      state_regs.clear();
      term_regs.clear();
      counters.clear();
      var.clear();
      valued_signals.clear();

      initial_state = -1; // Should be changed by removeConcurrency
      
      visited.clear();
      currentNode = NULL;

      num_threads = 0;
      myth = 0;
      lastthread = 0;
      nextnum = 0;
      testcount = 0;
      jmplabel = 0;

      sig_num = 0;
      state_num = 0;
      join_num = 0;
      var_num = 0;
    }
    virtual ~GRC2BAL() {}

    nodeVector threads; // the root of each thread.
    nodeVector dead_threads; // The root of each non-running thread

    list_of_lists context_switch_points;

    void assignThreads(GRCNode *);
    void assignSchedule(GRCNode *);
     
    void removeConcurrency(bool, CFGmap); // remove the concurrency of the graph

    int findSchedPos(GRCNode *);

    void printBAL(Module*, STmap stmap, STexcl *root, int initial_state);

    void indent(int n){
      for (int i = 0 ; i < n; i++) o << " ";
    }

    Status visit(Switch &);
    Status visit(Test &);
    Status visit(Terminate &);
    Status visit(Sync &);
    Status visit(Fork &);
    Status visit(Action &);
    Status visit(Enter &);
    Status visit(EnterGRC &);
    Status visit(ExitGRC &);
    Status visit(Nop &);
    Status visit(DefineSignal &);
    Status visit(STSuspend &);

    /* Used to Print BAL for each SGRNode */
    Status visit(DSNode &);
    Status visit(SwitchThreadNode &);
    Status visit(ActivePoint &);
    Status visit(TWBNode &);
    Status visit(MWBNode &);
    Status visit(NOPNode &);
    Status visit(ActionNode &);
    Status visit(JoinNode &);
    Status visit(ForkNode &);
    Status visit(ExitGRCNode &);
    Status visit(EnterGRCNode &);
    Status visit(EnterNode &);
    Status visit(TermNode &);
    Status visit(InitThreadsNode &);
        
    Status visit(SuspendNode &);

    void generateBAL(StartCounter *);
    void generateBAL(Expression *);
    void generateBAL(Assign *);
    void generateBAL(Literal *);
    void generateBAL(LoadSignalExpression *);
    void generateBAL(LoadSignalValueExpression *);
    void generateBAL(LoadVariableExpression *);
    void generateBAL(CheckCounter  *);
    void generateBAL(BinaryOp  *);
    void generateBAL(UnaryOp *);
  };

  /* **************** BAL Sequential Representation *********************** */

  // sequential representation
  class SGRNode {
    static std::list<SGRNode *> allNodes;

  public:
    SGRNode() {
      endnode = false;
      branch_visited = false;
      allNodes.push_back(this);
    }

    virtual ~SGRNode() {}
    
    static void removeNOPs();

    GRCNode * GRCme;
    bool endnode;
    bool branch_visited;
    int tnum;
    int nnum;
    vector<SGRNode *> parents;
    vector<SGRNode *> children;
    vector<int> visited;

    virtual void printDot(std::ostream &o, int n) = 0;
    virtual Status welcome(GVisitor&) = 0;

    bool isVisited(int cnum){
      for(unsigned int i = 0; i < visited.size(); i++) {
	if (visited[i] == cnum) return true;
      }
      return false;
    }
    void add(int cnum){
      if (!isVisited(cnum))
	visited.push_back(cnum);
    }
  };

  // define signal node
  class DSNode : public SGRNode{
  public:
    SignalSymbol * signal;
    bool is_surface;
    
    DSNode(SignalSymbol *s, bool ss): signal(s), is_surface(ss) {}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  // x node
  class SwitchThreadNode : public SGRNode{
  public:
    //  ActivePoint * myActive;
    int switchThread;
  
    SwitchThreadNode(int t): switchThread(t){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  // . node
  class ActivePoint : public SGRNode {
  public:
    ActivePoint(){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  // two way branch : Branch on state of a signal
  class TWBNode : public SGRNode{
  public:
    Expression * predicate;

    TWBNode(Expression * p): predicate(p){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  // multiway branch: branch on state of a state
  class MWBNode : public SGRNode{
  public:
    MWBNode(){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  // NOP node
  class NOPNode : public SGRNode{
  public:
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
    NOPNode(){}
  };

  // emit or assignment
  class ActionNode : public SGRNode{
  public:
    Statement * body;

    ActionNode(Statement * s): body(s){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  class JoinNode : public SGRNode{
  public:
    JoinNode() { }
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  class ForkNode : public SGRNode{
  public:
    vector<int> startthreads;
    ForkNode(){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
    void add(int num){
      bool found = false;
      for(vector<int>::iterator i = startthreads.begin();
	  i != startthreads.end() && !found; i++)
	if(*i == num) found = true;
      
      if(!found)
	startthreads.push_back(num);
    }
  };
  class ExitGRCNode : public SGRNode{
  public:
    ExitGRCNode(){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  }; 
 class EnterGRCNode : public SGRNode{
 public:
    EnterGRCNode(){}
    void printDot(std::ostream &o, int n);
   Status welcome(GVisitor&);
  };
  class EnterNode : public SGRNode{
  public:
    EnterNode(){}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };
  class TermNode : public SGRNode{
  public:
    int code;
    Sync * sync;
    TermNode(int c, Sync * s): code(c), sync(s) {}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };
  class InitThreadsNode : public SGRNode {
  public:
    vector<int> startthreads;
    InitThreadsNode() {}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
    void add(int num){
      bool found = false;
      for(vector<int>::iterator i = startthreads.begin();
	  i != startthreads.end() && !found; i++)
	if(*i == num) found = true;
      
      if(!found)
	startthreads.push_back(num);
    }
  };

  class SuspendNode : public SGRNode {
  public: 
    SuspendNode() {}
    void printDot(std::ostream &o, int n);
    Status welcome(GVisitor&);
  };

  class Scheduler {
    std::set<GRCNode*> visited;
    std::vector<GRCNode*> &schedule;
    void visit(GRCNode*);
  public:
    Scheduler(GRCNode *n, vector<GRCNode*> &schedule) : schedule(schedule) {
      visit(n);
    }
  };
};
#endif
