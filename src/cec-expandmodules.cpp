#include "IR.hpp"
#include "AST.hpp"
#include "ExpandModules.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  std::string root_module;

  if ( argc > 1) {
    if (argc == 3 && strcmp(argv[1], "-main") == 0) {
      root_module = argv[2];
    } else {
      std::cerr << "Usage: cec-expandmodules [-main <modulename>]\n";
      return 1;
    }
  }

  try {
    IR::Node *root;
    IR::XMListream r(std::cin);
    r >> root;

    AST::Modules *m = dynamic_cast<AST::Modules*>(root);
    if (!m) throw IR::Error("Root node is not a Modules node");

    IR::XMLostream w(std::cout);

    if (root_module.length() > 0) {
      if ( !(m->module_symbols.contains(root_module))) {
	std::cerr << "cec-expandmodules: Unknown main module \""
		  << root_module << "\"\n";
	return 1;
      }
      AST::ModuleSymbol *root_sym =
	dynamic_cast<AST::ModuleSymbol*>(m->module_symbols.get(root_module));
      assert(root_sym);
      AST::Module *root = root_sym->module;
      assert(root);
      AST::Modules *nm = new AST::Modules();
      ExpandModules::Copier copier(m, nm);
      nm->add(copier.copyModule(root));
      w << nm;
    } else {
      AST::Modules *nm = ExpandModules::find_roots_and_rewrite(m);
      assert(nm);    
      w << nm;
    }

    
  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    return -1;
  }

  return 0;
}
