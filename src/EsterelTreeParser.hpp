#ifndef INC_EsterelTreeParser_hpp_
#define INC_EsterelTreeParser_hpp_

#include <antlr/config.hpp>
#include "EsterelTreeParserTokenTypes.hpp"
/* $ANTLR 2.7.2: "staticsemantics.g" -> "EsterelTreeParser.hpp"$ */
#include <antlr/TreeParser.hpp>

#line 1 "staticsemantics.g"


#include <string>
#include <sstream>
using std::string;
#include "LineAST.hpp"
#include "AST.hpp"
using namespace AST;
#include <cassert>

  struct Context {
    Module *module;
    SymbolTable *variables;
    SymbolTable *traps;
    SymbolTable *signals;
    BuiltinTypeSymbol *boolean_type;
    BuiltinTypeSymbol *integer_type;
    BuiltinTypeSymbol *float_type;
    BuiltinTypeSymbol *double_type;
    BuiltinTypeSymbol *string_type;
    BuiltinConstantSymbol *true_constant;
    BuiltinConstantSymbol *false_constant;

    Context(Module *m) : module(m), variables(m->constants),
                         traps(0), signals(m->signals) {}
  };

  struct LineError {
    string s;
    LineError(RefLineAST a, string m) {
      std::ostringstream ost;
      ost << a->getLine() << ':' << m;
      s = ost.str();
    }
  };


#line 48 "EsterelTreeParser.hpp"
class EsterelTreeParser : public ANTLR_USE_NAMESPACE(antlr)TreeParser, public EsterelTreeParserTokenTypes
{
#line 125 "staticsemantics.g"

#line 52 "EsterelTreeParser.hpp"
public:
	EsterelTreeParser();
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
	int getNumTokens() const
	{
		return EsterelTreeParser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return EsterelTreeParser::tokenNames[type];
	}
	public: void file(RefLineAST _t,
		Modules *ms, string filename
	);
	public: void module(RefLineAST _t,
		Modules* modules
	);
	public: void declarations(RefLineAST _t,
		Context *c
	);
	public: Statement * statement(RefLineAST _t,
		Context *c
	);
	public: void typeDecl(RefLineAST _t,
		Context *c
	);
	public: void constantDecl(RefLineAST _t,
		Context *c
	);
	public: void signalDecl(RefLineAST _t,
		Context *c, string direction, bool isGlobal
	);
	public: void functionDecl(RefLineAST _t,
		Context *c
	);
	public: void procedureDecl(RefLineAST _t,
		Context *c
	);
	public: void taskDecl(RefLineAST _t,
		Context *c
	);
	public: void relationDecl(RefLineAST _t,
		Context *c
	);
	public: Expression * expression(RefLineAST _t,
		Context *c
	);
	public: TypeSymbol * type(RefLineAST _t,
		Context *c
	);
	public: void predefinedCombineFunction(RefLineAST _t);
	public: void trapDecl(RefLineAST _t,
		Context *c
	);
	public: void procArgs(RefLineAST _t,
		Context *c, ProcedureCall *pc
	);
	public: Expression * sigExpression(RefLineAST _t,
		Context *c
	);
	public: Expression * delayExpression(RefLineAST _t,
		Context *c
	);
	public: Expression * trapExpression(RefLineAST _t,
		Context *c
	);
	public: void funcName(RefLineAST _t);
	public: void variableDeclList(RefLineAST _t,
		 Context *oc, Context *nc, TypeSymbol *ts 
	);
public:
	RefLineAST getAST();
	
protected:
	RefLineAST returnAST;
	RefLineAST _retTree;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 125;
#else
	enum {
		NUM_TOKENS = 125
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
};

#endif /*INC_EsterelTreeParser_hpp_*/
