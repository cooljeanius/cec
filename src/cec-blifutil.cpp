#include "BLIF.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>

struct UsageError {};

int main(int argc, char *argv[])
{
  int verbose = 0;
  bool printBLIF = false;
  bool printDot = false;
  bool printStatistics = false;
  int cycles = 0;
  int outputstyle = 0;
  bool testv = false;
  bool printVerilog = false;
  std::ifstream testvectors;

  --argc; ++argv;

  try {

    while (argc > 0 && argv[0][0] == '-') {
      switch (argv[0][1]) {
      case 'b': printBLIF = true; break;
      case 'd': printDot = true; break;
      case 's': printStatistics = true; break;
      case 'V':
	--argc; ++argv;
	if (argc == 0) throw UsageError();
	verbose = atoi(argv[0]);
	break;
      case 'c':
	--argc; ++argv;
	if (argc == 0) throw UsageError();
	cycles = atoi(argv[0]);
	break;
      case 'o':
	--argc; ++argv;
	if (argc == 0) throw UsageError();
	outputstyle = atoi(argv[0]);
	break;
      case 't':
	--argc; ++argv;
	if (argc == 0) throw UsageError();
	testv = true;
	testvectors.open(argv[0]);
	break;
      case 'v': printVerilog = true; break;
      default:
	throw UsageError();
      }
      --argc; ++argv;
    }

    if (argc > 1) throw UsageError();

    {
      BLIF::Netlist *n;
      if (argc == 1) {
	std::ifstream i(argv[0]);
	if (!i.is_open()) {
	  std::cerr << "could not open " << argv[0] << std::endl;
	  return 1;
	}
	n = BLIF::read_blif(i);
	i.close();
      } else {
	n = BLIF::read_blif(std::cin);
      }

      if (printBLIF) print_blif(std::cout, *n);
      if (printDot) print_dot(std::cout, *n);
      if (printVerilog) print_verilog(std::cout, *n);

      if (printStatistics) {
	unsigned int gates = 0;
	unsigned int inputs = 0;
	unsigned int outputs = 0;
	unsigned int latches = 0;
	for ( std::vector<BLIF::Gate*>::const_iterator i = n->gates.begin() ;
	      i != n->gates.end() ; i++ ) {
	  if ( (*i)->is_input ) inputs++;
	  if ( (*i)->is_output ) outputs++;
	  if ( (*i)->is_latch ) latches++;
	  if ( !( (*i)->is_input || (*i)->is_latch) ) gates++;
	}
	
	std::cerr
	  << "gates: " << gates << " latches: " << latches
	  << " inputs: " << inputs << " outputs: " << outputs << std::endl;
      }

      if (cycles > 0 || testv) {

	// Run simulation

	BLIF::Simulator sim(*n);
	sim.debug = verbose & 1;

	if (outputstyle == 1) {
	  for ( std::vector<BLIF::Gate*>::const_iterator j =
		  sim.outputs.begin() ;
		j != sim.outputs.end() ; j++ )
	    std::cout << (*j)->name << ' ';
	  for ( std::vector<BLIF::Gate*>::const_iterator j =
		  sim.latches.begin() ;
		j != sim.latches.end() ; j++ )
	    std::cout << (*j)->name << ' ';
	  std::cout << '\n';
	}

	for ( int i = 0 ; cycles > 0 ? i < cycles : true ; i++ ) {
	  if ( testv ) {
	    std::string line;
            getline(testvectors, line);
	    if (testvectors.fail()) break;
	    if ( line.size() < sim.inputs.size() ) {
	      std::cerr << "Not enough inputs (" << line.size() << '<'
			<< sim.inputs.size() << ") in test vector file\n";
	      break;
	    }

	    std::string::const_iterator j = line.begin();
	    for ( std::vector<BLIF::Gate*>::const_iterator i =
		    sim.inputs.begin() ; i != sim.inputs.end() ; j++ ) {
	      if ((*j) != ' ') {
		sim.setInput(*i, (*j) == '1');
		i++;
	      }
	    }
	  }

	  sim.simulate();
	  switch (outputstyle) {
	  case 1:
	    for ( std::vector<BLIF::Gate*>::const_iterator j =
		    sim.outputs.begin() ;
		  j != sim.outputs.end() ; j++ )
	      std::cout << (sim.getOutput(*j) ? '1' : '0');
	    std::cout << ' ';
	    for ( std::vector<BLIF::Gate*>::const_iterator j =
		    sim.latches.begin() ;
		  j != sim.latches.end() ; j++ )
	      std::cout << (sim.getLatch(*j) ? '1' : '0');
	    std::cout << '\n';
	    break;
	  default:
	    {
	      char buf[5];
	      sprintf(buf, "%4d", i);
	      std::cout << buf;
	      for ( std::vector<BLIF::Gate*>::const_iterator j =
		      sim.outputs.begin() ;
		    j != sim.outputs.end() ; j++ )
		std::cout << ' ' << (*j)->name << '='
			  << (sim.getOutput(*j) ? '1' : '0');
	      std::cout << '\n';
	    }
	    break;
	  }
	}
      }
    }

  } catch (UsageError &) {

    std::cerr <<
      "Usage: cec-blifutil [-b] [-d] [-s] [-V <level>] [-c <cycles>] [-o <style>] [-v] [<file.blif>]\n"
      "-v   Print Verilog version of input netlist\n"
      "-b   Print BLIF version of input netlist\n"
      "-d   Print dot version of input\n"
      "-s   Print statistics on the netlist\n"
      "-c <cycles> Simulate for the given number of cycles\n"
      "-t <vectorfile> Use this file of test vectors as input stimuli\n"
      "-o <style> Set the simulation output style to 0, 1, 2, etc.\n"
      "-V <level> Set verbosity to <level>\n"
      ;
    return 1;
  }

  return 0;
}
