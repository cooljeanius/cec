#include "IR.hpp"
#include "AST.hpp"
#include "Dismantle.hpp"
#include <iostream>

int main()
{
  try {
    IR::Node *root;
    IR::XMListream r(std::cin);
    r >> root;

    AST::ASTNode *an = dynamic_cast<AST::ASTNode*>(root);
    if (!an) throw IR::Error("Root node is not an AST node");

    Dismantle::Dismantler1 d1;
    d1.rewrite(an);
    assert(an);

    IR::XMLostream w(std::cout);
    w << an;
    
  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    return -1;
  }

  return 0;
}
