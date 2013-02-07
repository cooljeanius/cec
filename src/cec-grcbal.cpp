#include "IR.hpp"
#include "AST.hpp"
#include "GRCBAL.hpp"
#include <iostream>
#include <stdlib.h>

struct UsageError {};

int main(int argc, char *argv[])
{
  try {
    //    std::cout<<"Starting Program"<<std::endl;
    bool dumpDot = false;
    bool debug = false;

    --argc; ++argv;

    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 's': dumpDot = true; break;
      case 'd': debug = true; break;
      default:
	std::cerr << "unrecognized option \"" << argv[0] << "\"\n";
	/* FALLTHROUGH */
      case 'h':
        throw UsageError();
      }
      --argc; ++argv;
    }

    // std::cout<<"I'm here now"<<std::endl;
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

      GRCBAL::convertGRC(std::cout, g, *i, dumpDot, debug);
    }
  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (UsageError &) {
    std::cerr <<
      "Usage: cec-grcbal [-s] [-d] [-h]\n"
      "-s   Print the sequential BAL graph in .dot format\n"
      "-d   Print debugging output\n"
      "-h   Print this usage message\n"
      ;
    return 1;

  }
  //  std::cout<<"All Done"<<std::endl;
  return 0;
}
