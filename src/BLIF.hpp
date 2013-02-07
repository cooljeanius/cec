#line 813 "BLIF.nw"
#ifndef _BLIF_HPP
#  define _BLIF_HPP

#  include <string>
#  include <vector>
#  include <assert.h>
#  include <iostream>
#  include <map>
#  include <set>

namespace BLIF {
  using std::vector;
  using std::string;
  using std::set;

  class Input;
  class Gate;
  class Netlist;

  
#line 81 "BLIF.nw"
class Input {
  friend class Gate;
  Input(Gate *d, Gate *g, bool i = false)
    : driver(d), gate(g), is_inverting(i) {}
public:
  Gate *driver;
  Gate *gate;
  bool is_inverting;
};
#line 833 "BLIF.nw"
  
#line 42 "BLIF.nw"
class Gate {
 friend class Netlist;

 Gate(Netlist *nl, unsigned int i, string n, bool inv = false)
   : parent(nl), id(i), name(n), is_inverting(inv),
     is_input(false), is_output(false), is_latch(false) {}
public:
  Netlist *parent;
  unsigned int id;
  string name;
  vector<Input*> inputs;
  bool is_inverting;
  vector<Input*> outputs;

  bool is_input;
  bool is_output;
  bool is_latch;

  void newInput(Gate *g, bool i = false) {
    assert(g);
    Input *ni = new Input(g, this, i);
    inputs.push_back(ni);
    g->outputs.push_back(ni);
  }

  void newOutput(Gate *g, bool i = false) {
    assert(g);
    g->newInput(this, i);
  }
};
#line 834 "BLIF.nw"
  
#line 98 "BLIF.nw"
  class Netlist {
  public:
    string name;
    Netlist(string n) : name(n) {}
    vector<Gate*> gates;
    Gate *newGate(bool = false, string = "");
  };

#line 836 "BLIF.nw"
  
#line 507 "BLIF.nw"
void print_dot(std::ostream &, const Netlist &);

#line 838 "BLIF.nw"
  
#line 127 "BLIF.nw"
Netlist *read_blif(std::istream &);
#line 839 "BLIF.nw"
  
#line 145 "BLIF.nw"
class BlifReader {
  Netlist *netlist;
  std::istream &inf;
  std::map<string, Gate*> namedGate;
  string line;
  vector<string> word;
  unsigned lineNumber;

  struct Row {
    string andplane;
    char orplane;
    Row(string s, char c) : andplane(s), orplane(c) {}
  };

public:
  BlifReader(std::istream &ii) :  netlist(0), inf(ii), lineNumber(0) {}

  struct Error {
    unsigned lineNumber;
    string error;
    Error(unsigned l, string s) : lineNumber(l), error(s) {}
  };

  
#line 177 "BLIF.nw"
Gate *getGate(string = "");
#line 196 "BLIF.nw"
Netlist *readModel();
#line 381 "BLIF.nw"
void readLine();
#line 169 "BLIF.nw"
};

#line 841 "BLIF.nw"
  
#line 455 "BLIF.nw"
void print_blif(std::ostream &, const Netlist &);
#line 842 "BLIF.nw"
  
#line 655 "BLIF.nw"
class Simulator {
  Netlist &n;

  unsigned int next;
  Gate **topoorder; // Array of gate pointers in topological order

  bool *currentState; // State of latches, indexed by gate number
  bool *nextState; // State of inputs and gates/latches after simulation

  void dfs(Gate*, set<Gate*> &);
public:
  vector<Gate*> inputs;
  vector<Gate*> outputs;
  vector<Gate*> latches;

  void setInput(Gate* g, bool v) { currentState[g->id] = v; }
  bool getOutput(Gate* g) { return currentState[g->id]; }
  bool getLatch(Gate* g) { return nextState[g->id]; }

  Simulator(Netlist &);
  ~Simulator();

  void reset();
  void simulate();

  static bool debug;
};

#line 844 "BLIF.nw"
  
#line 565 "BLIF.nw"
void print_verilog(std::ostream &, const Netlist &);
#line 845 "BLIF.nw"
}

#endif
