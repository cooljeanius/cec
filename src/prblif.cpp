#include "cverilog.hpp"

#include <vector>

namespace CVER{

  void Cverilog::build_blif(string modname)
  {

    reorder_wires();

    ob<<".model "<<modname<<"_blif\n";
    ob<<".inputs";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->input)
	ob<<" "<<(*w)->name;
    ob<<"\n";

    ob<<".outputs";
    for(vector<wire*>::iterator w=wires.begin(); w!=wires.end(); w++)
      if((*w)->output)
	ob<<" "<<(*w)->name;
    ob<<"\n";

    //    o<<".clock clk\n";

    ob<<".names w_0\n";
    ob<<".names w_1\n"<<" 1\n";

    for(vector<vermod*>::iterator m=modules.begin(); m!=modules.end();m++)
      (*m)->pr_blif();


    ob<<".end\n";
  }

  void strap::pr_blif()
  {
    ostream &o=current->ob;
    if(out->input) return;
    //    cerr<<"# strap \n";
    //    cerr<<"# in = "<<in->name<<"\n";
    //    cerr<<"#out = "<<out->name<<"\n";

    //    o<<"# strap \n";
    //    o<<"# in = "<<in->name<<"\n";
    //    o<<"#out = "<<out->name<<"\n";


    o<<".names "<<in->name<<" "<<out->name<<"\n";
    o<<"1 1\n";
  }

  void andgate::pr_blif()
  {
    ostream &o=current->ob;

    o<<".names";
    for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++){
      assert(*w);
      o<<" "<<(*w)->name;
    }
    
    assert(out);
    o<<" "<<out->name<<"\n";
    
    for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++)
      o<<"1";
    o<<" 1\n";
  }

  void orgate::pr_blif()
  {
    ostream &o=current->ob;

    o<<".names";
    for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++){
      assert(*w);
      o<<" "<<(*w)->name;
    }
    assert(out);
    o<<" "<<out->name<<"\n";

    for(vector<wire*>::iterator w=win.begin(); w!=win.end(); w++){
      for(vector<wire*>::iterator w1=win.begin(); w1!=win.end(); w1++)
	o<<((w==w1)?"1":"-");
      o<<" 1\n";
    }

  }
  
  void notgate::pr_blif()
  {
    ostream &o=current->ob;
    
    o<<".names";
    assert(in && out);
    o<<" "<<in->name;
    o<<" "<<out->name<<"\n";
    o<<"0 1\n";
  }
  
  void d_ff::pr_blif()
  {
    ostream &o=current->ob;
    assert(d && q && rst); // rst not used
    //    o<<".latch "<<d->name<<" "<<q->name<<" re clk "<<init<<"\n";
    o<<".latch "<<d->name<<" "<<q->name<<" "<<init<<"\n";
  }
  
  void counter::pr_blif()
  {
    ostream &o=current->ob;
    int sz, t, i, sv;
    vector<string> dname, qname;
    string toggle;
    vector<int> stbits;

    if(current->vcounters) return;
    
    sz=0; t=1;
    while(t <= startval){sz++; t+=t;}
    sv = t - startval - 1;
    o<<"# counter: using "<<sz<<" bits for startval="<<startval<<" hwsv="<<sv<<"\n";
      
    stbits.resize(sz);
    for(t=0;t<sz;t++){
      stbits[t] = sv & 1;
      sv >>= 1;
    }

    // just for now : binary counter
    dname.resize(sz); qname.resize(sz);

    for(t=0;t<sz;t++){
      dname[t]=current->new_wname();
      qname[t]=current->new_wname();
      toggle = current->new_wname();

      // toggle
      o<<".names "<<cenable->name<<" ";
      for(i=t-1;i>=0;i--) o<<qname[i]<<" ";
      o<<toggle<<"\n";
      for(i=0;i<t+1;i++)o<<"1"; o<<" 1\n";

      o<<".names "<<start->name<<" "<<qname[t]<<" "<<toggle<<" "<<dname[t]<<"\n";
      // start value
      if(stbits[t]) {
	o<<"1-- 1\n";
      }
      o<<"001 1\n";
      o<<"010 1\n";
      o<<".latch "<<dname[t]<<" "<<qname[t]<<" 0\n";

    }
   
    o<<".names ";
    for(i=0;i<sz;i++) o<<qname[i]<<" "; o<<alarm->name<<"\n";
    for(i=0;i<sz;i++)o<<"1"; o<<" 1\n";

  }

}
