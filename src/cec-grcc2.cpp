#line 921 "GRCC2.nw"
#include "AST.hpp"
#include "GRCC2.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <string.h>

struct Usage {};

int main(int argc, char *argv[])
{
  try {

    std::string basename;
    bool ansi = false;

    argc--, argv++;  // Skip program name
    
    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'a':
	ansi = true;
	break;
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

    assert(mods->modules.size() > 0);
    AST::Module *m = mods->modules.front();
    assert(m);

    if (basename.empty()) basename = m->symbol->name;

    GRCC2::generateC(std::cout, *m, basename, ansi);

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (Usage &) {
    std::cerr << "Usage: cec-grcc2 [-a] [-B basename]" << std::endl;
    exit(-1);
  }
  return 0;
}

