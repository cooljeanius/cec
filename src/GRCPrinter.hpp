#line 357 "GRCPrinter.nw"
#ifndef _GRC_PRINTER_HPP
#  define _GRC_PRINTER_HPP
#  include "AST.hpp"
#  include "EsterelPrinter.hpp"
#  include <iostream>
#  include <map>
#  include <set>

namespace GRCDot {
  using namespace AST;
  using std::map;
  using std::set;

  typedef map<GRCNode *, int> CFGmap;
  typedef map<STNode *, int> STmap;
  void GRCDot(std::ostream &, GRCgraph *, Module *, bool, bool);
  int GRCDot(std::ostream &o, GRCgraph *g, Module *m, bool drawstlink,
	      bool clean,  CFGmap &cfgmap, STmap &stmap, int mxnode);

  class GRCDP : public Visitor {
    std::ostream &o;
    CFGmap &cfgnum; // Node numbers for control-flow graph
    STmap &stnum; // Node numbers for selection tree
    set<GRCNode *> reached; // Used during DFS of CFG

    int nextnum;
    int mynum;

    EsterelPrinter ep;
  public:
    GRCDP(std::ostream &oo, CFGmap &cm, STmap &sm, int nextnum) :
      o(oo), cfgnum(cm), stnum(sm), nextnum(nextnum), ep(oo, false),
      drawstlink(false), clean(false) {}

    virtual ~GRCDP() {}

    // Output style flags

    bool drawstlink;
    bool clean;

    
#line 32 "GRCPrinter.nw"
void visit_cfg(GRCNode *);
#line 114 "GRCPrinter.nw"
Status visit(Switch &);
Status visit(Test &);
Status visit(Terminate &);
Status visit(Sync &);
Status visit(Fork &);
Status visit(Action &);
Status visit(Enter &);
Status visit(STSuspend &);
Status visit(EnterGRC &);
Status visit(ExitGRC &);
Status visit(Nop &);
Status visit(DefineSignal &);
#line 271 "GRCPrinter.nw"
void visit_st(STNode *);
#line 303 "GRCPrinter.nw"
Status visit(STexcl &);
Status visit(STref &);
Status visit(STpar &);
Status visit(STleaf &);

#line 400 "GRCPrinter.nw"
    void drawSTlink(GRCNode *, STNode *);
  };
};
#endif
