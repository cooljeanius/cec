#include "IR.hpp"
#include "AST.hpp"
#include "CPrinter.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace AST;
using namespace std;

class ModifyGraph
{
  set<GRCNode *> visited;
  GRCNode *top; //top of the flow graph

public:
  ModifyGraph(GRCNode *top):top(top)
  {
    EnterGRC *et = dynamic_cast<EnterGRC *>(top);
    assert(et);

    ReplaceNull2Nop(top);

    visited.clear();
    DFS(top);
  }

  GRCNode *addExit()
  {
    set<GRCNode *>::iterator i;
    set<GRCNode *> end_nodes;
    GRCNode *exitgrc = new ExitGRC();
    
    for (i = visited.begin(); i != visited.end(); i++){
      
      if ((*i)->successors.size() == 0){
	if (((*i)->predecessors.size() > 0) || (dynamic_cast<EnterGRC *>(*i)))
	  end_nodes.insert(*i);
      }
    }
    
    for (i = end_nodes.begin(); i != end_nodes.end(); i++){
      if (dynamic_cast<Nop *>(*i)){
	assert((*i)->predecessors.size() == 1);
	exitgrc->predecessors.push_back((*i)->predecessors[0]);
	vector<GRCNode *>::iterator j;
	GRCNode *par = (*i)->predecessors[0];
	for (j = par->successors.begin(); j != par->successors.end(); j++){
	  if (*j == *i){
	    *j = exitgrc;
	    break;
	  }
	}
      }
      else{
	(*i)->successors.push_back(exitgrc);
	exitgrc->predecessors.push_back(*i);
      }
    }
    
    return exitgrc;
  }
  
  GRCNode *removeEnter()
  {   
    GRCNode *org_top = top;
    assert(top->successors.size()>0);
    top = top->successors[0];
    assert(top != org_top);
    org_top->successors.clear();    
    top->predecessors.clear();
    assert(top->successors.size()>0);
    return top;
  }
  
private:
  void ReplaceNull2Nop(GRCNode *n)
  {
    if (visited.find(n) != visited.end())
      return;
    visited.insert(n);
    
    vector<GRCNode *>::iterator i;
    for (i = n->successors.begin(); i != n->successors.end(); i++)
      if (!*i){
	*i = new Nop();
	(*i)->predecessors.push_back(n);
	visited.insert(*i);
      }
      else
	ReplaceNull2Nop(*i);
  }

  void DFS(GRCNode *n)
  {
    assert(n);
    if (visited.find(n) != visited.end())
      return;
    visited.insert(n);
    vector<GRCNode *>::iterator i;
    for (i = n->successors.begin(); i != n->successors.end(); i++)
      DFS(*i);
  }

};

class CCodeGenerator
{
public:
  CCodeGenerator()
  {

  }

  void execute(ostream &o, Module &m, string basename, GRCNode *exitgrc)
  {
    CPrinter::Printer p(o, m, false);

    GRCgraph *g = dynamic_cast<AST::GRCgraph*>(m.body);
    
    if (!g) throw IR::Error("Module is not in GRC format");
    
    p.printDeclarations(basename);
    p.ioDefinitions();

    o <<
      "\n"
      "int " << m.symbol->name << "(void)\n"
      "{\n"
      ;


    for (map<Sync *, string>::iterator i = p.terminationVar.begin();
	 i != p.terminationVar.end(); i++)
      o << "\t"<<(*i).second<<"=-1;\n";

    // Generate the body of the function
    p.printStructuredCode(exitgrc, 1);

    // Call an emit function for each emitted signal and reset each input

    p.outputFunctions();
    p.resetInputs();

    STexcl *excl = dynamic_cast<STexcl*>(g->selection_tree);
    assert(excl);
    assert(!excl->children.empty());
    
    STleaf *terminal_leaf = dynamic_cast<STleaf*>(excl->children.front());
    
    if (terminal_leaf && terminal_leaf->isfinal()) {
      o << "  return (" << p.stateVar[excl] << " != 0);\n";
    } else {
      o << "  return 1;\n";
    }
    o <<
      "}\n";
    
    // Reset function
    
    o <<
      "\n"
      "int " << m.symbol->name << "_reset(void)\n"
      "{\n";

    p.resetInputs();
    
    o <<
    "  return 0;\n"
      "}\n";
  }

};

struct Usage {};

int main(int argc, char *argv[])
{
  try {

    std::string basename;

    argc--, argv++;  // Skip program name
    
    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'B':
	argc--, argv++;
	if (argc == 0) throw Usage();
	basename = argv[0];
	break;
      case 'h':
      default:
	throw Usage();
	break;
      }
      argc--, argv++;
    }

    if ( argc > 0 ) throw Usage();


    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;
    AST::Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    assert(mods->modules.size()>0);

    GRCgraph *gf = dynamic_cast<GRCgraph*>((*(mods->modules.begin()))->body);
    assert(gf);  
    GRCNode *top = gf->control_flow_graph;
    
    ModifyGraph *mg = new ModifyGraph(top);
    
    //add ExitGRC
    GRCNode *exitgrc = mg->addExit();
    assert(exitgrc);
    
    //remove EnterGRC
    gf->control_flow_graph = mg->removeEnter();
    assert(gf->control_flow_graph->successors.size()>0);

    // Generate C code
    AST::Module *m = mods->modules.front();
    assert(m);
    if (basename.empty()) basename = m->symbol->name;
    CCodeGenerator *cg = new CCodeGenerator();
    cg->execute(std::cout, *m, basename, exitgrc);

  } catch (IR::Error &e) {
    cerr << e.s << endl;
    exit(-1);
  } catch (Usage &) {
    std::cerr << "Usage: cec-scfgc [-B basename]" << std::endl;
    exit(-1);
  }

  return 0;

}
