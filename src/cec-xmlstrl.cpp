#line 1200 "EsterelPrinter.nw"
#include "IR.hpp"
#include "AST.hpp"
#include "EsterelPrinter.hpp"
#include <iostream>
#include <stdlib.h>

struct Usage {};

int main(int argc, char *argv[])
{
  try {

    bool v7code = false;

    argc--, argv++;  // Skip program name

    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case '7':
	v7code = true;
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

    AST::ASTNode *an = dynamic_cast<AST::ASTNode*>(n);
    if (!an) throw IR::Error("Root node is not an AST node");

    AST::EsterelPrinter p(std::cout, v7code);
    an->welcome(p);

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (Usage &) {
    std::cerr << "Usage: cec-xmlstrl [-7]" << std::endl;
    exit(-1);
  }

  return 0;

}

