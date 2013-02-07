#line 471 "GRCPrinter.nw"
#include "IR.hpp"
#include "AST.hpp"
#include "GRCPrinter.hpp"
#include <iostream>
#include <stdlib.h>

struct UsageError {};

int main(int argc, char *argv[])
{
  try {

    bool clean = false;
    bool stlink = false;

    --argc; ++argv;

    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'c': clean = true; break;
      case 'l': stlink = true; break;
      default:
	std::cerr << "unrecognized option \"" << argv[0] << "\"\n";
	/* FALLTHROUGH */
      case 'h':
        throw UsageError();
      }
      --argc; ++argv;
    }

    if (argc > 0) throw UsageError();

    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;

    AST::Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    for ( std::vector<AST::Module*>::iterator i = mods->modules.begin() ;
	  i != mods->modules.end() ; i++ ) {
      assert(*i);

      AST::GRCgraph *g = dynamic_cast<AST::GRCgraph*>((*i)->body);
      if (!g) throw IR::Error("Module is not in GRC format");

      GRCDot::GRCDot(std::cout, g, *i, stlink, clean);
    }
  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (UsageError &) {
    std::cerr <<
      "Usage: cec-grcdot [-c] [-s] [-h]\n"
      "-c   Print a cleaner version of the graph\n"
      "-l   Draw links to the ST graph\n"
      "-h   Print this usage message\n"
      ;
    return 1;

  }
  return 0;
}
