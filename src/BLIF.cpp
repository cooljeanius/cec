#line 851 "BLIF.nw"
#include "BLIF.hpp"
#include <sstream>

namespace BLIF {
  
#line 111 "BLIF.nw"
Gate *Netlist::newGate(bool inv, string name)
{
  if (name.empty()) {
    std::ostringstream longname;
    longname << "g" << gates.size();
    name = longname.str();
  }
  Gate *g = new Gate(this, gates.size(), name, inv);
  gates.push_back(g);
  return g;
}
#line 856 "BLIF.nw"
  
#line 511 "BLIF.nw"
void print_dot(std::ostream &o, const Netlist &n)
{
  o << "digraph " << n.name << " {\n"
    "rankdir=\"LR\"\n"
    "node [shape=\"house\" orientation=\"270\"]\n"
    "size=\"10,7.5\"\n"
    ;

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ ) {
    Gate &g = *(*i);
    if (g.is_latch) {
      o << 'g' << g.id << "q [label=\"" << g.id << ':' << g.name
	<< " Q\" style=filled color=beige]\n";
    }
    o << 'g' << g.id << " [label=\"" << g.id << ':';
    o << g.name;
    if (g.is_latch) o << " D";
    o << '\"';
    if (g.is_input)
      o << " style=filled color=palegreen1 shape=house orientation=-90";
    else if (g.is_output)
      o << " shape=house orientation=-90 style=filled color=pink1";
    else if (g.is_latch)
      o << " style=filled color=green1";
    o << "]\n";
    if (g.outputs.size() > 0 ) {
      o << 'g' << g.id << "o [shape=point]\n";
      o << 'g' << g.id << " -> g" << g.id << "o [arrowhead=none arrowtail=";
      if (g.is_inverting) o << "odot";
      else o << "none";
      o << "]\n";
    }
    for ( vector<Input*>::const_iterator j = g.inputs.begin() ;
	  j != g.inputs.end() ; j++ ) {
      Input &input = *(*j);
      o << 'g' << input.driver->id;
      if (input.driver->is_latch) o << 'q';
      else o << 'o';
      o << " -> g" << g.id << " [arrowhead=";
      if (input.is_inverting) o << "odot"; else o << "none";
      o << "]\n";
    }
  }

  o << "}\n";
}
#line 857 "BLIF.nw"
  
#line 131 "BLIF.nw"
Netlist *read_blif(std::istream &i)
{
  try {
    BlifReader r(i);
    if (i.eof()) throw BlifReader::Error(0, "no model found");
    return r.readModel();
  } catch (BlifReader::Error e) {
    std::cerr << e.lineNumber << ':' << e.error << std::endl;
    return 0;
  }
}
#line 858 "BLIF.nw"
  
#line 181 "BLIF.nw"
Gate *BlifReader::getGate(string n)
{
  if (n.empty() || namedGate.find(n) == namedGate.end()) {
    Gate *result = netlist->newGate(false, n);
    namedGate.insert( std::make_pair(result->name, result));
    return result;
  } else {
    return namedGate[n];
  }
}
#line 200 "BLIF.nw"
Netlist *BlifReader::readModel()
{
  readLine();
  if (word.empty() || word[0] != ".model")
    throw Error(lineNumber, "expecting .model, found `" + line + '\'');
  if (word.size() != 2)
    throw Error(lineNumber, "too many words after .model");
  netlist = new Netlist(word[1]);
  if (inf.eof()) throw Error(lineNumber, "empty model");
  readLine();
  do {
    if (word.empty()) throw Error(lineNumber, "empty line?");
    if (word[0] == ".inputs") {
      
#line 244 "BLIF.nw"
for (vector<string>::const_iterator i = (word.begin()) + 1 ;
     i != word.end() ; i++) {
  Gate *new_input = getGate(*i);
  new_input->is_input = true;
}
readLine();
continue;
#line 214 "BLIF.nw"
    } else if (word[0] == ".outputs") {
      
#line 254 "BLIF.nw"
for (vector<string>::const_iterator i = (word.begin()) + 1 ;
     i != word.end() ; i++) {
  Gate *new_output = getGate(*i);
  new_output->is_output = true;
}
readLine();
continue;
#line 216 "BLIF.nw"
    } else if (word[0] == ".names") {
      
#line 269 "BLIF.nw"
vector<Gate*> inputs;
for (unsigned int i = 1 ; i < word.size() - 1 ; i++)
  inputs.push_back(getGate(word[i]));
Gate *output = getGate(word[word.size()-1]);

vector<Row> rows;
readLine();
while (!(inf.eof() || word.empty() || word[0][0] == '.')) {
  if ( !((word.size() == 1 && inputs.empty()) ||
	 word.size() == 2) )
    throw Error(lineNumber, "syntax error in PLA");  
  
  string andplane = (word.size() == 2) ? word[0] : "";
  string orplane = (word.size() == 2) ? word[1] : word[0];
  if (andplane.size() != inputs.size() || orplane.size() != 1)
    throw Error(lineNumber, "PLA wrong width");
  if (andplane.find_first_not_of("01-") != std::string::npos)
    throw Error(lineNumber, string("bad character in AND plane of PLA"));
  if (orplane != "0" && orplane != "1")
    throw Error(lineNumber, string("bad character in OR plane PLA"));
  rows.push_back(Row(andplane, orplane[0]));
  
  readLine();
}

switch (rows.size()) {
 case 0:
   // Empty row: a constant 0
   output->is_inverting = true;
   break;

 case 1:
   // A single row: a single gate
   output->is_inverting = (rows.front().orplane == '0');
   for ( unsigned int i = 0 ; i < rows.front().andplane.size() ; i++ ) {
     char v = rows.front().andplane[i];
     if ( v != '-' ) output->newInput(inputs[i], v == '0');
   }
   break;

 default:
   // Two or more rows: the output gate acts as an OR (or NOR, if the orplane
   // is 0s); the others act as ANDs
   {
     output->is_inverting = (rows.front().orplane == '1');
     for ( vector<Row>::const_iterator i = rows.begin() ;
	   i != rows.end() ; i++ ) {
       const string &andplane = (*i).andplane;
       int uniqueInput = -1;
       for ( string::const_iterator j = andplane.begin() ;
	     j != andplane.end() ; j++ ) {	 
	 if (*j != '-') {
	   if (uniqueInput == -1)
	     uniqueInput = j - andplane.begin();
	   else
	     uniqueInput = -2;
	 }
       }

       if (uniqueInput >= 0) {

	 // Single unique input: connect it to the output gate directly
	 // and invert it, since it is not an OR gate
	 output->newInput(inputs[uniqueInput], andplane[uniqueInput] == '1');

       } else {
	 // Multiple care inputs: generate an AND gate and connect it to
	 // the output
	 Gate *andplanegate = getGate();
	 andplanegate->is_inverting = false;
	 output->newInput(andplanegate, true);
	 for ( string::const_iterator j = andplane.begin() ;
	       j != andplane.end() ; j++ )
	   if (*j != '-')
	     andplanegate->newInput(inputs[j - andplane.begin()], (*j) == '0');
       }
     }
   }
   break;
}
#line 218 "BLIF.nw"
    } else if (word[0] == ".latch") {
      
#line 356 "BLIF.nw"
if (word.size() < 3)
   throw Error(lineNumber, "too few arguments to .latch: need at least two");
if (word.size() > 6)
  throw Error(lineNumber, "too many arguments to .latch: no more than five");
if (word.size() == 5)
  throw Error(lineNumber, "wrong number of arguments to .latch");
Gate *latch = getGate(word[2]);
latch->is_latch = true;
latch->newInput(getGate(word[1]), false);
if (word.size() == 4)
  latch->is_inverting = (word[3] == "1");
if (word.size() == 6)
  latch->is_inverting = (word[5] == "1");
readLine();
continue;
#line 220 "BLIF.nw"
    } else if (word[0] == ".end") {
      readLine();
      break;
    } else throw Error(lineNumber, "unrecognized `" + word[0] + "'");	  
  } while (!inf.eof());
  
  /*  
  std::cout << "Line:" << line << "---" << std::endl;
    for (vector<string>::const_iterator i = word.begin() ;
         i != word.end() ; i++ ) {
      std::cout << "`" << *i << "'" << std::endl;
    }
  }
  */

  return netlist;
}
#line 385 "BLIF.nw"
void BlifReader::readLine()
{
  
#line 396 "BLIF.nw"
char c;
line.resize(0);
while ( c = inf.get(), !inf.eof()) {
  if ( c == '\\') {
    if ( (c = inf.get()) == '\n') {
	lineNumber++;
	c = ' ';
    }
  } else if ( c == '#' ) {
    do {
      c = inf.get();
      if (inf.eof()) goto done;
    } while (c != '\n');
  }
  if ( c == '\n' && !line.empty() ) {
    lineNumber++;
    break;
  }
  switch (c) {
  case '\n':
    lineNumber++;
    // FALLTHROUGH
  case ' ':
  case '\t':
  case '\f':
    c = ' ';
    if ( line.empty() || *(line.rbegin()) == ' ')
	continue; // condense multiple spaces
    // FALLTHROUGH
  default:
    line += c;
  }
}
done:
#line 388 "BLIF.nw"
  
#line 436 "BLIF.nw"
word.resize(0);
string::const_iterator i = line.begin();
string::const_iterator lastWordStart;
do {
  while (i != line.end() && *i == ' ') i++;
  lastWordStart = i;
  while (i != line.end() && *i != ' ') i++;
  if (i != lastWordStart) word.push_back(string(lastWordStart, i));
} while (i != line.end());
#line 389 "BLIF.nw"
}
#line 859 "BLIF.nw"
  
#line 459 "BLIF.nw"
void print_blif(std::ostream &o, const Netlist &n)
{
  o << ".model " << n.name << '\n';
  o << ".inputs";
  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_input ) o << ' ' << (*i)->name;
  o << '\n';

  o << ".outputs";
  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_output ) o << ' ' << (*i)->name;
  o << '\n';
 
  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ ) {
    Gate &g = *(*i);
    if ( !g.is_input && !g.is_latch) {
      o << ".names";
      for ( vector<Input*>::const_iterator j = g.inputs.begin() ;
	    j != g.inputs.end() ; j++ )
	o << ' ' << (*j)->driver->name;
      o << ' ' << g.name << '\n';
      for ( vector<Input*>::const_iterator j = g.inputs.begin() ;
	    j != g.inputs.end() ; j++ )
	o << ((*j)->is_inverting ? '0' : '1');
      o << ' ';
      o << (g.is_inverting ? '0' : '1') << '\n';
    } else if (g.is_latch) {
      assert(g.inputs.size() == 1);
      o << ".latch " << g.inputs.front()->driver->name << ' ' << g.name << ' '
	<< (g.is_inverting ? '1' : '0') << '\n';
    }
  }
  o << ".end" << std::endl;
}
#line 860 "BLIF.nw"
  
#line 687 "BLIF.nw"
bool Simulator::debug = false;
#line 697 "BLIF.nw"
Simulator::Simulator(Netlist &nl) : n(nl)
{
  unsigned int ngates = n.gates.size();
  topoorder = new Gate*[ngates];
  currentState = new bool[ngates];
  nextState = new bool[ngates];

  // Populate the inputs, outputs, and latches vectors
  for (vector<Gate*>::const_iterator i = n.gates.begin() ;
       i != n.gates.end() ; i++) {
    if ((*i)->is_input) inputs.push_back(*i);
    if ((*i)->is_output) outputs.push_back(*i);
    if ((*i)->is_latch) latches.push_back(*i);
  }

  // Topologically sort the gates
  next = 0;
  set<Gate*> visited;
  for (vector<Gate*>::const_iterator i = n.gates.begin() ;
       i != n.gates.end() ; i++ ) dfs(*i, visited);
  assert(next == n.gates.size());

  reset();
}
#line 726 "BLIF.nw"
Simulator::~Simulator()
{
  delete [] nextState;
  delete [] currentState;
  delete [] topoorder;
}
#line 738 "BLIF.nw"
void Simulator::dfs(Gate *g, set<Gate*> &visited)
{
  if (visited.find(g) != visited.end()) return;
  visited.insert(g);
  for ( vector<Input*>::const_iterator i = g->inputs.begin() ;
	i != g->inputs.end() ; i++ )
    if (!(*i)->driver->is_latch) dfs((*i)->driver, visited);
  topoorder[next++] = g;
}
#line 755 "BLIF.nw"
void Simulator::reset()
{
  for ( vector<Gate*>::const_iterator i = latches.begin() ;
	i != latches.end() ; i++ )
    nextState[(*i)->id] = (*i)->is_inverting;
  for ( vector<Gate*>::const_iterator i = inputs.begin() ;
	i != inputs.end() ; i++ ) currentState[(*i)->id] = false;
}
#line 772 "BLIF.nw"
void Simulator::simulate()
{
  // The clock: copy the Ds of the latches to the Qs

  for (vector<Gate*>::const_iterator i = latches.begin() ;
       i != latches.end() ; i++ )
    currentState[(*i)->id] = nextState[(*i)->id];

  // Propagate the new current state information throughout the circuit

  for (unsigned int i = 0 ; i < n.gates.size() ; i++) {
    Gate *g = topoorder[i];
    if ( !(g->is_input) ) {
      bool state = true;
      for (vector<Input*>::const_iterator j = g->inputs.begin() ;
	   j != g->inputs.end() ; j++)
	state =
	 state && ( currentState[(*j)->driver->id] != ((*j)->is_inverting) );
      if ( g->is_latch ) {
	// Write the output of the latch to the next state array
	nextState[g->id] = state;
	if (debug)
	  std::cout << "latch " << g->name << '=' 
                    << ( nextState[g->id] ? '1' : '0') << '\n';
      } else {
	// Write the output of a normal gate
	currentState[g->id] = (state != g->is_inverting);
	if (debug)
	  std::cout << g->name << '=' 
                    << ( currentState[g->id] ? '1' : '0') << '\n';
      }
    }
  }
}
#line 861 "BLIF.nw"
  
#line 569 "BLIF.nw"
string &renw(string &s) // rename wire
{
  int lc = s.length() - 1;
  if(s[0]=='[' && s[lc]==']'){
    s[0]=s[lc]='_';
  }
  return s;
}

void print_verilog(std::ostream &o, const Netlist &n)
{
  o << "module " << n.name << "(clk, rst";

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_output ) o << ", " << (*i)->name;

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_input ) o << ", " << (*i)->name;

  o << ");\n";

  o << "input clk, rst;\n";

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_output ) o << "output " << (*i)->name << ";\n";

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    if ( (*i)->is_input ) o << "input " << (*i)->name << ";\n";

  o << '\n';

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ )
    o << "wire " << renw((*i)->name) << ";\n";

  o << '\n';

  unsigned int instnum = 0;

  for ( vector<Gate*>::const_iterator i = n.gates.begin() ;
        i != n.gates.end() ; i++ ) {
    if ((*i)->is_input) continue;
    else if ((*i)->is_latch) {
      assert((*i)->inputs.size() == 1);
      o << "d_ff" << ((*i)->is_inverting ? '1' : '0') << " u" << instnum++
	<< "(rst, clk, " << renw((*i)->name) << ", "
	<< renw((*i)->inputs.front()->driver->name) << ");\n";
    } else {
      // Normal gate: becomes an assignment
      o << "assign " << renw((*i)->name) << " = ";
      if ( (*i)->inputs.size() == 0 ) {
	// No inputs: a constant
	o << ((*i)->is_inverting ? '0' : '1');
      } else {
	// Some form of AND or OR gate 
	for ( vector<Input*>::const_iterator j = (*i)->inputs.begin() ;
	      j != (*i)->inputs.end() ; j++ ) {
	  if ( j != (*i)->inputs.begin() )
	    o << ' ' << ((*i)->is_inverting ? '|' : '&') << ' ';
	  if ( (*j)->is_inverting != (*i)->is_inverting ) o << '!';
	  o << renw((*j)->driver->name);
	}
      }

      o << ";\n";
    }
  }

  o << "\nendmodule\n";
}
#line 862 "BLIF.nw"
}
