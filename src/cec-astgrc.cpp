#line 3248 "ASTGRC.nw"
#include "IR.hpp"
#include "AST.hpp"
#include <stdio.h>
#include "ASTGRC.hpp"
#include <iostream>
#include <vector>
#include <string.h>

int main(int argc, char *argv[])
{

  bool expand = true;

  if ( argc > 1) {
    if (argc == 2 && strcmp(argv[1], "-s") == 0) {
      expand = false;
    } else {
      std::cerr << "Usage: cec-astgrc [-s]\n";
      return 1;
    }
  }
  
  try {
    IR::Node *root;
    IR::XMListream r(std::cin);
    r >> root;

    AST::Modules *mods = dynamic_cast<AST::Modules*>(root);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    for ( std::vector<AST::Module*>::iterator i = mods->modules.begin() ;
	  i != mods->modules.end() ; i++ ) {
      assert(*i);
      // Compute completion codes for this module
      ASTGRC::CompletionCodes cc(*i);

      // Synthesize GRC for this module and replace it
      if (expand) {
	ASTGRC::GrcSynth synth(*i, cc);
	(*i)->body = synth.synthesize();
      } else {
	ASTGRC::RecursiveSynth synth(*i, cc);
	(*i)->body = synth.synthesize();
      }
      assert((*i)->body);

      AST::GRCgraph *g = dynamic_cast<AST::GRCgraph *>((*i)->body);
      assert(g);
      assert(g->control_flow_graph);

      // Add dependencies
      ASTGRC::Dependencies::compute(g->control_flow_graph);
    }

    IR::XMLostream w(std::cout);
    w << mods;
    
  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    return -1;
  }

  return 0;
}
