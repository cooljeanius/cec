#line 1018 "GRCsim.nw"
#include "GRCsim.hpp"
#include <fstream>

using namespace std;

struct UsageError {};

int main(int argc, char *argv[])
{
  try {
    int debug = 0;
    int maxticks = 0;
    string testvectorfile;

    ++argv, --argc;

    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'd':
	if (argc == 1) throw UsageError();
	++argv, --argc;
	debug = atoi(argv[0]);
	break;
      case 'c':
	if (argc == 1) throw UsageError();
	++argv, --argc;
	maxticks = atoi(argv[0]);
	break;
      case 't':
	if (argc == 1) throw UsageError();
	++argv, --argc;
	testvectorfile = argv[0];
	break;
      default:
	cerr << "Unrecognized option " << argv[0] << endl;
	throw UsageError();
      }
      ++argv, --argc;
    }

    if ( argc != 0 ) throw UsageError();

    IR::XMListream r(std::cin);
    IR::Node *n;
    r >> n;

    AST::Modules *mods = dynamic_cast<AST::Modules*>(n);
    if (!mods) throw IR::Error("Root node is not a Modules object");

    AST::Module *module = mods->modules.front();
    assert(module);

    AST::GRCgraph *gn = dynamic_cast<AST::GRCgraph*>(module->body);
    assert(gn);

    AST::GRCNode::NumMap cfgmap;
    AST::STNode::NumMap stmap;

    gn->enumerate(cfgmap, stmap);
    
    AST::GRCsim simulator(gn, module, cfgmap, stmap, debug, std::cout);
    if ( maxticks > 0 )
      simulator.execute_max(maxticks);
    else if (!testvectorfile.empty()) {
      std::ifstream tvf(testvectorfile.c_str());
      if (tvf.bad()) {       
	cerr << "Error opening test vector file " << testvectorfile << '\n';
	exit(-2);
      }
      simulator.execute_vectors(tvf);
      tvf.close();
    } else throw UsageError();

  } catch (IR::Error &e) {
    cerr << e.s << endl;
    exit(-1);
  } catch (UsageError &) {
    cerr << "Usage: cec-grcsim [-d <level>] -c <cycles> | -t <vectorfile>\n"
      "-d <level>   Enable debugging at the given level\n"
      "-c <cycles>  Simulate with no inputs for this many cycles maximum\n"
      "-t <vectorfile> Simulate taking inputs from the given vector file\n";
    exit(-1);
  }

  return 0;
}
