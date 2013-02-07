#line 407 "GRCPrinter.nw"
#include "GRCPrinter.hpp"
#include <cassert>

namespace GRCDot {

  
#line 36 "GRCPrinter.nw"
void GRCDP::visit_cfg(GRCNode *n) {
  assert(n);

  if (reached.find(n) == reached.end()) {
    reached.insert(n);
    
    // Print a definition for the node
    assert(cfgnum.find(n) != cfgnum.end());

    mynum = cfgnum[n]; // used by most visitors
    o << 'n' << mynum << ' ';
    n->welcome(*this);

    // Draw data dependencies

    for (vector<GRCNode *>::const_iterator k = n->dataPredecessors.begin() ; 
	 k != n->dataPredecessors.end() ; k++) {
      assert(cfgnum.find(*k) != cfgnum.end());
      o << 'n' << cfgnum[*k] << " -> n" << cfgnum[n];
      if (clean) o << " [color=red]\n";
      else o << " [color=red constraint=false]\n";
    }

    /*for (vector<GRCNode *>::const_iterator k = n->dataSuccessors.begin() ; 
	 k != n->dataSuccessors.end() ; k++) {
      assert(cfgnum.find(*k) != cfgnum.end());
      o << 'n' << cfgnum[n] << " -> n" << cfgnum[*k];
      if (clean) o << " [color=blue]\n";
      else o << " [color=blue constraint=false]\n";
    }*/
    // Draw control dependencies

    for ( vector<GRCNode*>::iterator j = n->successors.begin() ;
	  j != n->successors.end() ; j++ ) {
      if (*j) {
	o << 'n' << cfgnum[n] << " -> n" << cfgnum[*j];
	if ( n->successors.size() > 1) {
	  if (clean) {
	    if (dynamic_cast<Switch*>(n) != NULL ||
		dynamic_cast<Sync*>(n) != NULL)
	      o << " [label=\"" << j - n->successors.begin() << "\"]";
	    else if (dynamic_cast<Test*>(n) != NULL &&
		     j == n->successors.end() - 1)
	      o << " [label=\"P\"]";
	  } else {
	    o << " [label=\"" << j - n->successors.begin() << "\"]";
	  }
	}
	o << '\n';
      } else if (!clean) {
	  o << 'n'<< cfgnum[n] << " -> n" << nextnum << "[label=\""
	    << j-n->successors.begin() << "\"]" << '\n';
	  o << 'n' << nextnum++
	    << " [shape=octagon style=filled color=black]\n";
      }
    }

    // Visit control successors and predecessors

    for ( vector<GRCNode*>::iterator j = n->successors.begin() ;
	  j != n->successors.end() ; j++ )
      if (*j) visit_cfg(*j);

    for ( vector<GRCNode*>::iterator j = n->predecessors.begin() ;
      j != n->predecessors.end() ; j++ ) visit_cfg(*j);

    // Visit data successors and predecessors

    for ( vector<GRCNode*>::iterator j = n->dataSuccessors.begin() ;
	  j != n->dataSuccessors.end() ; j++ ) visit_cfg(*j);

    for ( vector<GRCNode*>::iterator j = n->dataPredecessors.begin() ;
      j != n->dataPredecessors.end() ; j++ ) visit_cfg(*j);    
  }
}
#line 129 "GRCPrinter.nw"
Status GRCDP::visit(Switch &s) {
  if (clean) {
    o << "[label=\"s" << stnum[s.st]
      << "\" shape=diamond peripheries=2]\n";
    o << "{ rank=same n" << mynum << " n" << stnum[s.st] << " }\n";
  } else {
    o << "[label=\"" << mynum << " switch ";
    o << stnum[s.st]
      << "\" shape=diamond color=pink style=filled]\n";
    drawSTlink(&s,s.st);
  }
  return Status();
}

Status GRCDP::visit(Test &s) {
  o << "[label=\"";
  if (!clean) o << mynum << " test ";
  s.predicate->welcome(ep);
  o << "\" shape=diamond]\n";
  return Status();
}

Status GRCDP::visit(STSuspend &s){
  o << "[label=\"";
  if (!clean) o << mynum << " Suspend ";
  o << stnum[s.st]
    << "\" shape=egg]\n";
  return Status();
}

Status GRCDP::visit(Terminate &s) {
  if (clean) {
    o << "[label=\"" << s.code
      << "\" shape=octagon]\n";
  } else {
    o << "[label=\"" << mynum << ' ' << s.index << '@'
      << s.code
      << "\" shape=octagon color=red style=filled "
         "fontcolor=white fontname=\"Times-Bold\"]\n";
  }
  return Status();
}

Status GRCDP::visit(Sync &s) {
  o << "[label=\"";
  if (!clean) o << mynum << " sync" << " " << stnum[s.st];
  o << "\" shape=invtriangle]\n";

  // Set all the predecessors (should be Terminates) at the same level
  o << "{ rank=same; ";
  for ( vector<GRCNode*>::iterator i = s.predecessors.begin() ;
	i != s.predecessors.end() ; i++ )
    o << 'n' << cfgnum[*i] << "; ";
  o << "}\n";
  return Status();
}

Status GRCDP::visit(Fork &s) {
  o << "[label=\"";
  if (!clean) o << mynum << " fork";
  o << "\" shape=triangle]\n";
  return Status();
}

Status GRCDP::visit(Action &s) {
  o << "[label=\"";
  if (!clean) o << mynum << " action ";
  s.body->welcome(ep);
  o << '\"';
  if (dynamic_cast<Emit*>(s.body))
    o << " shape=house orientation=270]\n";
  else
    o << " shape=box]\n";
  return Status();
}

Status GRCDP::visit(Enter &s) {
  if (clean) {
    // Calculate the child number 

    STNode *n = s.st;
    STNode *parent = NULL;
    STexcl *exclusive = NULL;
    for (;;) {
      parent = n->parent;
      exclusive = dynamic_cast<STexcl*>(parent);
      if ( exclusive != NULL ) break;
      n = parent;
    }
    vector<STNode*>::iterator i = exclusive->children.begin();
    while (*i != n && i != exclusive->children.end()) i++;
    int childnum = i - exclusive->children.begin();

    o << "[label=\"s" << stnum[parent] << '=' << childnum << "\" shape=box]\n";

  } else {
    o << "[label=\"" << mynum << " enter " << stnum[s.st]
      << "\" shape=house color=palegreen1 style=filled]\n";
  }
  return Status();
}

Status GRCDP::visit(EnterGRC &s){
  o << "[label=\"";
  if (!clean) o << mynum << " EnterGRC";
  o << "\"]\n";
  return Status();
}

Status GRCDP::visit(ExitGRC &s){
  o << "[label=\"";
  if (!clean) o << mynum << " ExitGRC";
  o << "\"]\n";
  return Status();
}

Status GRCDP::visit(Nop &s){
  o << "[label=\"";
  if (!clean) o << mynum << " ";
  if (s.isflowin()) o << "*"; else 
    if (s.isshortcut()) o << "#";
    else o << "\\n" << s.code;
  o << "\" shape=circle]\n";
  return Status();
}

Status GRCDP::visit(DefineSignal &s){
  o << "[label=\"";
  if (!clean) o << mynum << " DefS\\n";
  o << s.signal->name
    << "\" shape=house orientation=90]\n";
  return Status();
}
#line 275 "GRCPrinter.nw"
void GRCDP::visit_st(STNode *n) {
  assert(n);

  mynum = stnum[n];
  o << 'n' << mynum << ' ';
  n->welcome(*this);

  // Visit children

  for ( vector<STNode*>::const_iterator i = n->children.begin() ;
	i != n->children.end() ; i++ )
    if (*i) {
      visit_st(*i);
      o << 'n' << stnum[n] << " -> n" << stnum[*i];
      if (!clean || dynamic_cast<STexcl*>(n) != NULL)
	o << " [label=\"" << (i - n->children.begin()) << "\"]";
      o << '\n';
    } else {
      o << 'n'<< stnum[n] << " -> n" << nextnum << "[label=\""
	<< i - n->children.begin() << "\"]"<<'\n';
      o << 'n' << nextnum++;
      if (clean) o <<  " [shape=point]\n";
      else o << " [shape=octagon style=filled color=black]\n";
    }
}
#line 310 "GRCPrinter.nw"
Status GRCDP::visit(STexcl &s) {
  if (clean) {
    o << "[label=\"s" << mynum << "\" shape=diamond peripheries=2]\n";
  } else {
    o << "[label=\"" << mynum << "\" shape=diamond color=pink style=filled]\n";
  }
  return Status();
}

Status GRCDP::visit(STref &s) {
  if (clean) {
    o << "[shape=box label=\"\"]\n";
  } else {
    o << "[label=\"" << mynum << " ";
    if(s.isabort()) o << "A";
    if(s.issuspend()) o << "S";
    o << "\" ]\n";
  }
  return Status();
}

Status GRCDP::visit(STpar &s) {
  if (clean) {
    o << "[label=\"\" shape=triangle]\n";
  } else { 
    o << "[label=\"" << mynum << "\" shape=triangle]\n";
  }
  return Status();
}

Status GRCDP::visit(STleaf &s) {
  if (clean) {
    o << "[label=\"";
    if(s.isfinal()) o << "*";
    o << "\" shape=box]\n";
  } else { 
    o << "[label=\"" << mynum << " ";
    if(s.isfinal()) o << "*";
    o << "\" shape=box]\n";
  }
  return Status();
}

#line 414 "GRCPrinter.nw"
  void drawDot(std::ostream &o, GRCgraph *g, Module *m, bool drawstlink,
	      bool clean, CFGmap &cfgmap, STmap &stmap, int mxnode)
  {
    GRCDP visitor(o, cfgmap, stmap, mxnode+1);
    visitor.drawstlink = drawstlink;
    visitor.clean = clean;

    o << "digraph " << m->symbol->name << " {" << std::endl;
    o << "size=\"7.5,10\"\n";

    visitor.visit_st(g->selection_tree);
    visitor.visit_cfg(g->control_flow_graph);  

    o << "}" << std::endl;
  }

  int GRCDot(std::ostream &o, GRCgraph *g, Module *m, bool drawstlink,
	      bool clean,  CFGmap &cfgmap, STmap &stmap, int mxnode)
  {
     assert(g);
     assert(m);
     assert(m->symbol);
     mxnode = g->enumerate(cfgmap, stmap, mxnode);
     drawDot(o, g, m, drawstlink, clean, cfgmap, stmap, mxnode);
     return mxnode;
  }

  void GRCDot(std::ostream &o, GRCgraph *g, Module *m, bool drawstlink,
	      bool clean)
  {
    assert(g);
    assert(m);
    assert(m->symbol);

    CFGmap cfgmap;
    STmap stmap;

    int mxnode = g->enumerate(cfgmap, stmap);
    drawDot(o, g, m, drawstlink, clean, cfgmap, stmap, mxnode);
   
  }

  void GRCDP::drawSTlink(GRCNode *g, STNode *s)
  {
    o << "{ rank=same; n" << cfgnum[g] << "; n" << stnum[s] << " }\n";
    if (!drawstlink) return;

    assert( stnum.find(s) != stnum.end() );

    o << 'n' << cfgnum[g] << " -> n" << stnum[s];
    o << "[color=blue constraint=false]";
    o << '\n';
  }
}
