#include "IR.hpp"
#include "AST.hpp"
#include "EsterelLexer.hpp"
#include "EsterelParser.hpp"
#include "EsterelTreeParser.hpp"
#include "LineAST.hpp"
#include <iostream>
#include <stdlib.h>

int main()
{
  try {

    EsterelLexer lexer(std::cin);
    antlr::ASTFactory factory("LineAST", LineAST::factory);
    EsterelParser parser(lexer);
    parser.initializeASTFactory( factory );
    parser.setASTNodeFactory( &factory );
    parser.file();
    RefLineAST t = (RefLineAST) parser.getAST();

    EsterelTreeParser treeParser;

    AST::Modules *ms = new AST::Modules();
    treeParser.file(t, ms, "");

    IR::XMLostream w(std::cout);
    w << ms;

  } catch (IR::Error &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (LineError &e) {
    std::cerr << e.s << std::endl;
    exit(-1);
  } catch (antlr::ANTLRException &e) {
    std::cerr << e.toString() << std::endl;
    exit(-1);
  } 
 
}
