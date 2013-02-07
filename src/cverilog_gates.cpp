#include "cverilog.hpp"
#include <set>

namespace CVER {

 // SR FLIP FLOP - S has priority over R -- can be not updated !!!

  sr_ff::sr_ff(wire *q, wire *s, wire *r):s(s),r(r),q(q)
  {
    assert(q && (s||r));
    rst=current->rst;
    clk=current->clk;
    init=0;
    name=current->new_uname(); current->modules.push_back(this);
  }

  void sr_ff::pr_def()
  {
    ostream &o=current->ov;

    o<<"module sr_ff(rst, clk, q, s, r, init);\n";
    o<<"input rst, clk, s, r, init;\n";
    o<<"output q;\n";
    o<<"reg q;\n";

    o<<"always @(posedge clk or posedge rst) begin\n";
    o<<"  if(rst==1) q <= init; else\n";
    o<<"    if(s==1) q <= 1; else if(r==1) q<=0;\n";
    o<<"end\n";

    o<<"endmodule\n\n";
  }

  // s or r can be missing
  void sr_ff::pr_instance()
  {
    ostream &o=current->ov;
    assert(rst&&clk&&q&&  (s||r));
    o<<"sr_ff "<<name<<"("<<rst->name<<","<<clk->name<<","
     <<q->name<<","<<wn(s,0)<<","<<wn(r,0)<<","<<init<<");\n";
  }

  void sr_ff::dw_dot()
  {
    ostream &o=current->odot;
    assert(rst&&clk&&q&& (s||r));
    o<<name<<"[label=\"SRFF\" shape=box ]\n";
    drawin(s,"s");
    drawin(r,"r");
    drawout(q,"q");
  }

  void sr_ff::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    assert(s || r); assert(q);
    if(s) win.push_back(s);
    if(r) win.push_back(r);
    wout.push_back(q);
  }


  // D FLIP-FLOP

  d_ff::d_ff(wire *q, wire *d):d(d),q(q)
  {
    assert(q);
    //    assert(d);
    rst=current->rst;
    clk=current->clk;
    init=0;
    name=current->new_uname(); current->modules.push_back(this);
  }

  d_ff::d_ff(wire *q, wire *d, string pref, int ix):d(d), q(q)
  {
    stringstream ss;
    assert(q);
    // assert(d);
    rst=current->rst;
    clk=current->clk;
    init=0;
    ss<<pref<<ix;
    name=ss.str(); current->modules.push_back(this);
  }

  void d_ff::pr_def()
  {
     ostream &o=current->ov;

    o<<"module d_ff0(rst, clk, q, d);\n";
    o<<"input rst, clk, d;\n";
    o<<"output q;\n";
    o<<"reg q;\n";
    o<<"always @(posedge clk or posedge rst) begin\n";
    o<<"  if(rst) q <= 1'b0; else\n";
    o<<"      q <= d;\n";
    o<<"end\n";
    o<<"endmodule\n\n";


    o<<"module d_ff1(rst, clk, q, d);\n";
    o<<"input rst, clk, d;\n";
    o<<"output q;\n";
    o<<"reg q;\n";
    o<<"always @(posedge clk or posedge rst) begin\n";
    o<<"  if(rst) q <= 1'b1; else\n";
    o<<"      q <= d;\n";
    o<<"end\n";
    o<<"endmodule\n\n";
  }

  void d_ff::pr_def_xilinx() { pr_def(); }


  void d_ff::pr_instance() // rst, clk, out, in -- no init
  {
    ostream &o=current->ov;
    assert(rst&&clk&&q);
    if(init)o<<"d_ff1 "; else o<<"d_ff0 ";
    o<<name<<"("<<rst->name<<","<<clk->name<<","<<q->name<<","<<wn(d,0)<<");\n";
  }
 
  void d_ff::dw_dot()
  {
    ostream &o=current->odot;
    assert(rst&&clk&&q);
    o<<name<<"[label=\"DFF"<<(init?"*":"")<<"\\n"<<name<<"\" shape=box]\n";
    drawin(d,"d");
    drawout(q,"q");
  }

  void d_ff::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    if( !d || !q ) cerr<<name<<"\n";
    assert(d); assert(q);
    win.push_back(d);
    wout.push_back(q);
  }


  // AND GATE

  andgate::andgate(wire *out, wire *in1, wire *in2): out(out)
  {
    assert(out && in1 && in2);
    win.push_back(in1);
    win.push_back(in2);
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);
  }

  andgate::andgate()
  {
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);
  }

  void andgate::pr_instance()
  {
    ostream &o=current->ov;
    assert(out);
    if(delay) o<<"and #"<<delay<<" "; else o<<"and ";
    o<<name<<"("<<out->name;

    for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++){
      assert(*w);
      o<<","<<(*w)->name;
    }
    o<<");\n";
  }

  void andgate::dw_dot()
  {
    ostream &o=current->odot;
    o<<name<<"[label=\"\" shape=box style=filled color=\"#ff8080\" height=0.2 width=0.3]\n";
    drawin(win);
    drawout(out,"");
  }

   void andgate::req_wire(vector<wire*> &win, vector<wire*> &wout)
   {
     assert(out);
     win = this->win;
     wout.push_back(out);
   }

  // OR GATE

  orgate::orgate(wire *out, wire *in1, wire *in2): out(out)
  {
    assert(out && in1 && in2);
    win.push_back(in1);
    win.push_back(in2);
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);
  }

  orgate::orgate()
  {
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);
  }

  void orgate::pr_instance()
  {
    ostream &o=current->ov;
    assert(out);
    if(delay) o<<"or #"<<delay<<" "; else o<<"or ";
    o<<name<<"("<<out->name;

      for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++){
	assert(*w);
	o<<","<<(*w)->name;
      }
    o<<");\n";

  }

  void orgate::dw_dot()
  {
    ostream &o=current->odot;
    o<<name<<"[label=\"\" shape=box style=filled color=blue height=0.2 width=0.3 ]\n";
    drawin(win);
    drawout(out,"");
  }
  
  void orgate::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    assert(out);
    win = this->win;
    wout.push_back(out);
  }



  // NOT GATE

  notgate::notgate(wire *out, wire *in): out(out),in(in)
  {
    assert(out && in);
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);
  }

  void notgate::pr_instance()
  {
    ostream &o=current->ov;
    assert(out&&in);
    if(delay) o<<"not #"<<delay<<" "; else o<<"not ";
    o<<name<<"("<<out->name<<","<<in->name<<");\n";
  }

  void notgate::dw_dot()
  {
    ostream &o=current->odot;
    assert(in && out);
    o<<name<<"[label=\"\" style=filled shape=circle color=black height=0.2 width=0.2]\n";
    drawin(in,"");
    drawout(out,"");
  }  

  void notgate::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    assert(in && out);
    win.push_back(in);
    wout.push_back(out);
  }

  counter::counter(wire *alarm, wire *start, wire *cenable, int startval):
    alarm(alarm),start(start),cenable(cenable),startval(startval)
  {
    assert(alarm); assert(cenable); assert(start);
    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);    
  }

  void counter::pr_def() // NOT UPDATED !!
  {
    ostream &o=current->ov;
    counter *cnt;
    int sv, sz, t;

    set<int> cvals;

    if(!current->vcounters) return;

    for(vector<vermod*>::iterator m=current->modules.begin();
	m!=current->modules.end();m++)
      if((cnt=dynamic_cast<counter*>(*m)))
	 if(cvals.count(cnt->startval) == 0) cvals.insert(cnt->startval);
      
    for(set<int>::iterator iv=cvals.begin(); iv!=cvals.end(); iv++){
      sz=0; t=1;
      while(t <= *iv){sz++; t+=t;}
      sv = t - *iv - 1;
      cerr<<"... using "<<sz<<" bits for startval="<<*iv<<" realsv="<<sv<<"\n";

      // no reset !!!
      o<<"module counter"<<*iv<<"(clk, alarm, start, enable);\n";
      o<<"input clk, start, enable;\n";
      o<<"output alarm;\n";
      o<<"reg ["<<sz-1<<":0]q;\n";
      o<<"wire ["<<sz<<":0]q1;\n";
      o<<"assign q1=q+1;\n";
      o<<"always @(posedge clk)\n";
      o<<"  if(start) q<="<<sv<<"; else q <= enable ? q1["<<sz-1<<":0] : q;\n";
      o<<"assign alarm=q1["<<sz<<"];\n";

      o<<"endmodule\n\n";
 
    }
    
  }

  void counter::pr_instance() // nothing
  {
  }
  
  // note inputs & outputs below are REVERSED
  // because that's the way the main module sees them
  void counter::aux_io()
  {
    if(current->vcounters){
      add_top2main(alarm);
      add_main2top(start);
      add_main2top(cenable);
    }
  }

  // prints top instances
  void counter::pr_top()
  {
    ostream &o=current->ov;
    
    if(!current->vcounters) return;

    assert(alarm && start && cenable);    
    o<<"counter"<<startval<<" "<<name<<"(.clk(clk),.alarm("<<alarm->name
     <<"),.start("<<wn(start,0)<<"),.enable("<<wn(cenable,0)<<") );\n";
  }

  void counter::dw_dot()
  {
    ostream &o=current->odot;
    assert(alarm && cenable && start);
    o<<name<<"[label=\""<<startval<<"\" style=filled shape=box fillcolor=yellow]\n";
    drawout(alarm,"a");
    drawin(start,"s");
    drawin(cenable,"e");
  }
  
  void counter::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    assert(alarm && cenable && start);
    win.push_back(cenable);
    win.push_back(start);
    wout.push_back(alarm);
  }



  // LUT


  void lut::pr_instance()
  {
    ostream &o=current->ov; 
    int nin, nlines, v, l, fv;

    nin=win.size();
    nlines=btable.size();

    o<<"assign "<<wout->name<<"=";
    for(l=0;l<nlines;l++){
      fv=1;
      for(v=0; v<nin; v++){
	if(btable[l][v]=='1'){if(!fv)o<<" & "; else fv=0; o<<wn(win[v],0);}
	if(btable[l][v]=='0'){if(!fv)o<<" & "; else fv=0; o<<"!"<<wn(win[v],0);}
      }
      if(l!=nlines-1)o<<" | ";
    }
    o<<";\n";
  }

  void lut::dw_dot()
  {
    ostream &o=current->odot; 
    string lab;
    int nin, nlines, v, l;

    nin=win.size();
    nlines=btable.size();

    lab=outplane?"":"*NOT*\\n";
    for(l=0;l<nlines;l++){
      for(v=0; v<nin; v++){
	if(btable[l][v]=='1')lab+=wn(win[v],0)+" ";
	if(btable[l][v]=='0')lab+=wn(win[v],0)+"' ";
      }
      if(l!=nlines-1)lab+="+\\n";
    }

    o<<name<<"[label=\""<< lab <<"\" shape=box]\n";
    drawout(wout,"");
    for(v=0;v<nin;v++) drawin(win[v],"");

  }
    
  void lut::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    win=this->win;
    wout.clear(); wout.push_back(this->wout);
  }
  
  void lut::pr_blif() {}; // simple but not necessary now
    
  lut::lut(wire *wout, vector<wire *> &win, vector<vector<char> > &btable, int outplane):
    wout(wout), win(win), btable(btable), outplane(outplane)
  {
    assert(wout);
    assert(win.size() > 1);
    assert(btable.size() > 0);
    assert(win.size() == btable[0].size());

    delay=current->delay;
    name=current->new_uname();
    current->modules.push_back(this);    
  }

}


