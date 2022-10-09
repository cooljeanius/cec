#include <set>
#include <map>
#include <fstream>
#include "cverilog.hpp"
#include <algorithm>

namespace CVER {

  // Cverilog main class

  Cverilog *current;
  wire *w_0, *w_1;
  int monitor_level;
  
  Cverilog::Cverilog(ostream &ov, ostream &ob, ostream &odot):
    ov(ov), ob(ob), odot(odot) {
      current=this;
      w_0=new wire("w_0"); 
      w_1=new wire("w_1");
      clk=new wire("clk");
      rst=new wire("rst");
      w_0->monitor=w_1->monitor=rst->monitor=0;
      wires.clear();

      w_prefix="w_";
      labwire=2;
      labcirc=0;
      monitor_level=1;
      delay=0; // default
      vcounters=0;
 }
    

  void Cverilog::reorder_wires() // first outputs, then inputs, w_finished (output) is last
  {
    vector<wire*> t;
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end();w++)
      if((*w)->output && (*w)->name!="w_finished") t.push_back(*w);
    
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end();w++)
      if((*w)->input) t.push_back(*w);
    
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end();w++)
      if(!(*w)->input && !(*w)->output) t.push_back(*w);

    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end();w++)
      if((*w)->name=="w_finished") t.push_back(*w);

    wires=t;
  }

  void Cverilog::build() // the body - just module main: used also by blif2v
  {

    reorder_wires();

    ov<<"module main(clk, rst";

    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input || (*w)->output)
	ov<<", "<<(*w)->name;
    ov<<");\n";

    ov<<"input clk, rst;\n";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input)
	ov<<"input "<<(*w)->name<<";\n";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->output)
	ov<<"output "<<(*w)->name<<";\n";

    ov<<"wire w_0, w_1;\n";
    ov<<"assign w_0=0;\n";
    ov<<"assign w_1=1;\n";

    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if(!(*w)->input && !(*w)->output) ov<<"wire "<<(*w)->name<<";\n";    


    for(vector<vermod*>::iterator m=modules.begin(); m!=modules.end();m++)
      (*m)->pr_instance();

    ov<<"endmodule\n\n";
  }

  void Cverilog::par_rename(string s){
    ov<<", "<<"."<<s<<"("<<s<<")";
  }


  int Cverilog::testvectors(int debugsignals)
  {
    int x, step;
    char buf[200]; char *p;

    ifstream tv("testvectors");

    if(!tv.good()) return -1;

    step = 0;
    while(1){
      tv.getline(buf,200); if(tv.fail()) break;
      p = buf;
      if(step==0) ov<<"  rst=1; clk=0; #500 rst=0;\n"; else ov<<"  #100 clk=1; #500 clk=0;\n";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input){
	  switch(*p++){
	  case  '0': x=0; break;
	  case  '1': x=1; break;
	  default: cerr<<"wrong testvectors\n";exit(-1);
	  }
	  ov<<"  "<<(*w)->name<<" = "<<x<<";\n";      
	}
      // display
      ov<<"  #400 $display(\""<<step*1000<<"\"";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input || (*w)->output) ov<<",\" \","<<(*w)->name; else
	  if(debugsignals && (*w)->monitor>=monitor_level) ov<<",\" \",u_top.u_main."<<(*w)->name;
      ov<<");\n";

      step++;

    }

    tv.close();
    return 0;

  }

  void Cverilog::build_interface(string modname, bool v5, bool xilinx, bool tbench, bool debugsignals)
  {
    int stop_wf;

    reorder_wires();

    // module testbench
    if(tbench){
      ov<<"module testbench;\n";
      
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output) ov<<"wire "<<(*w)->name<<";\n";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input) ov<<"reg "<<(*w)->name<<";\n";
      ov<<"reg clk;\n";
      ov<<"reg rst;\n\n";
      
      // instantiate main module
      ov<<"top u_top(.rst(rst), .clk(clk)";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output || ((*w)->input )) par_rename((*w)->name);
      ov<<");\n";

      // simulation
       
      ov<<"initial begin\n";

      ov<<"$display(\"outputs:";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output && (*w)->name!="w_finished") ov<<" "<<(*w)->name;
      ov<<"\");\n";
      ov<<"$display(\"inputs:";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input) ov<<" "<<(*w)->name;
      ov<<"\");\n";
      ov<<"$display(\"internals:";
      if(debugsignals) for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if(!(*w)->output && !(*w)->input && (*w)->monitor >=monitor_level ) ov<<" "<<(*w)->name;
      ov<<"\");\n";
      
      stop_wf = 0;
      ov<<"$display(\"w_finished:";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output && (*w)->name=="w_finished") { ov<<" "<<(*w)->name; stop_wf = 1;}
      ov<<"\");\n";
      
      if(testvectors(debugsignals)==-1) {
	
	// monitor in same order (already sorted)
	
	ov<<"  $monitor($time,";
	for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	  if((*w)->input || (*w)->output)
		  ov<<",\" \","<<(*w)->name;
	  else if(debugsignals && (*w)->monitor>=monitor_level)
		  ov<<",\" \",u_top.u_main."<<(*w)->name;
	ov<<");\n";
    
	// link all inputs to 0 : no stimulus
	for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	  if((*w)->input) ov<<"  "<<(*w)->name<<" = 0;\n";
	// it w_finished is present, use it
	cerr<<"no testvectors found. all inputs assumes to be 0\n";
	ov<<"  clk=0; rst=1;\n  #500 rst=0;\n";
	if(stop_wf) ov<<"  while(w_finished == 0) begin"; else
	  ov<<"  while($time<1000*11000) begin\n";
	//    ov<<"  while($time < 1000000010 && w_finished == 0) begin\n";
	
	ov<<"     #500 clk=1;\n";
	ov<<"     #500 clk=0;\n";
	ov<<"  end\n";
	
      }
      
      ov<<"end\n"; // initial
      ov<<"endmodule\n\n";
    }

    if(xilinx){
      // module xilinx
      ov<<"module "<<modname<<"_xilinx(rst, clk";
      // add real world inputs / outputs : will be buffered
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input) ov<<", ipad_"<<(*w)->name;
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output) ov<<", opad_"<<(*w)->name;
      ov<<");\n";
      // declare inputs / outputs
      ov<<"input rst, clk;\n";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input) ov<<"input ipad_"<<(*w)->name<<";\n";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output) ov<<"output opad_"<<(*w)->name<<";\n";
      
      // instantiate main module
      ov<<modname<<" u_top(.clk(clk), .rst(rst)";
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output || ((*w)->input )) par_rename((*w)->name);
      ov<<");\n";
      
      // add ffps to input and output signals
      
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->input) ov<<" d_ff0 u_I_"<<(*w)->name<<"(rst, clk, "<<(*w)->name<<
			  ", ipad_"<<(*w)->name<<");\n";
      
      for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
	if((*w)->output) ov<<" d_ff0 u_O_"<<(*w)->name<<"(rst, clk, opad_"<<(*w)->name<<
			   ", "<<(*w)->name<<");\n";
      ov<<"endmodule\n\n";
    }
      
    // module top
    ov<<"module "<<modname<<"(rst, clk";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input || (*w)->output) ov<<", "<<(*w)->name;
    ov<<");\n";
    ov<<"input rst, clk;\n";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input) ov<<"input "<<(*w)->name<<";\n";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->output) ov<<"output "<<(*w)->name<<";\n";
    
    if(!v5){
      // add counter & stuff  / inputs outputs
      for(vector<vermod*>::iterator m=modules.begin(); m!=modules.end();m++)
	(*m)->aux_io();
      // now remove outputs not generated by main
      for(set<wire*>::iterator iw=auxout.begin(); iw!=auxout.end(); iw++)
	if(auxin.count(*iw) == 0)
	  (*iw)->output=1;
      for(set<wire*>::iterator iw=auxin.begin(); iw!=auxin.end(); iw++)
	(*iw)->input=1;
    }
    
    // instantiate main
    ov<<modname<<"_blif u_main(.rst(rst), .clk(clk)";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input || (*w)->output) par_rename((*w)->name);
    ov<<");\n";
    
    if(!v5){
      // instantiate counter & stff
      for(vector<vermod*>::iterator m=modules.begin(); m!=modules.end();m++)
	(*m)->pr_top();
    }
    
    ov<<"endmodule\n\n";
     
    d_ff::pr_def();
    if(!v5) counter::pr_def();
      
  }

  //this will be called by the user program for debugging
  string Cverilog::wn(wire *w)
  {
    if(w) return w->name;
    return "NULL";
  }

  // this is called by modules in pr_instance()
  string vermod::wn(wire *w, int def)
  {
    if(!w) return def ? "1" : "0";
    if(w==w_0) return "0";
    if(w==w_1) return "1";
    return w->name;
  }

  int vermod::isconst(wire *w)
  {
    if(!w || w==w_0 || w==w_1) return 1; else return 0;
  }

  void vermod::add_top2main(wire *w)
  {
    assert(!isconst(w));
    if(current->auxin.count(w) == 0) current->auxin.insert(w);
  }
  void vermod::add_main2top(wire *w)
  {
    if(isconst(w)) return;
    if(current->auxout.count(w) == 0) current->auxout.insert(w);
  }

  void vermod::drawin(wire *w, string edge_label)
  {
    if(w)(current->odot)<<w->name<<" -> "<<name<<"[ label=\""<<edge_label<<"\"]\n";
  }

  void vermod::drawout(wire *w, string edge_label)
  {
    if(w)(current->odot)<<name<<" -> "<< w->name<<"[ label=\""<<edge_label<<"\"]"<<'\n';
  }

  void vermod::drawin(vector<wire *> &w)
  {
    for (vector<wire *>::iterator i = w.begin(); i != w.end(); i++)
      drawin((*i),"");
  }

  void Cverilog::drawdot()
  {
    odot<<"digraph circuit {\nsize=\"7.5,10\"\n";

    w_0->dw_dot();
    w_1->dw_dot();
    //    w_finished->dw_dot();

    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++){
      (*w)->dw_dot();
      }
    
    for(vector<vermod*>::iterator m=modules.begin(); m!=modules.end();m++){
      //      std::cerr<<"in\n";
      if(*m) {
	//std::cerr<<"name="<<(*m)->name<<'\n';
	(*m)->dw_dot();
      } else assert(0);
      //std::cerr<<"out\n";
    }

    odot<<"}\n";
  }

  wire* Cverilog::build_orgate(vector<wire*> &win)
  {

    int sz;
    orgate *ogate;
    wire *w;
    
    // NULL and w_0 ignored / w_1 shortcuts ; no wires -> 0


    vector<wire*>::iterator iv = win.begin();
    while(iv != win.end()){
      if((*iv) == w_1) return w_1;
      if((*iv) == NULL || (*iv) == w_0) iv = win.erase(iv); else iv++;
    }

    sort(win.begin(), win.end());
    win.erase(unique(win.begin(), win.end()), win.end());

    sz = win.size();
    if(sz == 0) return NULL;
    if(sz == 1) return win[0];

    for(vector<vermod*>::iterator m = current->modules.begin(); m != current->modules.end(); m++){
      ogate = dynamic_cast<orgate*> (*m);
      if(!ogate) continue;
      if(ogate->win == win) return ogate->out;
    }

    ogate = new orgate();
    ogate->win = win;
    w=new wire(); ogate->out=w;
    return w;
  }

  wire* Cverilog::build_orgate(wire *w1, wire *w2)
  {
    vector<wire*> tmp;
    tmp.push_back(w1);
    tmp.push_back(w2);
    return build_orgate(tmp);
  }

  wire* Cverilog::build_andgate(vector<wire*> &win)
  {
    int sz;
    andgate *agate;
    wire *w;
    
    // w_1 ignored / NULL or w_0 shortcuts  ; no wires -> 1

    vector<wire*>::iterator iv = win.begin();
    while(iv != win.end()){
      if((*iv) == NULL || (*iv)==w_0) return NULL;
      if((*iv) == w_1) iv = win.erase(iv); else iv++;
    }

    sort(win.begin(), win.end());
    win.erase(unique(win.begin(), win.end()), win.end());

    sz = win.size();
    if(sz == 0) return w_1;
    if(sz == 1) return win[0];

    for(vector<vermod*>::iterator m = current->modules.begin(); m != current->modules.end(); m++){
      agate = dynamic_cast<andgate*> (*m);
      if(!agate) continue;

      if(agate->win == win) return agate->out;
    }

    agate = new andgate();
    agate->win = win;
    w=new wire(); agate->out=w;
    return w;
   }

  wire* Cverilog::build_andgate(wire *w1, wire *w2)
  {
    vector<wire*> tmp;
    tmp.push_back(w1);
    tmp.push_back(w2);
    return build_andgate(tmp);
  }

  wire* Cverilog::build_notgate(wire *w)
  {
    notgate *ng;
    
    wire *wn;
    if(w==NULL || w==w_0) return w_1;
    if(w==w_1) return NULL;


    for(vector<vermod*>::iterator m = current->modules.begin(); m != current->modules.end(); m++){
      ng = dynamic_cast<notgate*> (*m);
      if(!ng) continue;

      if( w == ng->in ) return ng->out;
      if( w == ng->out) return ng->in;

    }


    wn=new wire();
    new notgate(wn,w);
    return wn;
  }



  // REMOVE UNUSED COMPONENTS

  void Cverilog::remove_unused()
  {
    vector<vermod*>::iterator im;

    vector<wire*> win, wout;
    vector<wire*>::iterator iw, jw;

    map<wire*, vermod*> wowner;
    map<wire*, vector<wire*> > wneeded;
    wire *w;

    vector<wire*> wreq;


    for(im=modules.begin();im!=modules.end();im++){

      win.clear(); wout.clear();
      (*im)->req_wire(win, wout);

      assert(wout.size()==1); //temporary
      w = wout[0];

      wowner[w] = *im;
      wneeded[w]= win;

    }

    //    wreq.push_back(w_finished); // required !
    for(iw=wires.begin(); iw!=wires.end();iw++){
      if((*iw)->output) (*iw)->required = 1; // on output is required !
      if((*iw)->required) {
	wreq.push_back(*iw);
      }
    }

    // checks wire dependency

    modules.clear();
    iw=wreq.begin();
    while(iw != wreq.end()){

      w=*iw; wreq.erase(iw);

      if(wowner.count(w) > 0){
	modules.push_back(wowner[w]);

	for(jw=wneeded[w].begin(); jw!=wneeded[w].end(); jw++){
	  if((*jw)->required) continue; // already processed
	  wreq.push_back(*jw);
	  (*jw)->required=10;
	}
      }
      
      iw=wreq.begin();
    }
    
    // removes unused wires and restore the require level
    wreq.clear();
    for(iw=wires.begin(); iw!=wires.end(); iw++){
      if((*iw)->required || (*iw)->input || (*iw)->output) wreq.push_back(*iw);
      if((*iw)->required==10) (*iw)->required=0;
    }
    wires = wreq;

  }




  // WIRE

  wire::wire(string name): name(name), input(0), output(0){
    current->wires.push_back(this);
    monitor=1;
    required=0;
    color="black";
  }

  wire::wire(string name, int i): input(0), output(0)
  {
    stringstream s;
    s<<name<<i;
    this->name=s.str(); 
    current->wires.push_back(this);
    monitor=1;
    required=0;
    color="black";
  }

  wire::wire(): input(0), output(0) {
    name=current->new_wname(); current->wires.push_back(this);
    monitor=0;
    required=0;
    color="black";
  }

  void wire::dw_dot()
  {
    string label;

    label = (input || output || monitor >= monitor_level) ? name : "";
    if(this==w_0)
	   label="0";
	if(this==w_1)
		label="1";
    if(name=="w_finished")
		label="STOP";

    current->odot<<name<<" [ label=\""<<label<<"\" color="<<color<<" fixed_size=true height=\"0.2\" width=\"0.2\" ";
    current->odot <<" fontsize=\"10\" ]\n";
  }



  // STRAP

  strap::strap(wire *out, wire *in): out (out), in(in)
  {
    assert(out);
    assert(in);
    name = current->new_wname();
    current->modules.push_back(this);

    // cerr<<"new strap: in= "<<in->name<<" out= "<<out->name<<"\n";

  }

  strap::strap(wire *probe, string wname, int monitor)
  {
    wire *w;

    if(!probe) probe=w_0;

    w=new wire(wname);
    w->monitor = monitor;
    out = w; in = probe;

    name = current ->new_wname();
    current->modules.push_back(this);
  }
  
  strap::strap(wire *probe, string wname, int ix, int monitor)
  {
    wire *w;
    stringstream w_name;

    if(!probe) probe=w_0;

    w_name << wname << ix;
    w=new wire(w_name.str());
    w->monitor = monitor;
    out = w; in = probe;

    name = current ->new_wname();
    current->modules.push_back(this);
  }

  void strap::pr_instance()
  {
    ostream &o=current->ov;
    assert(out&&in);
    o<<"assign "<<out->name<<"="<<in->name<<";\n";
  }

  void strap::dw_dot()
  {
    assert(out && in);
    ostream &o=current->odot;
    o<<in->name<<" -> "<<out->name<<'\n';
  }

  void strap::req_wire(vector<wire*> &win, vector<wire*> &wout)
  {
    assert(in && out);
    win.push_back(in);
    wout.push_back(out);
  }

}
