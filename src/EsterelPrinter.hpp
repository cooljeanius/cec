#line 27 "EsterelPrinter.nw"
#ifndef _ESTEREL_PRINTER_HPP
#  define _ESTEREL_PRINTER_HPP

#  include "AST.hpp"
#  include <iostream>
#  include <vector>
#  include <map>

namespace AST {

  using std::vector;
  using std::map;

  class EsterelPrinter : public Visitor {
    std::ostream &o;
    bool v7code;
    unsigned int indentlevel;

    std::vector<int> precedence;
    std::map<string, int> level;
    std::map<string, int> unarylevel;

  public:
    EsterelPrinter(std::ostream &, bool);
    virtual ~EsterelPrinter() {}

    using Visitor::visit; // Bring the Visitor's visit method into scope

    void print(ASTNode* n) { assert(n); n->welcome(*this); }
    void statement(Statement *);
    void expression(Expression *);
    void sigexpression(Expression *);

    static const int sequentialPrecedence = 2;
    static const int parallelPrecedence = 1;

    bool push_precedence(int);
    void pop_precedence();

    Status visit(ModuleSymbol&);
    Status visit(VariableSymbol&);
    Status visit(BuiltinConstantSymbol&);
    Status visit(BuiltinSignalSymbol&);
    Status visit(BuiltinTypeSymbol&);
    Status visit(BuiltinFunctionSymbol&);
    Status visit(TypeRenaming &);
    Status visit(ConstantRenaming &);
    Status visit(FunctionRenaming &);
    Status visit(ProcedureRenaming &);
    Status visit(SignalRenaming &);
    Status visit(PredicatedStatement &);
    Status visit(TaskCall &);

    Status visit(Module&);
    Status visit(Exclusion&);
    Status visit(Implication&);
    Status visit(Modules&);
    Status visit(SymbolTable&);

    Status visit(TypeSymbol&);
    Status visit(ConstantSymbol&);
    Status visit(SignalSymbol&);
    Status visit(FunctionSymbol&);
    Status visit(ProcedureSymbol&);
    Status visit(TaskSymbol&);

    Status visit(StatementList&);
    Status visit(ParallelStatementList&);

    Status visit(Nothing&);
    Status visit(Pause&);
    Status visit(Halt&);
    Status visit(Emit&);
    Status visit(Sustain&);
    Status visit(Assign&);
    Status visit(StartCounter&);
    Status visit(ProcedureCall&);
    Status visit(Exec&);
    Status visit(Present&);
    Status visit(If&);
    Status visit(Loop&);
    Status visit(Repeat&);
    Status visit(Abort&);
    Status visit(Await&);
    Status visit(LoopEach&);
    Status visit(Every&);
    Status visit(Suspend&);
    Status visit(DoWatching&);
    Status visit(DoUpto&);
    Status visit(Trap&);
    Status visit(Exit&);
    Status visit(Var&);
    Status visit(Signal&);
    Status visit(Run&);

    Status visit(UnaryOp&);
    Status visit(BinaryOp&);
    Status visit(LoadVariableExpression&);
    Status visit(LoadSignalExpression&);
    Status visit(LoadSignalValueExpression&);
    Status visit(Literal&);
    Status visit(FunctionCall&);
    Status visit(Delay&);
    Status visit(CheckCounter&);

    Status visit(IfThenElse&);

    void indent() { indentlevel += 2; }
    void unindent() { indentlevel -= 2; }
    void tab() { for (unsigned int i = 0 ; i < indentlevel ; i++) o << ' '; }
  };
}

#endif
