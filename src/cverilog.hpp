#ifndef _CVER_
#define _CVER_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <cassert>

using namespace std;

namespace CVER {

  class Cverilog;

  struct wire {
    string name;
    int monitor;
    int required;

    int input, output;

    string color;

    wire(string name);
    wire(string name, int i);
    wire();
    void dw_dot();
  };

  struct vermod {
    string name;
    int delay;
    virtual ~vermod() {};
    virtual void pr_instance() {}
    virtual void pr_top() {}
    virtual void dw_dot() {}
    virtual void req_wire(vector<wire*> &win, vector<wire*> &wout) {assert(0);}
    string wn(wire *, int def);
    int isconst(wire *w);

    void drawin(wire *w, string edge_label);
    void drawout(wire *w, string edgelabel);
    void drawin(vector<wire *> &w);

    virtual void pr_blif() {}
    virtual void aux_io() {}
    
    void add_top2main(wire *w);
    void add_main2top(wire *w);
  };
  
  struct strap : public vermod {
    wire *out, *in;
    void pr_instance();
    void dw_dot();
    void req_wire(vector<wire*> &win, vector<wire*> &wout);
    strap(wire *out, wire *in);
    strap(wire *probe, string wname, int monitor);
    strap(wire *probe, string wname, int ix, int monitor);

    void pr_blif();
  };

  struct sr_ff : public vermod {
    wire *s, *r, *q, *rst, *clk;
    int init;
    static void pr_def();
    void pr_instance();
    void dw_dot();
    void req_wire(vector<wire*> &win, vector<wire*> &wout);
    sr_ff(wire *q, wire *s, wire *r);
  };
  
  struct d_ff : public vermod {
    wire *d, *q, *rst, *clk;
    int init;
    static void pr_def();
    static void pr_def_xilinx();
    void pr_instance();
    void dw_dot();
    d_ff(wire *q, wire *d);
    d_ff(wire *q, wire *d, string pref, int ix);
    void req_wire(vector<wire*> &win, vector<wire*> &wout);

    void pr_blif();
  };

  struct andgate: public vermod {
    wire *out;
    vector<wire*> win;
    void pr_instance();
    void dw_dot();
    andgate();
    andgate(wire *out, wire *in1, wire *in2);
    void req_wire(vector<wire*> &win, vector<wire*> &wout);

    void pr_blif();
  };

  struct orgate: public vermod {
    wire *out;
    vector<wire*> win;
    void pr_instance();
    void dw_dot();
    orgate();
    orgate(wire *out, wire *in1, wire *in2);
    void req_wire(vector<wire*> &win, vector<wire*> &wout);

    void pr_blif();
  };

  struct notgate: public vermod {
    wire *out, *in;
    void pr_instance();
    void dw_dot();
    notgate(wire *out, wire *in);
    void req_wire(vector<wire*> &win, vector<wire*> &wout);

    void pr_blif();
  };

  struct counter: public vermod {  
    wire *alarm, *start, *cenable;
    int size, startval;
    void pr_instance();
    static void pr_def();
    void dw_dot();
    counter(wire *alarm, wire *start, wire *cenable, int startval);
    void req_wire(vector<wire*> &win, vector<wire*> &wout);

    void pr_blif();
    void pr_top();
    void aux_io();
  };

  struct lut: public vermod {
    wire *wout;
    vector<wire*> win;
    vector<vector<char> > btable;
    int outplane;

    void pr_instance();
    void dw_dot();
    void req_wire(vector<wire*> &win, vector<wire*> &wout);
    void pr_blif();
    
    lut(wire *wout, vector<wire *> &win, vector<vector<char> > &btable, int outplane);

  };

  extern Cverilog *current;
  extern wire *w_0, *w_1;
  extern int monitor_level;

  struct Cverilog{
    ostream &ov, &ob, &odot;
    int labwire, labcirc;
    string w_prefix;

    vector<wire*> wires;
    vector<vermod*> modules;
    wire *clk, *rst;

  public:
    set<wire*> auxin, auxout;

    Cverilog(ostream &ov, ostream &ob, ostream &odot);

    int delay;
    int vcounters;

    string new_wname(){
      stringstream s;
      s<<w_prefix<<labwire++;
      return s.str();
    }

    string new_uname(){
      stringstream s;
      s<<"u_"<<labcirc++;
      return s.str();
    }

    wire* build_orgate(vector<wire*> &win);
    wire* build_andgate(vector<wire*> &win);
    wire* build_orgate(wire *w1, wire *w2);
    wire* build_andgate(wire *w1, wire *w2);
    wire* build_notgate(wire *w);

    void reorder_wires();
    void build(); // builds the body to feed iverilog & vvp and also ISE Webpack
    void build_blif(string modname);
    void par_rename(string s);
    void build_interface(string modname, bool v5, bool xilinx, bool tbench, bool debugsignals);
    int testvectors(int debugsignals);

    void remove_unused();
    string wn(wire *);
    void drawdot();
  };
  
}

#endif
