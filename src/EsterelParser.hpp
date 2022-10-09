#ifndef INC_EsterelParser_hpp_
#define INC_EsterelParser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "esterel.g" -> "EsterelParser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "EsterelTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 1 "esterel.g"

#include "LineAST.hpp"

#line 16 "EsterelParser.hpp"
/**
 *
 * Esterel Grammar for ANTLR based on the grammar in the Esterel Primer, v91
 * Also includes deprecated syntax: see commentary below.
 *
 * Author: Stephen A. Edwards
 *         sedwards@cs.columbia.edu
 *
 * This generates a generic AST using ANTLR's built-in facilities for AST
 * synthesis.
 *
 * Change Log
 */
class CUSTOM_API EsterelParser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public EsterelTokenTypes
{
#line 1 "esterel.g"
#line 33 "EsterelParser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	EsterelParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return EsterelParser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return EsterelParser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return EsterelParser::tokenNames;
	}
	public: void file();
	public: void module();
	public: void moduleIdentifier();
	public: void declarations();
	public: void statement();
	public: void interfaceDecls();
	public: void typeDecls();
	public: void constantDecls();
	public: void functionDecls();
	public: void procedureDecls();
	public: void taskDecls();
	public: void interfacesignalDecls();
	public: void sensorDecls();
	public: void relationDecls();
	public: void typeIdentifier();
	public: void constantDecl();
	public: void constantIdentifier();
	public: void constantInitializer();
	public: void identifierList();
	public: void constantAtom();
	public: void functionDecl();
	public: void functionIdentifier();
	public: void optTypeIdentifierList();
	public: void procedureDecl();
	public: void procedureIdentifier();
	public: void taskDecl();
	public: void taskIdentifier();
	public: void signalDecl();
	public: void signalDeclList();
	public: void signalIdentifier();
	public: void signalInitializer();
	public: void channelType();
	public: void expression();
	public: void predefinedCombineFunction();
	public: void sensorDecl();
	public: void sensorIdentifier();
	public: void relationDecl();
	public: void implicationDecl();
	public: void exclusionDecl();
	public: void orexpr();
	public: void andexpr();
	public: void notexpr();
	public: void cmpexpr();
	public: void addexpr();
	public: void mulexpr();
	public: void unaryexpr();
	public: void trapIdentifier();
	public: void functionCall();
	public: void constant();
	public: void constantLiteral();
	public: void unsignedNumber();
	public: void stringConstant();
	public: void signedNumber();
	public: void signalExpression();
	public: void sandexpr();
	public: void sunaryexpr();
	public: void delayExpression();
	public: void bracketedSignalExpression();
	public: void delayPair();
	public: void sequence();
	public: void atomicStatement();
	public: void emit();
	public: void sustain();
	public: void assignment();
	public: void procedureCall();
	public: void present();
	public: void ifstatement();
	public: void loop();
	public: void repeat();
	public: void abort();
	public: void await();
	public: void every();
	public: void suspend();
	public: void trap();
	public: void exit();
	public: void exec();
	public: void localvariableDecl();
	public: void localSignalDecl();
	public: void runModule();
	public: void doStatements();
	public: void variableIdentifier();
	public: void refArgs();
	public: void valueArgs();
	public: void presentThenPart();
	public: void presentCase();
	public: void elsePart();
	public: void presentEvent();
	public: void thenPart();
	public: void elsif();
	public: void abortOneCaseStrong();
	public: void acase();
	public: void abortOneCaseWeak();
	public: void awaitOneCase();
	public: void trapDeclList();
	public: void trapHandler();
	public: void trapDecl();
	public: void trapInitializer();
	public: void trapEvent();
	public: void eand();
	public: void eunary();
	public: void execOneCase();
	public: void execCase();
	public: void variableDeclList();
	public: void variableDecls();
	public: void variableDeclList2();
	public: void variableDecl();
	public: void variableInitializer();
	public: void renaming();
	public: void typeRenaming();
	public: void constantRenaming();
	public: void functionRenaming();
	public: void procedureRenaming();
	public: void taskRenaming();
	public: void signalRenaming();
	public: void predefinedFunction();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST);
	}
	
protected:
	RefLineAST returnAST;
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
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
	static const unsigned long _tokenSet_7_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_7;
};

#endif /*INC_EsterelParser_hpp_*/
