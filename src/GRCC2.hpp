#line 884 "GRCC2.nw"
#ifndef _GRCC2_HPP
#  define _GRCC2_HPP

#  include "AST.hpp"
#  include "CPrinter.hpp"

namespace GRCC2 {
  using namespace AST;
  
#line 20 "GRCC2.nw"
typedef map<GRCNode *, int> CFGmap;
typedef map<STNode *, int> STmap;
#line 27 "GRCC2.nw"
void generateC(std::ostream &o, Module &, string, bool);
#line 302 "GRCC2.nw"
void fixGRCNode(GRCNode *);
#line 364 "GRCC2.nw"
void checkAcyclic(GRCNode *);
#line 374 "GRCC2.nw"
class AcyclicChecker {
  std::map<GRCNode*, bool> completed;
  bool visit(GRCNode *);
public:
  AcyclicChecker(GRCNode* n) {
    if (visit(n)) {
      std::cerr << std::endl;
      throw IR::Error("CFG is cyclic");
    }
  }
};
#line 428 "GRCC2.nw"
void calculateSchedule(GRCNode *, vector<GRCNode*> &);
#line 438 "GRCC2.nw"
class Scheduler {
  std::set<GRCNode*> visited;
  std::vector<GRCNode*> &schedule;
  void visit(GRCNode*);
public:
  Scheduler(GRCNode *n, vector<GRCNode*> &schedule) : schedule(schedule) {
    visit(n);
  }
};
#line 474 "GRCC2.nw"
struct Cluster {
  unsigned int id;
  unsigned int level;
  vector<GRCNode*> nodes;
  std::set<Cluster*> successors;
  vector<GRCNode*> entries;
  Cluster(int id) : id(id) {}
};
#line 485 "GRCC2.nw"
void cluster(vector<GRCNode*> &, vector<Cluster*> &, map<GRCNode*, Cluster*> &);
#line 540 "GRCC2.nw"
void greedyCluster(GRCNode *, vector<Cluster*> &, map<GRCNode*, Cluster*> &);
#line 656 "GRCC2.nw"
struct Level {
  vector<Cluster *> clusters;
};
#line 662 "GRCC2.nw"
void levelize(vector<Cluster*> &, vector<Level*> &);
#line 740 "GRCC2.nw"
void split(Cluster *, map<GRCNode*, Cluster*> &, map<GRCNode*, string> &);
#line 893 "GRCC2.nw"
}

#endif
