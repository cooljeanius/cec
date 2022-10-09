/* $ANTLR 2.7.7 (2006-11-01): "esterel.g" -> "EsterelParser.cpp"$ */
#include "EsterelParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 1 "esterel.g"
#line 8 "EsterelParser.cpp"
EsterelParser::EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

EsterelParser::EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,2)
{
}

EsterelParser::EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

EsterelParser::EsterelParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,2)
{
}

EsterelParser::EsterelParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,2)
{
}

void EsterelParser::file() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST file_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{ // ( ... )+
	int _cnt3=0;
	for (;;) {
		if ((LA(1) == LITERAL_module)) {
			module();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			if ( _cnt3>=1 ) { goto _loop3; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt3++;
	}
	_loop3:;
	}  // ( ... )+
	match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
	file_AST = RefLineAST(currentAST.root);
	returnAST = file_AST;
}

void EsterelParser::module() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST module_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp2_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp2_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp2_AST));
	}
	match(LITERAL_module);
	moduleIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(COLON);
	declarations();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_end:
	{
		match(LITERAL_end);
		match(LITERAL_module);
		break;
	}
	case PERIOD:
	{
		match(PERIOD);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	module_AST = RefLineAST(currentAST.root);
	returnAST = module_AST;
}

void EsterelParser::moduleIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST moduleIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp7_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp7_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
	}
	match(ID);
	moduleIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = moduleIdentifier_AST;
}

void EsterelParser::declarations() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST declarations_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			interfaceDecls();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop8;
		}
		
	}
	_loop8:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		declarations_AST = RefLineAST(currentAST.root);
#line 88 "esterel.g"
		declarations_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(DECLS,"decls")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(declarations_AST))));
#line 147 "EsterelParser.cpp"
		currentAST.root = declarations_AST;
		if ( declarations_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			declarations_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = declarations_AST->getFirstChild();
		else
			currentAST.child = declarations_AST;
		currentAST.advanceChildToEnd();
	}
	declarations_AST = RefLineAST(currentAST.root);
	returnAST = declarations_AST;
}

void EsterelParser::statement() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST statement_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	sequence();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == PARALLEL)) {
			match(PARALLEL);
			sequence();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop116;
		}
		
	}
	_loop116:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		statement_AST = RefLineAST(currentAST.root);
#line 366 "esterel.g"
		if (statement_AST && statement_AST->getNextSibling()) {
		statement_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PARALLEL,"||")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(statement_AST))));
		}
		
#line 192 "EsterelParser.cpp"
		currentAST.root = statement_AST;
		if ( statement_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			statement_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = statement_AST->getFirstChild();
		else
			currentAST.child = statement_AST;
		currentAST.advanceChildToEnd();
	}
	statement_AST = RefLineAST(currentAST.root);
	returnAST = statement_AST;
}

void EsterelParser::interfaceDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST interfaceDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_type:
	{
		typeDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_constant:
	{
		constantDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_function:
	{
		functionDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_procedure:
	{
		procedureDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_task:
	{
		taskDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_input:
	case LITERAL_output:
	case LITERAL_inputoutput:
	case LITERAL_return:
	{
		interfacesignalDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_sensor:
	{
		sensorDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_relation:
	{
		relationDecls();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		interfaceDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = interfaceDecls_AST;
}

void EsterelParser::typeDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST typeDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp9_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp9_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp9_AST));
	}
	match(LITERAL_type);
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			typeIdentifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop12;
		}
		
	}
	_loop12:;
	} // ( ... )*
	match(SEMICOLON);
	typeDecls_AST = RefLineAST(currentAST.root);
	returnAST = typeDecls_AST;
}

void EsterelParser::constantDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp12_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp12_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp12_AST));
	}
	match(LITERAL_constant);
	constantDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			constantDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop15;
		}
		
	}
	_loop15:;
	} // ( ... )*
	match(SEMICOLON);
	constantDecls_AST = RefLineAST(currentAST.root);
	returnAST = constantDecls_AST;
}

void EsterelParser::functionDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST functionDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp15_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp15_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp15_AST));
	}
	match(LITERAL_function);
	functionDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			functionDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop25;
		}
		
	}
	_loop25:;
	} // ( ... )*
	match(SEMICOLON);
	functionDecls_AST = RefLineAST(currentAST.root);
	returnAST = functionDecls_AST;
}

void EsterelParser::procedureDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST procedureDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp18_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp18_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp18_AST));
	}
	match(LITERAL_procedure);
	procedureDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			procedureDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop33;
		}
		
	}
	_loop33:;
	} // ( ... )*
	match(SEMICOLON);
	procedureDecls_AST = RefLineAST(currentAST.root);
	returnAST = procedureDecls_AST;
}

void EsterelParser::taskDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST taskDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp21_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp21_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp21_AST));
	}
	match(LITERAL_task);
	taskDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			taskDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop37;
		}
		
	}
	_loop37:;
	} // ( ... )*
	match(SEMICOLON);
	taskDecls_AST = RefLineAST(currentAST.root);
	returnAST = taskDecls_AST;
}

/**
    The grammar allows full expressions in the initializers
    but only constants are permitted in interface signals.
*/
void EsterelParser::interfacesignalDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST interfacesignalDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_input:
	{
		RefLineAST tmp24_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp24_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp24_AST));
		}
		match(LITERAL_input);
		signalDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				signalDecl();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop41;
			}
			
		}
		_loop41:;
		} // ( ... )*
		match(SEMICOLON);
		interfacesignalDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_output:
	{
		RefLineAST tmp27_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp27_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp27_AST));
		}
		match(LITERAL_output);
		signalDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				signalDecl();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop43;
			}
			
		}
		_loop43:;
		} // ( ... )*
		match(SEMICOLON);
		interfacesignalDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_inputoutput:
	{
		RefLineAST tmp30_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp30_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp30_AST));
		}
		match(LITERAL_inputoutput);
		signalDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				signalDecl();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop45;
			}
			
		}
		_loop45:;
		} // ( ... )*
		match(SEMICOLON);
		interfacesignalDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_return:
	{
		RefLineAST tmp33_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp33_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp33_AST));
		}
		match(LITERAL_return);
		signalDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				signalDecl();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop47;
			}
			
		}
		_loop47:;
		} // ( ... )*
		match(SEMICOLON);
		interfacesignalDecls_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = interfacesignalDecls_AST;
}

void EsterelParser::sensorDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sensorDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp36_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp36_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp36_AST));
	}
	match(LITERAL_sensor);
	sensorDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			sensorDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop60;
		}
		
	}
	_loop60:;
	} // ( ... )*
	match(SEMICOLON);
	sensorDecls_AST = RefLineAST(currentAST.root);
	returnAST = sensorDecls_AST;
}

void EsterelParser::relationDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST relationDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp39_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp39_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp39_AST));
	}
	match(LITERAL_relation);
	relationDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			relationDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop65;
		}
		
	}
	_loop65:;
	} // ( ... )*
	match(SEMICOLON);
	relationDecls_AST = RefLineAST(currentAST.root);
	returnAST = relationDecls_AST;
}

void EsterelParser::typeIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST typeIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp42_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp42_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp42_AST));
	}
	match(ID);
	typeIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = typeIdentifier_AST;
}

void EsterelParser::constantDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	if ((LA(1) == ID) && (LA(2) == COLON || LA(2) == EQUALS)) {
		constantIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case EQUALS:
		{
			constantInitializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case COLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
	}
	else if ((LA(1) == ID) && (LA(2) == COMMA)) {
		identifierList();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	match(COLON);
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		constantDecl_AST = RefLineAST(currentAST.root);
#line 118 "esterel.g"
		constantDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CDECL,"cdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(constantDecl_AST))));
#line 759 "EsterelParser.cpp"
		currentAST.root = constantDecl_AST;
		if ( constantDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			constantDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = constantDecl_AST->getFirstChild();
		else
			currentAST.child = constantDecl_AST;
		currentAST.advanceChildToEnd();
	}
	constantDecl_AST = RefLineAST(currentAST.root);
	returnAST = constantDecl_AST;
}

void EsterelParser::constantIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp44_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp44_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp44_AST));
	}
	match(ID);
	constantIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = constantIdentifier_AST;
}

void EsterelParser::constantInitializer() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantInitializer_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp45_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp45_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp45_AST));
	}
	match(EQUALS);
	constantAtom();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	constantInitializer_AST = RefLineAST(currentAST.root);
	returnAST = constantInitializer_AST;
}

void EsterelParser::identifierList() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST identifierList_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp46_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp46_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp46_AST));
	}
	match(ID);
	RefLineAST tmp47_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp47_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp47_AST));
	}
	match(COMMA);
	RefLineAST tmp48_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp48_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp48_AST));
	}
	match(ID);
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			RefLineAST tmp50_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp50_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp50_AST));
			}
			match(ID);
		}
		else {
			goto _loop22;
		}
		
	}
	_loop22:;
	} // ( ... )*
	identifierList_AST = RefLineAST(currentAST.root);
	returnAST = identifierList_AST;
}

void EsterelParser::constantAtom() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantAtom_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	case LITERAL_true:
	case LITERAL_false:
	case StringConstant:
	{
		constantLiteral();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constantAtom_AST = RefLineAST(currentAST.root);
		break;
	}
	case Integer:
	case FloatConst:
	case DoubleConst:
	case DASH:
	{
		signedNumber();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constantAtom_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = constantAtom_AST;
}

void EsterelParser::functionDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST functionDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	functionIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	optTypeIdentifierList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(COLON);
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		functionDecl_AST = RefLineAST(currentAST.root);
#line 137 "esterel.g"
		functionDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(FDECL,"fdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(functionDecl_AST))));
#line 911 "EsterelParser.cpp"
		currentAST.root = functionDecl_AST;
		if ( functionDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			functionDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = functionDecl_AST->getFirstChild();
		else
			currentAST.child = functionDecl_AST;
		currentAST.advanceChildToEnd();
	}
	functionDecl_AST = RefLineAST(currentAST.root);
	returnAST = functionDecl_AST;
}

void EsterelParser::functionIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST functionIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp52_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp52_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp52_AST));
	}
	match(ID);
	functionIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = functionIdentifier_AST;
}

void EsterelParser::optTypeIdentifierList() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST optTypeIdentifierList_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LPAREN);
	{
	switch ( LA(1)) {
	case ID:
	{
		typeIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				typeIdentifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop30;
			}
			
		}
		_loop30:;
		} // ( ... )*
		break;
	}
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(RPAREN);
	if ( inputState->guessing==0 ) {
		optTypeIdentifierList_AST = RefLineAST(currentAST.root);
#line 142 "esterel.g"
		optTypeIdentifierList_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TYPES,"types")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(optTypeIdentifierList_AST))));
#line 986 "EsterelParser.cpp"
		currentAST.root = optTypeIdentifierList_AST;
		if ( optTypeIdentifierList_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			optTypeIdentifierList_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = optTypeIdentifierList_AST->getFirstChild();
		else
			currentAST.child = optTypeIdentifierList_AST;
		currentAST.advanceChildToEnd();
	}
	optTypeIdentifierList_AST = RefLineAST(currentAST.root);
	returnAST = optTypeIdentifierList_AST;
}

void EsterelParser::procedureDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST procedureDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	procedureIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	optTypeIdentifierList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	optTypeIdentifierList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		procedureDecl_AST = RefLineAST(currentAST.root);
#line 153 "esterel.g"
		procedureDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PDECL,"pdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(procedureDecl_AST))));
#line 1020 "EsterelParser.cpp"
		currentAST.root = procedureDecl_AST;
		if ( procedureDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			procedureDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = procedureDecl_AST->getFirstChild();
		else
			currentAST.child = procedureDecl_AST;
		currentAST.advanceChildToEnd();
	}
	procedureDecl_AST = RefLineAST(currentAST.root);
	returnAST = procedureDecl_AST;
}

void EsterelParser::procedureIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST procedureIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp56_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp56_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp56_AST));
	}
	match(ID);
	procedureIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = procedureIdentifier_AST;
}

void EsterelParser::taskDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST taskDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	taskIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	optTypeIdentifierList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	optTypeIdentifierList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		taskDecl_AST = RefLineAST(currentAST.root);
#line 164 "esterel.g"
		taskDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TADECL,"tdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(taskDecl_AST))));
#line 1069 "EsterelParser.cpp"
		currentAST.root = taskDecl_AST;
		if ( taskDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			taskDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = taskDecl_AST->getFirstChild();
		else
			currentAST.child = taskDecl_AST;
		currentAST.advanceChildToEnd();
	}
	taskDecl_AST = RefLineAST(currentAST.root);
	returnAST = taskDecl_AST;
}

void EsterelParser::taskIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST taskIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp57_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp57_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp57_AST));
	}
	match(ID);
	taskIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = taskIdentifier_AST;
}

void EsterelParser::signalDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case COMMA:
	case SEMICOLON:
	case LITERAL_in:
	{
		break;
	}
	case COLON:
	case COLEQUALS:
	{
		{
		switch ( LA(1)) {
		case COLEQUALS:
		{
			signalInitializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case COLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(COLON);
		channelType();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LPAREN:
	{
		match(LPAREN);
		channelType();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		signalDecl_AST = RefLineAST(currentAST.root);
#line 192 "esterel.g"
		signalDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SDECL,"sdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(signalDecl_AST))));
#line 1164 "EsterelParser.cpp"
		currentAST.root = signalDecl_AST;
		if ( signalDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			signalDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = signalDecl_AST->getFirstChild();
		else
			currentAST.child = signalDecl_AST;
		currentAST.advanceChildToEnd();
	}
	signalDecl_AST = RefLineAST(currentAST.root);
	returnAST = signalDecl_AST;
}

void EsterelParser::signalDeclList() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalDeclList_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	signalDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			signalDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop50;
		}
		
	}
	_loop50:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		signalDeclList_AST = RefLineAST(currentAST.root);
#line 183 "esterel.g"
		signalDeclList_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SIGS,"sigs")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(signalDeclList_AST))));
#line 1206 "EsterelParser.cpp"
		currentAST.root = signalDeclList_AST;
		if ( signalDeclList_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			signalDeclList_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = signalDeclList_AST->getFirstChild();
		else
			currentAST.child = signalDeclList_AST;
		currentAST.advanceChildToEnd();
	}
	signalDeclList_AST = RefLineAST(currentAST.root);
	returnAST = signalDeclList_AST;
}

void EsterelParser::signalIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp62_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp62_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp62_AST));
	}
	match(ID);
	signalIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = signalIdentifier_AST;
}

void EsterelParser::signalInitializer() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalInitializer_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp63_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp63_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp63_AST));
	}
	match(COLEQUALS);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	signalInitializer_AST = RefLineAST(currentAST.root);
	returnAST = signalInitializer_AST;
}

void EsterelParser::channelType() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST channelType_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	{
		typeIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		channelType_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_combine:
	{
		match(LITERAL_combine);
		typeIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_with);
		{
		switch ( LA(1)) {
		case ID:
		{
			functionIdentifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case PLUS:
		case STAR:
		case LITERAL_or:
		case LITERAL_and:
		{
			predefinedCombineFunction();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		channelType_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = channelType_AST;
}

/***********************************************************************
 *
 * Expressions
 *
 ***********************************************************************/
void EsterelParser::expression() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST expression_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	orexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	expression_AST = RefLineAST(currentAST.root);
	returnAST = expression_AST;
}

void EsterelParser::predefinedCombineFunction() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST predefinedCombineFunction_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case PLUS:
	{
		RefLineAST tmp66_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp66_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp66_AST));
		}
		match(PLUS);
		predefinedCombineFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case STAR:
	{
		RefLineAST tmp67_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp67_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp67_AST));
		}
		match(STAR);
		predefinedCombineFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_or:
	{
		RefLineAST tmp68_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp68_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp68_AST));
		}
		match(LITERAL_or);
		predefinedCombineFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_and:
	{
		RefLineAST tmp69_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp69_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp69_AST));
		}
		match(LITERAL_and);
		predefinedCombineFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = predefinedCombineFunction_AST;
}

void EsterelParser::sensorDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sensorDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	sensorIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case COLON:
	{
		match(COLON);
		typeIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LPAREN:
	{
		match(LPAREN);
		typeIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		sensorDecl_AST = RefLineAST(currentAST.root);
#line 223 "esterel.g"
		sensorDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SDECL,"sdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(sensorDecl_AST))));
#line 1430 "EsterelParser.cpp"
		currentAST.root = sensorDecl_AST;
		if ( sensorDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			sensorDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = sensorDecl_AST->getFirstChild();
		else
			currentAST.child = sensorDecl_AST;
		currentAST.advanceChildToEnd();
	}
	sensorDecl_AST = RefLineAST(currentAST.root);
	returnAST = sensorDecl_AST;
}

void EsterelParser::sensorIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sensorIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp73_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp73_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp73_AST));
	}
	match(ID);
	sensorIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = sensorIdentifier_AST;
}

void EsterelParser::relationDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST relationDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	if ((LA(1) == ID) && (LA(2) == IMPLIES)) {
		implicationDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		relationDecl_AST = RefLineAST(currentAST.root);
	}
	else if ((LA(1) == ID) && (LA(2) == POUND)) {
		exclusionDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		relationDecl_AST = RefLineAST(currentAST.root);
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = relationDecl_AST;
}

void EsterelParser::implicationDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST implicationDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	RefLineAST tmp74_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp74_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp74_AST));
	}
	match(IMPLIES);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	implicationDecl_AST = RefLineAST(currentAST.root);
	returnAST = implicationDecl_AST;
}

void EsterelParser::exclusionDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST exclusionDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	RefLineAST tmp75_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp75_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp75_AST));
	}
	match(POUND);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == POUND)) {
			match(POUND);
			signalIdentifier();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop70;
		}
		
	}
	_loop70:;
	} // ( ... )*
	exclusionDecl_AST = RefLineAST(currentAST.root);
	returnAST = exclusionDecl_AST;
}

void EsterelParser::orexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST orexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	andexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_or)) {
			RefLineAST tmp77_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp77_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp77_AST));
			}
			match(LITERAL_or);
			andexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop74;
		}
		
	}
	_loop74:;
	} // ( ... )*
	orexpr_AST = RefLineAST(currentAST.root);
	returnAST = orexpr_AST;
}

void EsterelParser::andexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST andexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	notexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_and)) {
			RefLineAST tmp78_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp78_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp78_AST));
			}
			match(LITERAL_and);
			notexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop77;
		}
		
	}
	_loop77:;
	} // ( ... )*
	andexpr_AST = RefLineAST(currentAST.root);
	returnAST = andexpr_AST;
}

void EsterelParser::notexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST notexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_not:
	{
		RefLineAST tmp79_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp79_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp79_AST));
		}
		match(LITERAL_not);
		cmpexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		notexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case Integer:
	case FloatConst:
	case DoubleConst:
	case ID:
	case LPAREN:
	case DASH:
	case QUESTION:
	case LITERAL_pre:
	case DQUESTION:
	case LITERAL_true:
	case LITERAL_false:
	case StringConstant:
	{
		cmpexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		notexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = notexpr_AST;
}

void EsterelParser::cmpexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST cmpexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	addexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			{
			switch ( LA(1)) {
			case EQUALS:
			{
				RefLineAST tmp80_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp80_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp80_AST));
				}
				match(EQUALS);
				break;
			}
			case NEQUAL:
			{
				RefLineAST tmp81_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp81_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp81_AST));
				}
				match(NEQUAL);
				break;
			}
			case LESSTHAN:
			{
				RefLineAST tmp82_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp82_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp82_AST));
				}
				match(LESSTHAN);
				break;
			}
			case GREATERTHAN:
			{
				RefLineAST tmp83_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp83_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp83_AST));
				}
				match(GREATERTHAN);
				break;
			}
			case LEQUAL:
			{
				RefLineAST tmp84_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp84_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp84_AST));
				}
				match(LEQUAL);
				break;
			}
			case GEQUAL:
			{
				RefLineAST tmp85_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp85_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp85_AST));
				}
				match(GEQUAL);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			addexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop82;
		}
		
	}
	_loop82:;
	} // ( ... )*
	cmpexpr_AST = RefLineAST(currentAST.root);
	returnAST = cmpexpr_AST;
}

void EsterelParser::addexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST addexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	mulexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == PLUS || LA(1) == DASH)) {
			{
			switch ( LA(1)) {
			case PLUS:
			{
				RefLineAST tmp86_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp86_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp86_AST));
				}
				match(PLUS);
				break;
			}
			case DASH:
			{
				RefLineAST tmp87_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp87_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp87_AST));
				}
				match(DASH);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			mulexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop86;
		}
		
	}
	_loop86:;
	} // ( ... )*
	addexpr_AST = RefLineAST(currentAST.root);
	returnAST = addexpr_AST;
}

void EsterelParser::mulexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST mulexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	unaryexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == STAR || LA(1) == SLASH || LA(1) == LITERAL_mod)) {
			{
			switch ( LA(1)) {
			case STAR:
			{
				RefLineAST tmp88_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp88_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp88_AST));
				}
				match(STAR);
				break;
			}
			case SLASH:
			{
				RefLineAST tmp89_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp89_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp89_AST));
				}
				match(SLASH);
				break;
			}
			case LITERAL_mod:
			{
				RefLineAST tmp90_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp90_AST = astFactory->create(LT(1));
					astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp90_AST));
				}
				match(LITERAL_mod);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			unaryexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop90;
		}
		
	}
	_loop90:;
	} // ( ... )*
	mulexpr_AST = RefLineAST(currentAST.root);
	returnAST = mulexpr_AST;
}

void EsterelParser::unaryexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST unaryexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case DASH:
	{
		RefLineAST tmp91_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp91_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp91_AST));
		}
		match(DASH);
		unaryexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		unaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case LPAREN:
	{
		match(LPAREN);
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		unaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case QUESTION:
	{
		RefLineAST tmp94_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp94_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp94_AST));
		}
		match(QUESTION);
		signalIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		unaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_pre:
	{
		RefLineAST tmp95_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp95_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp95_AST));
		}
		match(LITERAL_pre);
		match(LPAREN);
		match(QUESTION);
		signalIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		unaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case DQUESTION:
	{
		RefLineAST tmp99_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp99_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp99_AST));
		}
		match(DQUESTION);
		trapIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		unaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
		if ((LA(1) == ID) && (LA(2) == LPAREN)) {
			functionCall();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			unaryexpr_AST = RefLineAST(currentAST.root);
		}
		else if ((_tokenSet_2.member(LA(1))) && (_tokenSet_3.member(LA(2)))) {
			constant();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			unaryexpr_AST = RefLineAST(currentAST.root);
		}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = unaryexpr_AST;
}

void EsterelParser::trapIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp100_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp100_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp100_AST));
	}
	match(ID);
	trapIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = trapIdentifier_AST;
}

void EsterelParser::functionCall() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST functionCall_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	functionIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LPAREN);
	{
	switch ( LA(1)) {
	case Integer:
	case FloatConst:
	case DoubleConst:
	case ID:
	case LPAREN:
	case LITERAL_not:
	case DASH:
	case QUESTION:
	case LITERAL_pre:
	case DQUESTION:
	case LITERAL_true:
	case LITERAL_false:
	case StringConstant:
	{
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop100;
			}
			
		}
		_loop100:;
		} // ( ... )*
		break;
	}
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(RPAREN);
	if ( inputState->guessing==0 ) {
		functionCall_AST = RefLineAST(currentAST.root);
#line 322 "esterel.g"
		functionCall_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CALL,"call")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(functionCall_AST))));
#line 2061 "EsterelParser.cpp"
		currentAST.root = functionCall_AST;
		if ( functionCall_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			functionCall_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = functionCall_AST->getFirstChild();
		else
			currentAST.child = functionCall_AST;
		currentAST.advanceChildToEnd();
	}
	functionCall_AST = RefLineAST(currentAST.root);
	returnAST = functionCall_AST;
}

void EsterelParser::constant() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constant_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	case LITERAL_true:
	case LITERAL_false:
	case StringConstant:
	{
		constantLiteral();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constant_AST = RefLineAST(currentAST.root);
		break;
	}
	case Integer:
	case FloatConst:
	case DoubleConst:
	{
		unsignedNumber();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constant_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = constant_AST;
}

void EsterelParser::constantLiteral() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantLiteral_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	{
		constantIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constantLiteral_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_true:
	{
		RefLineAST tmp104_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp104_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp104_AST));
		}
		match(LITERAL_true);
		constantLiteral_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_false:
	{
		RefLineAST tmp105_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp105_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp105_AST));
		}
		match(LITERAL_false);
		constantLiteral_AST = RefLineAST(currentAST.root);
		break;
	}
	case StringConstant:
	{
		stringConstant();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		constantLiteral_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = constantLiteral_AST;
}

void EsterelParser::unsignedNumber() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST unsignedNumber_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case Integer:
	{
		RefLineAST tmp106_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp106_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp106_AST));
		}
		match(Integer);
		unsignedNumber_AST = RefLineAST(currentAST.root);
		break;
	}
	case FloatConst:
	{
		RefLineAST tmp107_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp107_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp107_AST));
		}
		match(FloatConst);
		unsignedNumber_AST = RefLineAST(currentAST.root);
		break;
	}
	case DoubleConst:
	{
		RefLineAST tmp108_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp108_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp108_AST));
		}
		match(DoubleConst);
		unsignedNumber_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = unsignedNumber_AST;
}

void EsterelParser::stringConstant() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST stringConstant_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp109_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp109_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp109_AST));
	}
	match(StringConstant);
	stringConstant_AST = RefLineAST(currentAST.root);
	returnAST = stringConstant_AST;
}

void EsterelParser::signedNumber() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signedNumber_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case Integer:
	case FloatConst:
	case DoubleConst:
	{
		unsignedNumber();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		signedNumber_AST = RefLineAST(currentAST.root);
		break;
	}
	case DASH:
	{
		RefLineAST tmp110_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp110_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp110_AST));
		}
		match(DASH);
		unsignedNumber();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		signedNumber_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = signedNumber_AST;
}

void EsterelParser::signalExpression() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalExpression_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	sandexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_or)) {
			RefLineAST tmp111_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp111_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp111_AST));
			}
			match(LITERAL_or);
			sandexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop103;
		}
		
	}
	_loop103:;
	} // ( ... )*
	signalExpression_AST = RefLineAST(currentAST.root);
	returnAST = signalExpression_AST;
}

void EsterelParser::sandexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sandexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	sunaryexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_and)) {
			RefLineAST tmp112_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp112_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp112_AST));
			}
			match(LITERAL_and);
			sunaryexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop106;
		}
		
	}
	_loop106:;
	} // ( ... )*
	sandexpr_AST = RefLineAST(currentAST.root);
	returnAST = sandexpr_AST;
}

void EsterelParser::sunaryexpr() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sunaryexpr_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	{
		signalIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		sunaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_pre:
	{
		RefLineAST tmp113_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp113_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp113_AST));
		}
		match(LITERAL_pre);
		match(LPAREN);
		signalIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		sunaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_not:
	{
		RefLineAST tmp116_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp116_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp116_AST));
		}
		match(LITERAL_not);
		sunaryexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		sunaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	case LPAREN:
	{
		match(LPAREN);
		signalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		sunaryexpr_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = sunaryexpr_AST;
}

void EsterelParser::delayExpression() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST delayExpression_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	bool synPredMatched111 = false;
	if (((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2))))) {
		int _m111 = mark();
		synPredMatched111 = true;
		inputState->guessing++;
		try {
			{
			expression();
			bracketedSignalExpression();
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched111 = false;
		}
		rewind(_m111);
		inputState->guessing--;
	}
	if ( synPredMatched111 ) {
		delayPair();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
	}
	else if ((LA(1) == ID || LA(1) == LBRACKET) && (_tokenSet_6.member(LA(2)))) {
		bracketedSignalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
	}
	else if ((LA(1) == LITERAL_immediate)) {
		RefLineAST tmp119_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp119_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp119_AST));
		}
		match(LITERAL_immediate);
		bracketedSignalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	delayExpression_AST = RefLineAST(currentAST.root);
	returnAST = delayExpression_AST;
}

void EsterelParser::bracketedSignalExpression() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST bracketedSignalExpression_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	{
		signalIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		bracketedSignalExpression_AST = RefLineAST(currentAST.root);
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
		signalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RBRACKET);
		bracketedSignalExpression_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = bracketedSignalExpression_AST;
}

void EsterelParser::delayPair() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST delayPair_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	bracketedSignalExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		delayPair_AST = RefLineAST(currentAST.root);
#line 353 "esterel.g"
		delayPair_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(DELAY,"delay")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(delayPair_AST))));
#line 2508 "EsterelParser.cpp"
		currentAST.root = delayPair_AST;
		if ( delayPair_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			delayPair_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = delayPair_AST->getFirstChild();
		else
			currentAST.child = delayPair_AST;
		currentAST.advanceChildToEnd();
	}
	delayPair_AST = RefLineAST(currentAST.root);
	returnAST = delayPair_AST;
}

void EsterelParser::sequence() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sequence_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	atomicStatement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == SEMICOLON) && (_tokenSet_7.member(LA(2)))) {
			match(SEMICOLON);
			atomicStatement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop119;
		}
		
	}
	_loop119:;
	} // ( ... )*
	{
	switch ( LA(1)) {
	case SEMICOLON:
	{
		match(SEMICOLON);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		sequence_AST = RefLineAST(currentAST.root);
#line 377 "esterel.g"
		if (sequence_AST && sequence_AST->getNextSibling()) {
		sequence_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SEQUENCE,";")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(sequence_AST))));
		}
		
#line 2581 "EsterelParser.cpp"
		currentAST.root = sequence_AST;
		if ( sequence_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			sequence_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = sequence_AST->getFirstChild();
		else
			currentAST.child = sequence_AST;
		currentAST.advanceChildToEnd();
	}
	sequence_AST = RefLineAST(currentAST.root);
	returnAST = sequence_AST;
}

void EsterelParser::atomicStatement() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST atomicStatement_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_nothing:
	{
		RefLineAST tmp124_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp124_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp124_AST));
		}
		match(LITERAL_nothing);
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_pause:
	{
		RefLineAST tmp125_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp125_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp125_AST));
		}
		match(LITERAL_pause);
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_halt:
	{
		RefLineAST tmp126_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp126_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp126_AST));
		}
		match(LITERAL_halt);
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_emit:
	{
		emit();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_sustain:
	{
		sustain();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case ID:
	{
		assignment();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_call:
	{
		procedureCall();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_present:
	{
		present();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_if:
	{
		ifstatement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_loop:
	{
		loop();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_positive:
	case LITERAL_repeat:
	{
		repeat();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_abort:
	case LITERAL_weak:
	{
		abort();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_await:
	{
		await();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_every:
	{
		every();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_suspend:
	{
		suspend();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_trap:
	{
		trap();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_exit:
	{
		exit();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_exec:
	{
		exec();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_var:
	{
		localvariableDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_signal:
	{
		localSignalDecl();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_run:
	case LITERAL_copymodule:
	{
		runModule();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RBRACKET);
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_do:
	{
		doStatements();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		atomicStatement_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = atomicStatement_AST;
}

void EsterelParser::emit() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST emit_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp129_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp129_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp129_AST));
	}
	match(LITERAL_emit);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LPAREN:
	{
		match(LPAREN);
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	emit_AST = RefLineAST(currentAST.root);
	returnAST = emit_AST;
}

void EsterelParser::sustain() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST sustain_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp132_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp132_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp132_AST));
	}
	match(LITERAL_sustain);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LPAREN:
	{
		match(LPAREN);
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	sustain_AST = RefLineAST(currentAST.root);
	returnAST = sustain_AST;
}

void EsterelParser::assignment() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST assignment_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	variableIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	RefLineAST tmp135_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp135_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp135_AST));
	}
	match(COLEQUALS);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	assignment_AST = RefLineAST(currentAST.root);
	returnAST = assignment_AST;
}

void EsterelParser::procedureCall() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST procedureCall_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp136_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp136_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp136_AST));
	}
	match(LITERAL_call);
	procedureIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	refArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	valueArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	procedureCall_AST = RefLineAST(currentAST.root);
	returnAST = procedureCall_AST;
}

void EsterelParser::present() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST present_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp137_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp137_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp137_AST));
	}
	match(LITERAL_present);
	{
	switch ( LA(1)) {
	case ID:
	case LPAREN:
	case LITERAL_not:
	case LITERAL_pre:
	case LBRACKET:
	{
		presentThenPart();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_case:
	{
		{ // ( ... )+
		int _cnt139=0;
		for (;;) {
			if ((LA(1) == LITERAL_case)) {
				presentCase();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				if ( _cnt139>=1 ) { goto _loop139; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt139++;
		}
		_loop139:;
		}  // ( ... )+
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case LITERAL_else:
	{
		elsePart();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_present:
	{
		match(LITERAL_present);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	present_AST = RefLineAST(currentAST.root);
	returnAST = present_AST;
}

void EsterelParser::ifstatement() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST ifstatement_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp140_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp140_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp140_AST));
	}
	match(LITERAL_if);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_then:
	{
		thenPart();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case LITERAL_else:
	case LITERAL_elsif:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_elsif)) {
			elsif();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop152;
		}
		
	}
	_loop152:;
	} // ( ... )*
	{
	switch ( LA(1)) {
	case LITERAL_else:
	{
		elsePart();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_if:
	{
		match(LITERAL_if);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ifstatement_AST = RefLineAST(currentAST.root);
	returnAST = ifstatement_AST;
}

void EsterelParser::loop() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST loop_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp143_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp143_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp143_AST));
	}
	match(LITERAL_loop);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_end:
	{
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_loop:
		{
			match(LITERAL_loop);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case LITERAL_each:
	{
		match(LITERAL_each);
		delayExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	loop_AST = RefLineAST(currentAST.root);
	returnAST = loop_AST;
}

void EsterelParser::repeat() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST repeat_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	switch ( LA(1)) {
	case LITERAL_positive:
	{
		RefLineAST tmp147_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp147_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp147_AST));
		}
		match(LITERAL_positive);
		break;
	}
	case LITERAL_repeat:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	RefLineAST tmp148_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp148_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp148_AST));
	}
	match(LITERAL_repeat);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_times);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_repeat:
	{
		match(LITERAL_repeat);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	repeat_AST = RefLineAST(currentAST.root);
	returnAST = repeat_AST;
}

void EsterelParser::abort() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST abort_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_abort:
	{
		RefLineAST tmp152_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp152_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp152_AST));
		}
		match(LITERAL_abort);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_when);
		{
		switch ( LA(1)) {
		case Integer:
		case FloatConst:
		case DoubleConst:
		case ID:
		case LPAREN:
		case LITERAL_not:
		case DASH:
		case QUESTION:
		case LITERAL_pre:
		case DQUESTION:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL_immediate:
		case LBRACKET:
		case StringConstant:
		{
			abortOneCaseStrong();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_case:
		{
			{ // ( ... )+
			int _cnt163=0;
			for (;;) {
				if ((LA(1) == LITERAL_case)) {
					acase();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					if ( _cnt163>=1 ) { goto _loop163; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt163++;
			}
			_loop163:;
			}  // ( ... )+
			RefLineAST tmp154_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp154_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp154_AST));
			}
			match(LITERAL_end);
			{
			switch ( LA(1)) {
			case LITERAL_abort:
			{
				RefLineAST tmp155_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp155_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp155_AST));
				}
				match(LITERAL_abort);
				break;
			}
			case LITERAL_end:
			case PERIOD:
			case SEMICOLON:
			case RBRACKET:
			case PARALLEL:
			case LITERAL_else:
			case LITERAL_case:
			case LITERAL_elsif:
			case LITERAL_when:
			case LITERAL_each:
			case LITERAL_handle:
			case LITERAL_watching:
			case LITERAL_upto:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		abort_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_weak:
	{
		RefLineAST tmp156_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp156_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp156_AST));
		}
		match(LITERAL_weak);
		RefLineAST tmp157_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp157_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp157_AST));
		}
		match(LITERAL_abort);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_when);
		{
		switch ( LA(1)) {
		case Integer:
		case FloatConst:
		case DoubleConst:
		case ID:
		case LPAREN:
		case LITERAL_not:
		case DASH:
		case QUESTION:
		case LITERAL_pre:
		case DQUESTION:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL_immediate:
		case LBRACKET:
		case StringConstant:
		{
			abortOneCaseWeak();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case LITERAL_case:
		{
			{ // ( ... )+
			int _cnt167=0;
			for (;;) {
				if ((LA(1) == LITERAL_case)) {
					acase();
					if (inputState->guessing==0) {
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
					}
				}
				else {
					if ( _cnt167>=1 ) { goto _loop167; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt167++;
			}
			_loop167:;
			}  // ( ... )+
			RefLineAST tmp159_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp159_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp159_AST));
			}
			match(LITERAL_end);
			{
			switch ( LA(1)) {
			case LITERAL_abort:
			case LITERAL_weak:
			{
				{
				switch ( LA(1)) {
				case LITERAL_weak:
				{
					RefLineAST tmp160_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
					if ( inputState->guessing == 0 ) {
						tmp160_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp160_AST));
					}
					match(LITERAL_weak);
					break;
				}
				case LITERAL_abort:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				RefLineAST tmp161_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
				if ( inputState->guessing == 0 ) {
					tmp161_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp161_AST));
				}
				match(LITERAL_abort);
				break;
			}
			case LITERAL_end:
			case PERIOD:
			case SEMICOLON:
			case RBRACKET:
			case PARALLEL:
			case LITERAL_else:
			case LITERAL_case:
			case LITERAL_elsif:
			case LITERAL_when:
			case LITERAL_each:
			case LITERAL_handle:
			case LITERAL_watching:
			case LITERAL_upto:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		abort_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = abort_AST;
}

void EsterelParser::await() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST await_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp162_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp162_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp162_AST));
	}
	match(LITERAL_await);
	{
	switch ( LA(1)) {
	case Integer:
	case FloatConst:
	case DoubleConst:
	case ID:
	case LPAREN:
	case LITERAL_not:
	case DASH:
	case QUESTION:
	case LITERAL_pre:
	case DQUESTION:
	case LITERAL_true:
	case LITERAL_false:
	case LITERAL_immediate:
	case LBRACKET:
	case StringConstant:
	{
		awaitOneCase();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_case:
	{
		{ // ( ... )+
		int _cnt182=0;
		for (;;) {
			if ((LA(1) == LITERAL_case)) {
				acase();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				if ( _cnt182>=1 ) { goto _loop182; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt182++;
		}
		_loop182:;
		}  // ( ... )+
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_await:
		{
			match(LITERAL_await);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	await_AST = RefLineAST(currentAST.root);
	returnAST = await_AST;
}

void EsterelParser::every() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST every_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp165_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp165_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp165_AST));
	}
	match(LITERAL_every);
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_do);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_every:
	{
		match(LITERAL_every);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	every_AST = RefLineAST(currentAST.root);
	returnAST = every_AST;
}

void EsterelParser::suspend() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST suspend_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp169_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp169_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp169_AST));
	}
	match(LITERAL_suspend);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_when);
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	suspend_AST = RefLineAST(currentAST.root);
	returnAST = suspend_AST;
}

void EsterelParser::trap() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trap_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp171_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp171_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp171_AST));
	}
	match(LITERAL_trap);
	trapDeclList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_in);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_handle)) {
			trapHandler();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop195;
		}
		
	}
	_loop195:;
	} // ( ... )*
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_trap:
	{
		match(LITERAL_trap);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	trap_AST = RefLineAST(currentAST.root);
	returnAST = trap_AST;
}

void EsterelParser::exit() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST exit_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp175_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp175_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp175_AST));
	}
	match(LITERAL_exit);
	trapIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LPAREN:
	{
		match(LPAREN);
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	exit_AST = RefLineAST(currentAST.root);
	returnAST = exit_AST;
}

void EsterelParser::exec() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST exec_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	if ((LA(1) == LITERAL_exec) && (LA(2) == ID)) {
		RefLineAST tmp178_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp178_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp178_AST));
		}
		match(LITERAL_exec);
		execOneCase();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		exec_AST = RefLineAST(currentAST.root);
	}
	else if ((LA(1) == LITERAL_exec) && (LA(2) == LITERAL_case)) {
		RefLineAST tmp179_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp179_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp179_AST));
		}
		match(LITERAL_exec);
		{ // ( ... )+
		int _cnt216=0;
		for (;;) {
			if ((LA(1) == LITERAL_case)) {
				execCase();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				if ( _cnt216>=1 ) { goto _loop216; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt216++;
		}
		_loop216:;
		}  // ( ... )+
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_exec:
		{
			match(LITERAL_exec);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		exec_AST = RefLineAST(currentAST.root);
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = exec_AST;
}

void EsterelParser::localvariableDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST localvariableDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp182_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp182_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp182_AST));
	}
	match(LITERAL_var);
	variableDeclList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_in);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_var:
	{
		match(LITERAL_var);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	localvariableDecl_AST = RefLineAST(currentAST.root);
	returnAST = localvariableDecl_AST;
}

void EsterelParser::localSignalDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST localSignalDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp186_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp186_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp186_AST));
	}
	match(LITERAL_signal);
	signalDeclList();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_in);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_end);
	{
	switch ( LA(1)) {
	case LITERAL_signal:
	{
		match(LITERAL_signal);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	localSignalDecl_AST = RefLineAST(currentAST.root);
	returnAST = localSignalDecl_AST;
}

void EsterelParser::runModule() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST runModule_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	switch ( LA(1)) {
	case LITERAL_run:
	{
		match(LITERAL_run);
		break;
	}
	case LITERAL_copymodule:
	{
		match(LITERAL_copymodule);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	moduleIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case SLASH:
	{
		match(SLASH);
		moduleIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case LBRACKET:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case LBRACKET:
	{
		match(LBRACKET);
		renaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == SEMICOLON)) {
				match(SEMICOLON);
				renaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop243;
			}
			
		}
		_loop243:;
		} // ( ... )*
		match(RBRACKET);
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		runModule_AST = RefLineAST(currentAST.root);
#line 664 "esterel.g"
		runModule_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(RUN,"run")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(runModule_AST))));
#line 4197 "EsterelParser.cpp"
		currentAST.root = runModule_AST;
		if ( runModule_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			runModule_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = runModule_AST->getFirstChild();
		else
			currentAST.child = runModule_AST;
		currentAST.advanceChildToEnd();
	}
	runModule_AST = RefLineAST(currentAST.root);
	returnAST = runModule_AST;
}

void EsterelParser::doStatements() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST doStatements_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LITERAL_do);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_watching:
	{
		match(LITERAL_watching);
		delayExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{
		switch ( LA(1)) {
		case LITERAL_timeout:
		{
			match(LITERAL_timeout);
			statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			match(LITERAL_end);
			{
			switch ( LA(1)) {
			case LITERAL_timeout:
			{
				match(LITERAL_timeout);
				break;
			}
			case LITERAL_end:
			case PERIOD:
			case SEMICOLON:
			case RBRACKET:
			case PARALLEL:
			case LITERAL_else:
			case LITERAL_case:
			case LITERAL_elsif:
			case LITERAL_when:
			case LITERAL_each:
			case LITERAL_handle:
			case LITERAL_watching:
			case LITERAL_upto:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			doStatements_AST = RefLineAST(currentAST.root);
#line 731 "esterel.g"
			doStatements_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(DOWATCHING,"dowatching")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(doStatements_AST))));
#line 4296 "EsterelParser.cpp"
			currentAST.root = doStatements_AST;
			if ( doStatements_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				doStatements_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = doStatements_AST->getFirstChild();
			else
				currentAST.child = doStatements_AST;
			currentAST.advanceChildToEnd();
		}
		break;
	}
	case LITERAL_upto:
	{
		match(LITERAL_upto);
		delayExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		if ( inputState->guessing==0 ) {
			doStatements_AST = RefLineAST(currentAST.root);
#line 733 "esterel.g"
			doStatements_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(DOUPTO,"doupto")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(doStatements_AST))));
#line 4318 "EsterelParser.cpp"
			currentAST.root = doStatements_AST;
			if ( doStatements_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
				doStatements_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				  currentAST.child = doStatements_AST->getFirstChild();
			else
				currentAST.child = doStatements_AST;
			currentAST.advanceChildToEnd();
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	doStatements_AST = RefLineAST(currentAST.root);
	returnAST = doStatements_AST;
}

void EsterelParser::variableIdentifier() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableIdentifier_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp202_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp202_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp202_AST));
	}
	match(ID);
	variableIdentifier_AST = RefLineAST(currentAST.root);
	returnAST = variableIdentifier_AST;
}

void EsterelParser::refArgs() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST refArgs_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LPAREN);
	{
	switch ( LA(1)) {
	case ID:
	{
		variableIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				variableIdentifier();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop131;
			}
			
		}
		_loop131:;
		} // ( ... )*
		break;
	}
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(RPAREN);
	if ( inputState->guessing==0 ) {
		refArgs_AST = RefLineAST(currentAST.root);
#line 432 "esterel.g"
		refArgs_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(VARS,"vars")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(refArgs_AST))));
#line 4401 "EsterelParser.cpp"
		currentAST.root = refArgs_AST;
		if ( refArgs_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			refArgs_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = refArgs_AST->getFirstChild();
		else
			currentAST.child = refArgs_AST;
		currentAST.advanceChildToEnd();
	}
	refArgs_AST = RefLineAST(currentAST.root);
	returnAST = refArgs_AST;
}

void EsterelParser::valueArgs() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST valueArgs_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LPAREN);
	{
	switch ( LA(1)) {
	case Integer:
	case FloatConst:
	case DoubleConst:
	case ID:
	case LPAREN:
	case LITERAL_not:
	case DASH:
	case QUESTION:
	case LITERAL_pre:
	case DQUESTION:
	case LITERAL_true:
	case LITERAL_false:
	case StringConstant:
	{
		expression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				expression();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop135;
			}
			
		}
		_loop135:;
		} // ( ... )*
		break;
	}
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(RPAREN);
	if ( inputState->guessing==0 ) {
		valueArgs_AST = RefLineAST(currentAST.root);
#line 438 "esterel.g"
		valueArgs_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(ARGS,"args")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(valueArgs_AST))));
#line 4473 "EsterelParser.cpp"
		currentAST.root = valueArgs_AST;
		if ( valueArgs_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			valueArgs_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = valueArgs_AST->getFirstChild();
		else
			currentAST.child = valueArgs_AST;
		currentAST.advanceChildToEnd();
	}
	valueArgs_AST = RefLineAST(currentAST.root);
	returnAST = valueArgs_AST;
}

void EsterelParser::presentThenPart() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST presentThenPart_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	presentEvent();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_then:
	{
		match(LITERAL_then);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case LITERAL_else:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		presentThenPart_AST = RefLineAST(currentAST.root);
#line 451 "esterel.g"
		presentThenPart_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(presentThenPart_AST))));
#line 4521 "EsterelParser.cpp"
		currentAST.root = presentThenPart_AST;
		if ( presentThenPart_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			presentThenPart_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = presentThenPart_AST->getFirstChild();
		else
			currentAST.child = presentThenPart_AST;
		currentAST.advanceChildToEnd();
	}
	presentThenPart_AST = RefLineAST(currentAST.root);
	returnAST = presentThenPart_AST;
}

void EsterelParser::presentCase() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST presentCase_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LITERAL_case);
	presentEvent();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case LITERAL_else:
	case LITERAL_case:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		presentCase_AST = RefLineAST(currentAST.root);
#line 468 "esterel.g"
		presentCase_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(presentCase_AST))));
#line 4571 "EsterelParser.cpp"
		currentAST.root = presentCase_AST;
		if ( presentCase_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			presentCase_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = presentCase_AST->getFirstChild();
		else
			currentAST.child = presentCase_AST;
		currentAST.advanceChildToEnd();
	}
	presentCase_AST = RefLineAST(currentAST.root);
	returnAST = presentCase_AST;
}

void EsterelParser::elsePart() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST elsePart_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp212_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp212_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp212_AST));
	}
	match(LITERAL_else);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	elsePart_AST = RefLineAST(currentAST.root);
	returnAST = elsePart_AST;
}

void EsterelParser::presentEvent() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST presentEvent_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	switch ( LA(1)) {
	case ID:
	case LPAREN:
	case LITERAL_not:
	case LITERAL_pre:
	{
		signalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
		signalExpression();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RBRACKET);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	presentEvent_AST = RefLineAST(currentAST.root);
	returnAST = presentEvent_AST;
}

void EsterelParser::thenPart() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST thenPart_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp215_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp215_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp215_AST));
	}
	match(LITERAL_then);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	thenPart_AST = RefLineAST(currentAST.root);
	returnAST = thenPart_AST;
}

void EsterelParser::elsif() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST elsif_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp216_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp216_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp216_AST));
	}
	match(LITERAL_elsif);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_then);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	elsif_AST = RefLineAST(currentAST.root);
	returnAST = elsif_AST;
}

void EsterelParser::abortOneCaseStrong() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST abortOneCaseStrong_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_abort:
		{
			match(LITERAL_abort);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		abortOneCaseStrong_AST = RefLineAST(currentAST.root);
#line 506 "esterel.g"
		abortOneCaseStrong_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(abortOneCaseStrong_AST))));
#line 4760 "EsterelParser.cpp"
		currentAST.root = abortOneCaseStrong_AST;
		if ( abortOneCaseStrong_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			abortOneCaseStrong_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = abortOneCaseStrong_AST->getFirstChild();
		else
			currentAST.child = abortOneCaseStrong_AST;
		currentAST.advanceChildToEnd();
	}
	abortOneCaseStrong_AST = RefLineAST(currentAST.root);
	returnAST = abortOneCaseStrong_AST;
}

void EsterelParser::acase() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST acase_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LITERAL_case);
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case LITERAL_case:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		acase_AST = RefLineAST(currentAST.root);
#line 518 "esterel.g"
		acase_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(acase_AST))));
#line 4809 "EsterelParser.cpp"
		currentAST.root = acase_AST;
		if ( acase_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			acase_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = acase_AST->getFirstChild();
		else
			currentAST.child = acase_AST;
		currentAST.advanceChildToEnd();
	}
	acase_AST = RefLineAST(currentAST.root);
	returnAST = acase_AST;
}

void EsterelParser::abortOneCaseWeak() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST abortOneCaseWeak_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_abort:
		case LITERAL_weak:
		{
			{
			switch ( LA(1)) {
			case LITERAL_weak:
			{
				match(LITERAL_weak);
				break;
			}
			case LITERAL_abort:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(LITERAL_abort);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		abortOneCaseWeak_AST = RefLineAST(currentAST.root);
#line 511 "esterel.g"
		abortOneCaseWeak_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(abortOneCaseWeak_AST))));
#line 4916 "EsterelParser.cpp"
		currentAST.root = abortOneCaseWeak_AST;
		if ( abortOneCaseWeak_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			abortOneCaseWeak_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = abortOneCaseWeak_AST->getFirstChild();
		else
			currentAST.child = abortOneCaseWeak_AST;
		currentAST.advanceChildToEnd();
	}
	abortOneCaseWeak_AST = RefLineAST(currentAST.root);
	returnAST = abortOneCaseWeak_AST;
}

void EsterelParser::awaitOneCase() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST awaitOneCase_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	delayExpression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_await:
		{
			match(LITERAL_await);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		awaitOneCase_AST = RefLineAST(currentAST.root);
#line 529 "esterel.g"
		awaitOneCase_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(awaitOneCase_AST))));
#line 5005 "EsterelParser.cpp"
		currentAST.root = awaitOneCase_AST;
		if ( awaitOneCase_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			awaitOneCase_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = awaitOneCase_AST->getFirstChild();
		else
			currentAST.child = awaitOneCase_AST;
		currentAST.advanceChildToEnd();
	}
	awaitOneCase_AST = RefLineAST(currentAST.root);
	returnAST = awaitOneCase_AST;
}

void EsterelParser::trapDeclList() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapDeclList_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	trapDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			trapDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop199;
		}
		
	}
	_loop199:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		trapDeclList_AST = RefLineAST(currentAST.root);
#line 561 "esterel.g"
		trapDeclList_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TRAPS,"traps")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(trapDeclList_AST))));
#line 5047 "EsterelParser.cpp"
		currentAST.root = trapDeclList_AST;
		if ( trapDeclList_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			trapDeclList_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = trapDeclList_AST->getFirstChild();
		else
			currentAST.child = trapDeclList_AST;
		currentAST.advanceChildToEnd();
	}
	trapDeclList_AST = RefLineAST(currentAST.root);
	returnAST = trapDeclList_AST;
}

void EsterelParser::trapHandler() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapHandler_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp231_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp231_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp231_AST));
	}
	match(LITERAL_handle);
	trapEvent();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_do);
	statement();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	trapHandler_AST = RefLineAST(currentAST.root);
	returnAST = trapHandler_AST;
}

void EsterelParser::trapDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	trapIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case COLON:
	case COLEQUALS:
	{
		{
		switch ( LA(1)) {
		case COLEQUALS:
		{
			trapInitializer();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
			break;
		}
		case COLON:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(COLON);
		channelType();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case COMMA:
	case LITERAL_in:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		trapDecl_AST = RefLineAST(currentAST.root);
#line 566 "esterel.g"
		trapDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TDECL,"tdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(trapDecl_AST))));
#line 5140 "EsterelParser.cpp"
		currentAST.root = trapDecl_AST;
		if ( trapDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			trapDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = trapDecl_AST->getFirstChild();
		else
			currentAST.child = trapDecl_AST;
		currentAST.advanceChildToEnd();
	}
	trapDecl_AST = RefLineAST(currentAST.root);
	returnAST = trapDecl_AST;
}

void EsterelParser::trapInitializer() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapInitializer_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp234_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp234_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp234_AST));
	}
	match(COLEQUALS);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	trapInitializer_AST = RefLineAST(currentAST.root);
	returnAST = trapInitializer_AST;
}

void EsterelParser::trapEvent() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST trapEvent_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	eand();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_or)) {
			RefLineAST tmp235_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp235_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp235_AST));
			}
			match(LITERAL_or);
			eand();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop209;
		}
		
	}
	_loop209:;
	} // ( ... )*
	trapEvent_AST = RefLineAST(currentAST.root);
	returnAST = trapEvent_AST;
}

void EsterelParser::eand() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST eand_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	eunary();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == LITERAL_and)) {
			RefLineAST tmp236_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
			if ( inputState->guessing == 0 ) {
				tmp236_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp236_AST));
			}
			match(LITERAL_and);
			eunary();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop212;
		}
		
	}
	_loop212:;
	} // ( ... )*
	eand_AST = RefLineAST(currentAST.root);
	returnAST = eand_AST;
}

void EsterelParser::eunary() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST eunary_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case ID:
	{
		trapIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		eunary_AST = RefLineAST(currentAST.root);
		break;
	}
	case LPAREN:
	{
		match(LPAREN);
		trapEvent();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(RPAREN);
		eunary_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_not:
	{
		RefLineAST tmp239_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp239_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp239_AST));
		}
		match(LITERAL_not);
		eunary();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		eunary_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = eunary_AST;
}

void EsterelParser::execOneCase() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST execOneCase_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	taskIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	refArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	valueArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_return);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(LITERAL_end);
		{
		switch ( LA(1)) {
		case LITERAL_exec:
		{
			match(LITERAL_exec);
			break;
		}
		case LITERAL_end:
		case PERIOD:
		case SEMICOLON:
		case RBRACKET:
		case PARALLEL:
		case LITERAL_else:
		case LITERAL_case:
		case LITERAL_elsif:
		case LITERAL_when:
		case LITERAL_each:
		case LITERAL_handle:
		case LITERAL_watching:
		case LITERAL_upto:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case LITERAL_end:
	case PERIOD:
	case SEMICOLON:
	case RBRACKET:
	case PARALLEL:
	case LITERAL_else:
	case LITERAL_case:
	case LITERAL_elsif:
	case LITERAL_when:
	case LITERAL_each:
	case LITERAL_handle:
	case LITERAL_watching:
	case LITERAL_upto:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		execOneCase_AST = RefLineAST(currentAST.root);
#line 605 "esterel.g"
		execOneCase_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(execOneCase_AST))));
#line 5378 "EsterelParser.cpp"
		currentAST.root = execOneCase_AST;
		if ( execOneCase_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			execOneCase_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = execOneCase_AST->getFirstChild();
		else
			currentAST.child = execOneCase_AST;
		currentAST.advanceChildToEnd();
	}
	execOneCase_AST = RefLineAST(currentAST.root);
	returnAST = execOneCase_AST;
}

void EsterelParser::execCase() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST execCase_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	match(LITERAL_case);
	taskIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	refArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	valueArgs();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(LITERAL_return);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case LITERAL_do:
	{
		match(LITERAL_do);
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case LITERAL_end:
	case LITERAL_case:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		execCase_AST = RefLineAST(currentAST.root);
#line 611 "esterel.g"
		execCase_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CASE,"case")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(execCase_AST))));
#line 5440 "EsterelParser.cpp"
		currentAST.root = execCase_AST;
		if ( execCase_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			execCase_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = execCase_AST->getFirstChild();
		else
			currentAST.child = execCase_AST;
		currentAST.advanceChildToEnd();
	}
	execCase_AST = RefLineAST(currentAST.root);
	returnAST = execCase_AST;
}

void EsterelParser::variableDeclList() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableDeclList_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	variableDecls();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			variableDecls();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop229;
		}
		
	}
	_loop229:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		variableDeclList_AST = RefLineAST(currentAST.root);
#line 628 "esterel.g"
		variableDeclList_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(VARS,"vars")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(variableDeclList_AST))));
#line 5482 "EsterelParser.cpp"
		currentAST.root = variableDeclList_AST;
		if ( variableDeclList_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			variableDeclList_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = variableDeclList_AST->getFirstChild();
		else
			currentAST.child = variableDeclList_AST;
		currentAST.advanceChildToEnd();
	}
	variableDeclList_AST = RefLineAST(currentAST.root);
	returnAST = variableDeclList_AST;
}

void EsterelParser::variableDecls() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableDecls_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	variableDeclList2();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	RefLineAST tmp248_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp248_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp248_AST));
	}
	match(COLON);
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	variableDecls_AST = RefLineAST(currentAST.root);
	returnAST = variableDecls_AST;
}

void EsterelParser::variableDeclList2() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableDeclList2_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	variableDecl();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			variableDecl();
			if (inputState->guessing==0) {
				astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
			}
		}
		else {
			goto _loop234;
		}
		
	}
	_loop234:;
	} // ( ... )*
	}
	if ( inputState->guessing==0 ) {
		variableDeclList2_AST = RefLineAST(currentAST.root);
#line 642 "esterel.g"
		variableDeclList2_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(VARS,"vars")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(variableDeclList2_AST))));
#line 5549 "EsterelParser.cpp"
		currentAST.root = variableDeclList2_AST;
		if ( variableDeclList2_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			variableDeclList2_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = variableDeclList2_AST->getFirstChild();
		else
			currentAST.child = variableDeclList2_AST;
		currentAST.advanceChildToEnd();
	}
	variableDeclList2_AST = RefLineAST(currentAST.root);
	returnAST = variableDeclList2_AST;
}

void EsterelParser::variableDecl() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableDecl_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	variableIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	{
	switch ( LA(1)) {
	case COLEQUALS:
	{
		variableInitializer();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case COLON:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		variableDecl_AST = RefLineAST(currentAST.root);
#line 649 "esterel.g"
		variableDecl_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(VDECL,"vdecl")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(variableDecl_AST))));
#line 5596 "EsterelParser.cpp"
		currentAST.root = variableDecl_AST;
		if ( variableDecl_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			variableDecl_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = variableDecl_AST->getFirstChild();
		else
			currentAST.child = variableDecl_AST;
		currentAST.advanceChildToEnd();
	}
	variableDecl_AST = RefLineAST(currentAST.root);
	returnAST = variableDecl_AST;
}

void EsterelParser::variableInitializer() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST variableInitializer_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	RefLineAST tmp250_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	if ( inputState->guessing == 0 ) {
		tmp250_AST = astFactory->create(LT(1));
		astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp250_AST));
	}
	match(COLEQUALS);
	expression();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	variableInitializer_AST = RefLineAST(currentAST.root);
	returnAST = variableInitializer_AST;
}

void EsterelParser::renaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST renaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_type:
	{
		match(LITERAL_type);
		typeRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				typeRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop246;
			}
			
		}
		_loop246:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_constant:
	{
		match(LITERAL_constant);
		constantRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				constantRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop248;
			}
			
		}
		_loop248:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_function:
	{
		match(LITERAL_function);
		functionRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				functionRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop250;
			}
			
		}
		_loop250:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_procedure:
	{
		match(LITERAL_procedure);
		procedureRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				procedureRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop252;
			}
			
		}
		_loop252:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_task:
	{
		match(LITERAL_task);
		taskRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				taskRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop254;
			}
			
		}
		_loop254:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_signal:
	{
		match(LITERAL_signal);
		signalRenaming();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				signalRenaming();
				if (inputState->guessing==0) {
					astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
				}
			}
			else {
				goto _loop256;
			}
			
		}
		_loop256:;
		} // ( ... )*
		renaming_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = renaming_AST;
}

void EsterelParser::typeRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST typeRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(SLASH);
	typeIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		typeRenaming_AST = RefLineAST(currentAST.root);
#line 678 "esterel.g"
		typeRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TRENAME,"trename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(typeRenaming_AST))));
#line 5816 "EsterelParser.cpp"
		currentAST.root = typeRenaming_AST;
		if ( typeRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			typeRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = typeRenaming_AST->getFirstChild();
		else
			currentAST.child = typeRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	typeRenaming_AST = RefLineAST(currentAST.root);
	returnAST = typeRenaming_AST;
}

void EsterelParser::constantRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST constantRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	constantAtom();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(SLASH);
	constantIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		constantRenaming_AST = RefLineAST(currentAST.root);
#line 683 "esterel.g"
		constantRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(CRENAME,"crename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(constantRenaming_AST))));
#line 5847 "EsterelParser.cpp"
		currentAST.root = constantRenaming_AST;
		if ( constantRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			constantRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = constantRenaming_AST->getFirstChild();
		else
			currentAST.child = constantRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	constantRenaming_AST = RefLineAST(currentAST.root);
	returnAST = constantRenaming_AST;
}

void EsterelParser::functionRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST functionRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	{
	switch ( LA(1)) {
	case ID:
	{
		functionIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(SLASH);
		functionIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	case EQUALS:
	case PLUS:
	case STAR:
	case LITERAL_or:
	case LITERAL_and:
	case LITERAL_not:
	case NEQUAL:
	case LESSTHAN:
	case GREATERTHAN:
	case LEQUAL:
	case GEQUAL:
	case DASH:
	case SLASH:
	case LITERAL_mod:
	{
		predefinedFunction();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		match(SLASH);
		functionIdentifier();
		if (inputState->guessing==0) {
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		functionRenaming_AST = RefLineAST(currentAST.root);
#line 689 "esterel.g"
		functionRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(FRENAME,"frename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(functionRenaming_AST))));
#line 5916 "EsterelParser.cpp"
		currentAST.root = functionRenaming_AST;
		if ( functionRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			functionRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = functionRenaming_AST->getFirstChild();
		else
			currentAST.child = functionRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	functionRenaming_AST = RefLineAST(currentAST.root);
	returnAST = functionRenaming_AST;
}

void EsterelParser::procedureRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST procedureRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	procedureIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(SLASH);
	procedureIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		procedureRenaming_AST = RefLineAST(currentAST.root);
#line 712 "esterel.g"
		procedureRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(PRENAME,"prename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(procedureRenaming_AST))));
#line 5947 "EsterelParser.cpp"
		currentAST.root = procedureRenaming_AST;
		if ( procedureRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			procedureRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = procedureRenaming_AST->getFirstChild();
		else
			currentAST.child = procedureRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	procedureRenaming_AST = RefLineAST(currentAST.root);
	returnAST = procedureRenaming_AST;
}

void EsterelParser::taskRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST taskRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	taskIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(SLASH);
	taskIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		taskRenaming_AST = RefLineAST(currentAST.root);
#line 717 "esterel.g"
		taskRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(TARENAME,"trename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(taskRenaming_AST))));
#line 5978 "EsterelParser.cpp"
		currentAST.root = taskRenaming_AST;
		if ( taskRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			taskRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = taskRenaming_AST->getFirstChild();
		else
			currentAST.child = taskRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	taskRenaming_AST = RefLineAST(currentAST.root);
	returnAST = taskRenaming_AST;
}

void EsterelParser::signalRenaming() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST signalRenaming_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	match(SLASH);
	signalIdentifier();
	if (inputState->guessing==0) {
		astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST));
	}
	if ( inputState->guessing==0 ) {
		signalRenaming_AST = RefLineAST(currentAST.root);
#line 723 "esterel.g"
		signalRenaming_AST = RefLineAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->create(SRENAME,"srename")))->add(ANTLR_USE_NAMESPACE(antlr)RefAST(signalRenaming_AST))));
#line 6009 "EsterelParser.cpp"
		currentAST.root = signalRenaming_AST;
		if ( signalRenaming_AST!=RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) &&
			signalRenaming_AST->getFirstChild() != RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			  currentAST.child = signalRenaming_AST->getFirstChild();
		else
			currentAST.child = signalRenaming_AST;
		currentAST.advanceChildToEnd();
	}
	signalRenaming_AST = RefLineAST(currentAST.root);
	returnAST = signalRenaming_AST;
}

void EsterelParser::predefinedFunction() {
	returnAST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	RefLineAST predefinedFunction_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	switch ( LA(1)) {
	case LITERAL_and:
	{
		RefLineAST tmp270_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp270_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp270_AST));
		}
		match(LITERAL_and);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_or:
	{
		RefLineAST tmp271_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp271_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp271_AST));
		}
		match(LITERAL_or);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_not:
	{
		RefLineAST tmp272_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp272_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp272_AST));
		}
		match(LITERAL_not);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case PLUS:
	{
		RefLineAST tmp273_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp273_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp273_AST));
		}
		match(PLUS);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case DASH:
	{
		RefLineAST tmp274_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp274_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp274_AST));
		}
		match(DASH);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case STAR:
	{
		RefLineAST tmp275_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp275_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp275_AST));
		}
		match(STAR);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case SLASH:
	{
		RefLineAST tmp276_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp276_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp276_AST));
		}
		match(SLASH);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LITERAL_mod:
	{
		RefLineAST tmp277_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp277_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp277_AST));
		}
		match(LITERAL_mod);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LESSTHAN:
	{
		RefLineAST tmp278_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp278_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp278_AST));
		}
		match(LESSTHAN);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case GREATERTHAN:
	{
		RefLineAST tmp279_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp279_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp279_AST));
		}
		match(GREATERTHAN);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case LEQUAL:
	{
		RefLineAST tmp280_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp280_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp280_AST));
		}
		match(LEQUAL);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case GEQUAL:
	{
		RefLineAST tmp281_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp281_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp281_AST));
		}
		match(GEQUAL);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case NEQUAL:
	{
		RefLineAST tmp282_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp282_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp282_AST));
		}
		match(NEQUAL);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	case EQUALS:
	{
		RefLineAST tmp283_AST = RefLineAST(ANTLR_USE_NAMESPACE(antlr)nullAST);
		if ( inputState->guessing == 0 ) {
			tmp283_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp283_AST));
		}
		match(EQUALS);
		predefinedFunction_AST = RefLineAST(currentAST.root);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = predefinedFunction_AST;
}

void EsterelParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.setMaxNodeType(124);
}
const char* EsterelParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"SIGS",
	"VARS",
	"TYPES",
	"DECLS",
	"TRAPS",
	"SEQUENCE",
	"ARGS",
	"CASE",
	"DELAY",
	"DOWATCHING",
	"DOUPTO",
	"CDECL",
	"TDECL",
	"FDECL",
	"PDECL",
	"TADECL",
	"VDECLS",
	"VDECL",
	"SDECL",
	"CALL",
	"RUN",
	"TRENAME",
	"CRENAME",
	"SRENAME",
	"TARENAME",
	"FRENAME",
	"PRENAME",
	"Integer",
	"FloatConst",
	"DoubleConst",
	"\"module\"",
	"COLON",
	"\"end\"",
	"PERIOD",
	"\"type\"",
	"COMMA",
	"SEMICOLON",
	"\"constant\"",
	"EQUALS",
	"ID",
	"\"function\"",
	"LPAREN",
	"RPAREN",
	"\"procedure\"",
	"\"task\"",
	"\"input\"",
	"\"output\"",
	"\"inputoutput\"",
	"\"return\"",
	"COLEQUALS",
	"\"combine\"",
	"\"with\"",
	"PLUS",
	"STAR",
	"\"or\"",
	"\"and\"",
	"\"sensor\"",
	"\"relation\"",
	"IMPLIES",
	"POUND",
	"\"not\"",
	"NEQUAL",
	"LESSTHAN",
	"GREATERTHAN",
	"LEQUAL",
	"GEQUAL",
	"DASH",
	"SLASH",
	"\"mod\"",
	"QUESTION",
	"\"pre\"",
	"DQUESTION",
	"\"true\"",
	"\"false\"",
	"\"immediate\"",
	"LBRACKET",
	"RBRACKET",
	"PARALLEL",
	"\"nothing\"",
	"\"pause\"",
	"\"halt\"",
	"\"emit\"",
	"\"sustain\"",
	"\"call\"",
	"\"present\"",
	"\"then\"",
	"\"else\"",
	"\"case\"",
	"\"do\"",
	"\"if\"",
	"\"elsif\"",
	"\"positive\"",
	"\"repeat\"",
	"\"times\"",
	"\"abort\"",
	"\"when\"",
	"\"weak\"",
	"\"await\"",
	"\"loop\"",
	"\"each\"",
	"\"every\"",
	"\"suspend\"",
	"\"trap\"",
	"\"in\"",
	"\"exit\"",
	"\"handle\"",
	"\"exec\"",
	"\"signal\"",
	"\"var\"",
	"\"run\"",
	"\"copymodule\"",
	"\"watching\"",
	"\"timeout\"",
	"\"upto\"",
	"StringConstant",
	"Number",
	"FractionalNumber",
	"Exponent",
	"Whitespace",
	"Newline",
	"Comment",
	0
};

const unsigned long EsterelParser::_tokenSet_0_data_[] = { 0UL, 807375424UL, 0UL, 0UL };
// "type" "constant" "function" "procedure" "task" "input" "output" "inputoutput" 
// "return" "sensor" "relation" 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long EsterelParser::_tokenSet_1_data_[] = { 0UL, 1024UL, 62UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EQUALS NEQUAL LESSTHAN GREATERTHAN LEQUAL GEQUAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_1(_tokenSet_1_data_,8);
const unsigned long EsterelParser::_tokenSet_2_data_[] = { 2147483648UL, 2051UL, 12288UL, 4194304UL, 0UL, 0UL, 0UL, 0UL };
// Integer FloatConst DoubleConst ID "true" "false" StringConstant 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_2(_tokenSet_2_data_,8);
const unsigned long EsterelParser::_tokenSet_3_data_[] = { 0UL, 251678136UL, 1308852734UL, 2629770UL, 0UL, 0UL, 0UL, 0UL };
// COLON "end" PERIOD COMMA SEMICOLON EQUALS ID RPAREN PLUS STAR "or" "and" 
// NEQUAL LESSTHAN GREATERTHAN LEQUAL GEQUAL DASH SLASH "mod" LBRACKET 
// RBRACKET PARALLEL "then" "else" "case" "elsif" "times" "when" "each" 
// "handle" "watching" "upto" 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_3(_tokenSet_3_data_,8);
const unsigned long EsterelParser::_tokenSet_4_data_[] = { 2147483648UL, 10243UL, 15937UL, 4194304UL, 0UL, 0UL, 0UL, 0UL };
// Integer FloatConst DoubleConst ID LPAREN "not" DASH QUESTION "pre" DQUESTION 
// "true" "false" StringConstant 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_4(_tokenSet_4_data_,8);
const unsigned long EsterelParser::_tokenSet_5_data_[] = { 2147483648UL, 251669507UL, 49151UL, 4194304UL, 0UL, 0UL, 0UL, 0UL };
// Integer FloatConst DoubleConst EQUALS ID LPAREN PLUS STAR "or" "and" 
// "not" NEQUAL LESSTHAN GREATERTHAN LEQUAL GEQUAL DASH SLASH "mod" QUESTION 
// "pre" DQUESTION "true" "false" LBRACKET StringConstant 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_5(_tokenSet_5_data_,8);
const unsigned long EsterelParser::_tokenSet_6_data_[] = { 0UL, 10544UL, 1543701505UL, 3678344UL, 0UL, 0UL, 0UL, 0UL };
// "end" PERIOD SEMICOLON ID LPAREN "not" "pre" RBRACKET PARALLEL "else" 
// "case" "do" "elsif" "when" "each" "handle" "watching" "timeout" "upto" 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_6(_tokenSet_6_data_,8);
const unsigned long EsterelParser::_tokenSet_7_data_[] = { 0UL, 2048UL, 2986115072UL, 513909UL, 0UL, 0UL, 0UL, 0UL };
// ID LBRACKET "nothing" "pause" "halt" "emit" "sustain" "call" "present" 
// "do" "if" "positive" "repeat" "abort" "weak" "await" "loop" "every" 
// "suspend" "trap" "exit" "exec" "signal" "var" "run" "copymodule" 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelParser::_tokenSet_7(_tokenSet_7_data_,8);


