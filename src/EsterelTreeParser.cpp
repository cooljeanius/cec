/* $ANTLR 2.7.2: "staticsemantics.g" -> "EsterelTreeParser.cpp"$ */
#include "EsterelTreeParser.hpp"
#include <antlr/Token.hpp>
#include <antlr/AST.hpp>
#include <antlr/NoViableAltException.hpp>
#include <antlr/MismatchedTokenException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/BitSet.hpp>
#line 39 "staticsemantics.g"


static void
create_constant(RefLineAST l, Context *c, string name, Expression *e,
	        TypeSymbol *t)
{
  assert(t);
  if (c->module->constants->local_contains(name))
    throw LineError(l, "redefined constant " + name);
  if (c->module->variables->local_contains(name))
    throw LineError(l, "internal error: constant name already in use");
  if (e) {
    assert(e->type);
    if ( e->type != t)
      throw LineError(l, "type of initializer does not match type of constant");
  }
  c->module->constants->enter(new ConstantSymbol(name, t, e));
}

static Expression*
boolean_binop(RefLineAST l, Context *c, string op,
	      Expression *e1, Expression *e2)
{
  assert(c);
  assert(e1);
  assert(e2);

  if (e1->type != e2->type || e1->type != c->boolean_type)
    throw LineError(l, "arguments of " + op + " must be boolean");
  return new BinaryOp(e1->type, op, e1, e2);
}

static Expression*
numeric_binop(RefLineAST l, Context *c, string op,
              Expression *e1, Expression *e2)
{
  assert(c);
  assert(e1);
  assert(e2);

  if (e1->type != e2->type ||
      !(e1->type == c->integer_type || e1->type == c->float_type ||
	e1->type == c->double_type))
    throw LineError(l, "arguments of " + op + " must be numeric and be the same type");
  return new BinaryOp(e1->type, op, e1, e2);
}

static Expression*
equality(RefLineAST l, Context *c, string op, Expression *e1, Expression *e2)
{
  assert(c);
  assert(e1);
  assert(e2);

  if (e1->type != e2->type)
    throw LineError(l, "type of arguments of " + op + " must match");
  return new BinaryOp(c->boolean_type, op, e1, e2);
}

static Expression*
comparison(RefLineAST l, Context *c, string op, Expression *e1, Expression *e2)
{
  assert(c);
  assert(e1);
  assert(e2);

  if (e1->type != e2->type ||
      !(e1->type == c->integer_type || e1->type == c->float_type ||
	e1->type == c->double_type))
    throw LineError(l, "type of arguments of " + op + " must match");
  return new BinaryOp(c->boolean_type, op, e1, e2);
}


#line 85 "EsterelTreeParser.cpp"
#line 1 "staticsemantics.g"
#line 87 "EsterelTreeParser.cpp"
EsterelTreeParser::EsterelTreeParser()
	: ANTLR_USE_NAMESPACE(antlr)TreeParser() {
}

void EsterelTreeParser::file(RefLineAST _t,
	Modules *ms, string filename
) {
	RefLineAST file_AST_in = _t;
	
	try {      // for error handling
#line 129 "staticsemantics.g"
		
		assert(ms);
		
#line 102 "EsterelTreeParser.cpp"
		{ // ( ... )+
		int _cnt3=0;
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == LITERAL_module)) {
				module(_t,ms);
				_t = _retTree;
			}
			else {
				if ( _cnt3>=1 ) { goto _loop3; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
			}
			
			_cnt3++;
		}
		_loop3:;
		}  // ( ... )+
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::module(RefLineAST _t,
	Modules* modules
) {
	RefLineAST module_AST_in = _t;
	RefLineAST moduleName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t5 = _t;
		RefLineAST tmp1_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_module);
		_t = _t->getFirstChild();
		moduleName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 137 "staticsemantics.g"
		
		assert(modules);
		string name = moduleName->getText();
		if (modules->module_symbols.local_contains(name))
		throw LineError(moduleName, "Duplicate module " + name);
		ModuleSymbol *ms = new ModuleSymbol(name);
		Module *m = new Module(ms);
		
		ms->module = m;
		modules->add(m);
		
		Context c(m);
		
		// Primitive types from Primer v91, p.46
		m->types->enter(c.boolean_type = new BuiltinTypeSymbol("boolean"));
		m->types->enter(c.integer_type = new BuiltinTypeSymbol("integer"));
		m->types->enter(c.float_type = new BuiltinTypeSymbol("float"));
		m->types->enter(c.double_type = new BuiltinTypeSymbol("double"));
		m->types->enter(c.string_type = new BuiltinTypeSymbol("string"));
		
		// Built-in constants
		m->constants->enter(c.false_constant = 
		new BuiltinConstantSymbol("false", c.boolean_type,
		new Literal("0", c.boolean_type)));
		m->constants->enter(c.true_constant =
		new BuiltinConstantSymbol("true", c.boolean_type,
		new Literal("1", c.boolean_type)));
		
		// Built-in functions
		m->functions->enter(new BuiltinFunctionSymbol("+"));
		m->functions->enter(new BuiltinFunctionSymbol("-"));
		m->functions->enter(new BuiltinFunctionSymbol("*"));
		m->functions->enter(new BuiltinFunctionSymbol("/"));
		m->functions->enter(new BuiltinFunctionSymbol("mod"));
		m->functions->enter(new BuiltinFunctionSymbol("and"));
		m->functions->enter(new BuiltinFunctionSymbol("or"));
		m->functions->enter(new BuiltinFunctionSymbol("not"));
		m->functions->enter(new BuiltinFunctionSymbol("<"));
		m->functions->enter(new BuiltinFunctionSymbol("<="));
		m->functions->enter(new BuiltinFunctionSymbol(">"));
		m->functions->enter(new BuiltinFunctionSymbol(">="));
		m->functions->enter(new BuiltinFunctionSymbol("="));
		m->functions->enter(new BuiltinFunctionSymbol("<>"));
		
		// The built-in signal "tick"
		m->signals->enter(new BuiltinSignalSymbol("tick", 0,
		SignalSymbol::Input, 0));
		
		Statement *s;
		
#line 194 "EsterelTreeParser.cpp"
		declarations(_t,&c);
		_t = _retTree;
		s=statement(_t,&c);
		_t = _retTree;
#line 189 "staticsemantics.g"
		m->body = s;
#line 201 "EsterelTreeParser.cpp"
		_t = __t5;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::declarations(RefLineAST _t,
	Context *c
) {
	RefLineAST declarations_AST_in = _t;
	
	try {      // for error handling
		RefLineAST __t7 = _t;
		RefLineAST tmp2_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DECLS);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_type:
			{
				RefLineAST __t9 = _t;
				RefLineAST tmp3_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_type);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt11=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == ID)) {
						typeDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt11>=1 ) { goto _loop11; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt11++;
				}
				_loop11:;
				}  // ( ... )+
				_t = __t9;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_constant:
			{
				RefLineAST __t12 = _t;
				RefLineAST tmp4_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_constant);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt14=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == CDECL)) {
						constantDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt14>=1 ) { goto _loop14; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt14++;
				}
				_loop14:;
				}  // ( ... )+
				_t = __t12;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_input:
			{
				RefLineAST __t15 = _t;
				RefLineAST tmp5_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_input);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt17=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == SDECL)) {
						signalDecl(_t,c, "input", true);
						_t = _retTree;
					}
					else {
						if ( _cnt17>=1 ) { goto _loop17; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt17++;
				}
				_loop17:;
				}  // ( ... )+
				_t = __t15;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_output:
			{
				RefLineAST __t18 = _t;
				RefLineAST tmp6_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_output);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt20=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == SDECL)) {
						signalDecl(_t,c, "output", true );
						_t = _retTree;
					}
					else {
						if ( _cnt20>=1 ) { goto _loop20; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt20++;
				}
				_loop20:;
				}  // ( ... )+
				_t = __t18;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_inputoutput:
			{
				RefLineAST __t21 = _t;
				RefLineAST tmp7_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_inputoutput);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt23=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == SDECL)) {
						signalDecl(_t,c, "inputoutput", true);
						_t = _retTree;
					}
					else {
						if ( _cnt23>=1 ) { goto _loop23; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt23++;
				}
				_loop23:;
				}  // ( ... )+
				_t = __t21;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_return:
			{
				RefLineAST __t24 = _t;
				RefLineAST tmp8_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_return);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt26=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == SDECL)) {
						signalDecl(_t,c, "return", true);
						_t = _retTree;
					}
					else {
						if ( _cnt26>=1 ) { goto _loop26; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt26++;
				}
				_loop26:;
				}  // ( ... )+
				_t = __t24;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_function:
			{
				RefLineAST __t27 = _t;
				RefLineAST tmp9_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_function);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt29=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == FDECL)) {
						functionDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt29>=1 ) { goto _loop29; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt29++;
				}
				_loop29:;
				}  // ( ... )+
				_t = __t27;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_procedure:
			{
				RefLineAST __t30 = _t;
				RefLineAST tmp10_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_procedure);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt32=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == PDECL)) {
						procedureDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt32>=1 ) { goto _loop32; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt32++;
				}
				_loop32:;
				}  // ( ... )+
				_t = __t30;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_task:
			{
				RefLineAST __t33 = _t;
				RefLineAST tmp11_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_task);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt35=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == TADECL)) {
						taskDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt35>=1 ) { goto _loop35; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt35++;
				}
				_loop35:;
				}  // ( ... )+
				_t = __t33;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_sensor:
			{
				RefLineAST __t36 = _t;
				RefLineAST tmp12_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_sensor);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt38=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == SDECL)) {
						signalDecl(_t,c, "sensor", true);
						_t = _retTree;
					}
					else {
						if ( _cnt38>=1 ) { goto _loop38; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt38++;
				}
				_loop38:;
				}  // ( ... )+
				_t = __t36;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_relation:
			{
				RefLineAST __t39 = _t;
				RefLineAST tmp13_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_relation);
				_t = _t->getFirstChild();
				{ // ( ... )+
				int _cnt41=0;
				for (;;) {
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					if ((_t->getType() == IMPLIES || _t->getType() == POUND)) {
						relationDecl(_t,c);
						_t = _retTree;
					}
					else {
						if ( _cnt41>=1 ) { goto _loop41; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
					}
					
					_cnt41++;
				}
				_loop41:;
				}  // ( ... )+
				_t = __t39;
				_t = _t->getNextSibling();
				break;
			}
			default:
			{
				goto _loop42;
			}
			}
		}
		_loop42:;
		} // ( ... )*
		_t = __t7;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

Statement * EsterelTreeParser::statement(RefLineAST _t,
	Context *c
) {
#line 414 "staticsemantics.g"
	Statement *st;
#line 549 "EsterelTreeParser.cpp"
	RefLineAST statement_AST_in = _t;
	RefLineAST esig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST ssig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST var = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST proc = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST ife = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST elif = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST expr = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST etrap = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST texpr = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST taskid = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST tsig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST vdecls = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST module = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST oldMod = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST tID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST cID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST func = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST fID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST pro = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST pID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST task = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST trID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST sig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST sID = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST de = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 415 "staticsemantics.g"
		st = 0;
#line 580 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case SEQUENCE:
		{
			RefLineAST __t93 = _t;
			RefLineAST tmp14_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SEQUENCE);
			_t = _t->getFirstChild();
#line 416 "staticsemantics.g"
			StatementList *sl = new StatementList();
#line 593 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt95=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_tokenSet_0.member(_t->getType()))) {
					st=statement(_t,c);
					_t = _retTree;
#line 417 "staticsemantics.g"
					sl->statements.push_back(st);
#line 604 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt95>=1 ) { goto _loop95; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt95++;
			}
			_loop95:;
			}  // ( ... )+
			_t = __t93;
			_t = _t->getNextSibling();
#line 418 "staticsemantics.g"
			st = sl;
#line 618 "EsterelTreeParser.cpp"
			break;
		}
		case PARALLEL:
		{
			RefLineAST __t96 = _t;
			RefLineAST tmp15_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PARALLEL);
			_t = _t->getFirstChild();
#line 421 "staticsemantics.g"
			ParallelStatementList *sl = new ParallelStatementList();
#line 629 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt98=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_tokenSet_0.member(_t->getType()))) {
					st=statement(_t,c);
					_t = _retTree;
#line 422 "staticsemantics.g"
					sl->threads.push_back(st);
#line 640 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt98>=1 ) { goto _loop98; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt98++;
			}
			_loop98:;
			}  // ( ... )+
			_t = __t96;
			_t = _t->getNextSibling();
#line 423 "staticsemantics.g"
			st = sl;
#line 654 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_nothing:
		{
			RefLineAST tmp16_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_nothing);
			_t = _t->getNextSibling();
#line 425 "staticsemantics.g"
			st = new Nothing();
#line 664 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_pause:
		{
			RefLineAST tmp17_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_pause);
			_t = _t->getNextSibling();
#line 427 "staticsemantics.g"
			st = new Pause();
#line 674 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_halt:
		{
			RefLineAST tmp18_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_halt);
			_t = _t->getNextSibling();
#line 429 "staticsemantics.g"
			st = new Halt();
#line 684 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_emit:
		{
			RefLineAST __t99 = _t;
			RefLineAST tmp19_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_emit);
			_t = _t->getFirstChild();
			esig = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 433 "staticsemantics.g"
			
			string name = esig->getText();
			if (!c->signals->contains(name))
			throw LineError(esig, "undeclared signal " + name);
			SignalSymbol *ss =
			dynamic_cast<SignalSymbol*>(c->signals->get(name));
			assert(ss);
			Expression *e = 0;
			
#line 706 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case CALL:
			case Integer:
			case FloatConst:
			case DoubleConst:
			case EQUALS:
			case ID:
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
			case QUESTION:
			case LITERAL_pre:
			case DQUESTION:
			case LITERAL_true:
			case LITERAL_false:
			case StringConstant:
			{
				e=expression(_t,c);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 443 "staticsemantics.g"
			
			st = new Emit(ss, e);
			if (e) {
			if (ss->type == NULL)
			throw LineError(esig, "emission of value on pure signal " + name);
			if (ss->type != e->type)
			throw LineError(esig, "expression type mismatch on emit " + name);
			}
			// A valued signal does not have to have a valued emission (!)
			
#line 762 "EsterelTreeParser.cpp"
			_t = __t99;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_sustain:
		{
			RefLineAST __t101 = _t;
			RefLineAST tmp20_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_sustain);
			_t = _t->getFirstChild();
			ssig = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 457 "staticsemantics.g"
			
			string name = ssig->getText();
			if (!c->signals->contains(name))
			throw LineError(ssig, "undeclared signal " + name);
			SignalSymbol *ss =
			dynamic_cast<SignalSymbol*>(c->signals->get(name));
			assert(ss);
			Expression *e = 0;
			
#line 786 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case CALL:
			case Integer:
			case FloatConst:
			case DoubleConst:
			case EQUALS:
			case ID:
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
			case QUESTION:
			case LITERAL_pre:
			case DQUESTION:
			case LITERAL_true:
			case LITERAL_false:
			case StringConstant:
			{
				e=expression(_t,c);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 467 "staticsemantics.g"
			
			st = new Sustain(ss, e);
			if (e) {
			if (ss->type == NULL)
			throw LineError(ssig, "sustain of value on pure signal " + name);
			if (ss->type != e->type)
			throw LineError(ssig, "expression type mismatch on sustain " + name);
			}
			// A valued signal does not have to have a valued emission
			
#line 842 "EsterelTreeParser.cpp"
			_t = __t101;
			_t = _t->getNextSibling();
			break;
		}
		case COLEQUALS:
		{
#line 479 "staticsemantics.g"
			Expression *e;
#line 851 "EsterelTreeParser.cpp"
			RefLineAST __t103 = _t;
			RefLineAST tmp21_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COLEQUALS);
			_t = _t->getFirstChild();
			var = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			e=expression(_t,c);
			_t = _retTree;
			_t = __t103;
			_t = _t->getNextSibling();
#line 481 "staticsemantics.g"
			
			string name = var->getText();
			if (!c->variables->contains(name))
			throw LineError(var, "undeclared variable " + name);
			VariableSymbol *vs =
			dynamic_cast<VariableSymbol*>(c->variables->get(name));
			assert(vs);
			st = new Assign(vs, e);
			assert(vs->type);
			assert(e->type);
			if (vs->type != e->type)
			throw LineError(var, "type of rhs (" + e->type->name +
						   ") does not match type of variable " + name +
						   " ("  + vs->type->name + ")" );
			
#line 879 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_call:
		{
			RefLineAST __t104 = _t;
			RefLineAST tmp22_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_call);
			_t = _t->getFirstChild();
			proc = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 498 "staticsemantics.g"
			
			string name = proc->getText();
			if (!c->module->procedures->contains(name))
			throw LineError(proc, "undeclared procedure " + name);
			ProcedureSymbol *ps =
			dynamic_cast<ProcedureSymbol*>(c->module->procedures->get(name));
			assert(ps);
			ProcedureCall *pc = new ProcedureCall(ps);
			
#line 901 "EsterelTreeParser.cpp"
			procArgs(_t,c, pc);
			_t = _retTree;
#line 508 "staticsemantics.g"
			st = pc;
#line 906 "EsterelTreeParser.cpp"
			_t = __t104;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_present:
		{
			RefLineAST __t105 = _t;
			RefLineAST tmp23_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_present);
			_t = _t->getFirstChild();
#line 511 "staticsemantics.g"
			Present *p = new Present();
#line 919 "EsterelTreeParser.cpp"
			{ // ( ... )*
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == CASE)) {
					RefLineAST __t107 = _t;
					RefLineAST tmp24_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CASE);
					_t = _t->getFirstChild();
#line 512 "staticsemantics.g"
					Expression *e; Statement *s = 0;
#line 931 "EsterelTreeParser.cpp"
					e=sigExpression(_t,c);
					_t = _retTree;
					{
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					switch ( _t->getType()) {
					case SEQUENCE:
					case DOWATCHING:
					case DOUPTO:
					case RUN:
					case COLEQUALS:
					case PARALLEL:
					case LITERAL_nothing:
					case LITERAL_pause:
					case LITERAL_halt:
					case LITERAL_emit:
					case LITERAL_sustain:
					case LITERAL_call:
					case LITERAL_present:
					case LITERAL_if:
					case LITERAL_repeat:
					case LITERAL_abort:
					case LITERAL_await:
					case LITERAL_loop:
					case LITERAL_every:
					case LITERAL_suspend:
					case LITERAL_trap:
					case LITERAL_exit:
					case LITERAL_exec:
					case LITERAL_signal:
					case LITERAL_var:
					{
						s=statement(_t,c);
						_t = _retTree;
						break;
					}
					case 3:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
					}
					}
					}
					_t = __t107;
					_t = _t->getNextSibling();
#line 513 "staticsemantics.g"
					p->newCase(s, e);
#line 982 "EsterelTreeParser.cpp"
				}
				else {
					goto _loop109;
				}
				
			}
			_loop109:;
			} // ( ... )*
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_else:
			{
				RefLineAST __t111 = _t;
				RefLineAST tmp25_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_else);
				_t = _t->getFirstChild();
#line 515 "staticsemantics.g"
				Statement *s;
#line 1003 "EsterelTreeParser.cpp"
				s=statement(_t,c);
				_t = _retTree;
#line 517 "staticsemantics.g"
				p->default_stmt = s;
#line 1008 "EsterelTreeParser.cpp"
				_t = __t111;
				_t = _t->getNextSibling();
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 520 "staticsemantics.g"
			st = p;
#line 1025 "EsterelTreeParser.cpp"
			_t = __t105;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_if:
		{
			RefLineAST __t112 = _t;
			RefLineAST tmp26_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_if);
			_t = _t->getFirstChild();
#line 523 "staticsemantics.g"
			If *i = new If(); Expression *e;
#line 1038 "EsterelTreeParser.cpp"
			ife = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			e=expression(_t,c);
			_t = _retTree;
#line 525 "staticsemantics.g"
			Statement *s = 0;
#line 1044 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_then:
			{
				RefLineAST __t114 = _t;
				RefLineAST tmp27_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_then);
				_t = _t->getFirstChild();
				s=statement(_t,c);
				_t = _retTree;
				_t = __t114;
				_t = _t->getNextSibling();
				break;
			}
			case 3:
			case LITERAL_else:
			case LITERAL_elsif:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 527 "staticsemantics.g"
			i->newCase(s, e);
#line 1075 "EsterelTreeParser.cpp"
			{ // ( ... )*
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == LITERAL_elsif)) {
					RefLineAST __t116 = _t;
					elif = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_elsif);
					_t = _t->getFirstChild();
					e=expression(_t,c);
					_t = _retTree;
					s=statement(_t,c);
					_t = _retTree;
					_t = __t116;
					_t = _t->getNextSibling();
#line 529 "staticsemantics.g"
					i->newCase(s, e);
#line 1093 "EsterelTreeParser.cpp"
				}
				else {
					goto _loop117;
				}
				
			}
			_loop117:;
			} // ( ... )*
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_else:
			{
				RefLineAST __t119 = _t;
				RefLineAST tmp28_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_else);
				_t = _t->getFirstChild();
				s=statement(_t,c);
				_t = _retTree;
#line 531 "staticsemantics.g"
				i->default_stmt = s;
#line 1116 "EsterelTreeParser.cpp"
				_t = __t119;
				_t = _t->getNextSibling();
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 532 "staticsemantics.g"
			st = i;
#line 1133 "EsterelTreeParser.cpp"
			_t = __t112;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_loop:
		{
			RefLineAST __t120 = _t;
			RefLineAST tmp29_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_loop);
			_t = _t->getFirstChild();
#line 535 "staticsemantics.g"
			Statement *s; Expression *e;
#line 1146 "EsterelTreeParser.cpp"
			s=statement(_t,c);
			_t = _retTree;
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case 3:
			{
#line 537 "staticsemantics.g"
				st = new Loop(s);
#line 1157 "EsterelTreeParser.cpp"
				break;
			}
			case DELAY:
			case ID:
			case LITERAL_or:
			case LITERAL_and:
			case LITERAL_not:
			case LITERAL_pre:
			case LITERAL_immediate:
			{
				e=delayExpression(_t,c);
				_t = _retTree;
#line 538 "staticsemantics.g"
				st = new LoopEach(s, e);
#line 1172 "EsterelTreeParser.cpp"
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t120;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_repeat:
		{
			RefLineAST __t122 = _t;
			RefLineAST tmp30_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_repeat);
			_t = _t->getFirstChild();
#line 542 "staticsemantics.g"
			bool positive = false; Expression *e;
#line 1193 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_positive:
			{
				RefLineAST tmp31_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_positive);
				_t = _t->getNextSibling();
#line 543 "staticsemantics.g"
				positive = true;
#line 1205 "EsterelTreeParser.cpp"
				break;
			}
			case CALL:
			case Integer:
			case FloatConst:
			case DoubleConst:
			case EQUALS:
			case ID:
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
			case QUESTION:
			case LITERAL_pre:
			case DQUESTION:
			case LITERAL_true:
			case LITERAL_false:
			case StringConstant:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			expr = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			e=expression(_t,c);
			_t = _retTree;
#line 545 "staticsemantics.g"
			
			if (e->type != c->integer_type)
				      throw LineError(expr,
			"repeat type is not integer: " + e->type->name);
			Statement *s;
			
#line 1252 "EsterelTreeParser.cpp"
			s=statement(_t,c);
			_t = _retTree;
#line 552 "staticsemantics.g"
			
			Counter *cntr = new Counter();
			c->module->counters.push_back(cntr);
			st = new Repeat(s, e, positive, cntr);
			
#line 1261 "EsterelTreeParser.cpp"
			_t = __t122;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_abort:
		{
			RefLineAST __t124 = _t;
			RefLineAST tmp32_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_abort);
			_t = _t->getFirstChild();
#line 559 "staticsemantics.g"
			bool isWeak = false; Statement *s;
#line 1274 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_weak:
			{
				RefLineAST tmp33_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_weak);
				_t = _t->getNextSibling();
#line 560 "staticsemantics.g"
				isWeak = true;
#line 1286 "EsterelTreeParser.cpp"
				break;
			}
			case SEQUENCE:
			case DOWATCHING:
			case DOUPTO:
			case RUN:
			case COLEQUALS:
			case PARALLEL:
			case LITERAL_nothing:
			case LITERAL_pause:
			case LITERAL_halt:
			case LITERAL_emit:
			case LITERAL_sustain:
			case LITERAL_call:
			case LITERAL_present:
			case LITERAL_if:
			case LITERAL_repeat:
			case LITERAL_abort:
			case LITERAL_await:
			case LITERAL_loop:
			case LITERAL_every:
			case LITERAL_suspend:
			case LITERAL_trap:
			case LITERAL_exit:
			case LITERAL_exec:
			case LITERAL_signal:
			case LITERAL_var:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			s=statement(_t,c);
			_t = _retTree;
#line 562 "staticsemantics.g"
			Abort *a = new Abort(s, isWeak);
#line 1327 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt129=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == CASE)) {
					RefLineAST __t127 = _t;
					RefLineAST tmp34_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CASE);
					_t = _t->getFirstChild();
#line 563 "staticsemantics.g"
					Expression *e; s = 0;
#line 1340 "EsterelTreeParser.cpp"
					e=delayExpression(_t,c);
					_t = _retTree;
					{
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					switch ( _t->getType()) {
					case SEQUENCE:
					case DOWATCHING:
					case DOUPTO:
					case RUN:
					case COLEQUALS:
					case PARALLEL:
					case LITERAL_nothing:
					case LITERAL_pause:
					case LITERAL_halt:
					case LITERAL_emit:
					case LITERAL_sustain:
					case LITERAL_call:
					case LITERAL_present:
					case LITERAL_if:
					case LITERAL_repeat:
					case LITERAL_abort:
					case LITERAL_await:
					case LITERAL_loop:
					case LITERAL_every:
					case LITERAL_suspend:
					case LITERAL_trap:
					case LITERAL_exit:
					case LITERAL_exec:
					case LITERAL_signal:
					case LITERAL_var:
					{
						s=statement(_t,c);
						_t = _retTree;
						break;
					}
					case 3:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
					}
					}
					}
					_t = __t127;
					_t = _t->getNextSibling();
#line 564 "staticsemantics.g"
					a->newCase(s, e);
#line 1391 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt129>=1 ) { goto _loop129; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt129++;
			}
			_loop129:;
			}  // ( ... )+
#line 566 "staticsemantics.g"
			st = a;
#line 1403 "EsterelTreeParser.cpp"
			_t = __t124;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_await:
		{
			RefLineAST __t130 = _t;
			RefLineAST tmp35_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_await);
			_t = _t->getFirstChild();
#line 569 "staticsemantics.g"
			Await *a = new Await();
#line 1416 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt134=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == CASE)) {
					RefLineAST __t132 = _t;
					RefLineAST tmp36_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CASE);
					_t = _t->getFirstChild();
#line 570 "staticsemantics.g"
					Expression *e; Statement *s = 0;
#line 1429 "EsterelTreeParser.cpp"
					e=delayExpression(_t,c);
					_t = _retTree;
					{
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					switch ( _t->getType()) {
					case SEQUENCE:
					case DOWATCHING:
					case DOUPTO:
					case RUN:
					case COLEQUALS:
					case PARALLEL:
					case LITERAL_nothing:
					case LITERAL_pause:
					case LITERAL_halt:
					case LITERAL_emit:
					case LITERAL_sustain:
					case LITERAL_call:
					case LITERAL_present:
					case LITERAL_if:
					case LITERAL_repeat:
					case LITERAL_abort:
					case LITERAL_await:
					case LITERAL_loop:
					case LITERAL_every:
					case LITERAL_suspend:
					case LITERAL_trap:
					case LITERAL_exit:
					case LITERAL_exec:
					case LITERAL_signal:
					case LITERAL_var:
					{
						s=statement(_t,c);
						_t = _retTree;
						break;
					}
					case 3:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
					}
					}
					}
					_t = __t132;
					_t = _t->getNextSibling();
#line 571 "staticsemantics.g"
					a->newCase(s, e);
#line 1480 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt134>=1 ) { goto _loop134; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt134++;
			}
			_loop134:;
			}  // ( ... )+
#line 573 "staticsemantics.g"
			st = a;
#line 1492 "EsterelTreeParser.cpp"
			_t = __t130;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_every:
		{
			RefLineAST __t135 = _t;
			RefLineAST tmp37_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_every);
			_t = _t->getFirstChild();
#line 576 "staticsemantics.g"
			Expression *e; Statement *s;
#line 1505 "EsterelTreeParser.cpp"
			e=delayExpression(_t,c);
			_t = _retTree;
			s=statement(_t,c);
			_t = _retTree;
#line 578 "staticsemantics.g"
			st = new Every(s, e);
#line 1512 "EsterelTreeParser.cpp"
			_t = __t135;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_suspend:
		{
			RefLineAST __t136 = _t;
			RefLineAST tmp38_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_suspend);
			_t = _t->getFirstChild();
#line 581 "staticsemantics.g"
			Expression *e; Statement *s;
#line 1525 "EsterelTreeParser.cpp"
			s=statement(_t,c);
			_t = _retTree;
			e=delayExpression(_t,c);
			_t = _retTree;
#line 583 "staticsemantics.g"
			st = new Suspend(s, e);
#line 1532 "EsterelTreeParser.cpp"
			_t = __t136;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_trap:
		{
			RefLineAST __t137 = _t;
			RefLineAST tmp39_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_trap);
			_t = _t->getFirstChild();
#line 587 "staticsemantics.g"
			
				    Trap *t = new Trap();
			// Set up a new context by copying the old and opening up
				    // a new symbol table for traps
			Context nc = *c;
			nc.traps = t->symbols = new SymbolTable();
				    t->symbols->parent = c->traps;            
			Statement *s; Expression *e;
			
#line 1553 "EsterelTreeParser.cpp"
			RefLineAST __t138 = _t;
			RefLineAST tmp40_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TRAPS);
			_t = _t->getFirstChild();
			{ // ( ... )+
			int _cnt140=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == TDECL)) {
					trapDecl(_t,&nc);
					_t = _retTree;
				}
				else {
					if ( _cnt140>=1 ) { goto _loop140; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt140++;
			}
			_loop140:;
			}  // ( ... )+
			_t = __t138;
			_t = _t->getNextSibling();
			s=statement(_t,&nc);
			_t = _retTree;
#line 598 "staticsemantics.g"
			t->body = s;
#line 1581 "EsterelTreeParser.cpp"
			{ // ( ... )*
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == LITERAL_handle)) {
					RefLineAST __t142 = _t;
					RefLineAST tmp41_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_handle);
					_t = _t->getFirstChild();
					e=trapExpression(_t,&nc);
					_t = _retTree;
					s=statement(_t,&nc);
					_t = _retTree;
#line 600 "staticsemantics.g"
					t->newHandler(e, s);
#line 1597 "EsterelTreeParser.cpp"
					_t = __t142;
					_t = _t->getNextSibling();
				}
				else {
					goto _loop143;
				}
				
			}
			_loop143:;
			} // ( ... )*
#line 603 "staticsemantics.g"
			st = t;
#line 1610 "EsterelTreeParser.cpp"
			_t = __t137;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_exit:
		{
			RefLineAST __t144 = _t;
			RefLineAST tmp42_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_exit);
			_t = _t->getFirstChild();
			etrap = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 606 "staticsemantics.g"
			Expression *e = 0;
#line 1626 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case CALL:
			case Integer:
			case FloatConst:
			case DoubleConst:
			case EQUALS:
			case ID:
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
			case QUESTION:
			case LITERAL_pre:
			case DQUESTION:
			case LITERAL_true:
			case LITERAL_false:
			case StringConstant:
			{
				texpr = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
				e=expression(_t,c);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 607 "staticsemantics.g"
			
			string name = etrap->getText();
				     if (!c->traps->contains(name))
			throw LineError(etrap, "undeclared trap " + name);
			SignalSymbol *ts = dynamic_cast<SignalSymbol*>(c->traps->get(name));
				         assert(ts);
			assert(ts->kind == SignalSymbol::Trap);
			st = new Exit(ts, e);
			if (e) {
			if (!ts->type)
			throw LineError(etrap, "returning value for pure trap " + name);
			else if (e->type != ts->type)
			throw LineError(etrap, "expression-trap type mismatch for " + name);
				     } else {
			// No experssion
			if (ts->type) 
			throw LineError(etrap, "missing expression for valued trap " + name);
			}
			
#line 1692 "EsterelTreeParser.cpp"
			_t = __t144;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_exec:
		{
			RefLineAST __t146 = _t;
			RefLineAST tmp43_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_exec);
			_t = _t->getFirstChild();
#line 628 "staticsemantics.g"
			Exec *ex = new Exec();
#line 1705 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt150=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == CASE)) {
					RefLineAST __t148 = _t;
					RefLineAST tmp44_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CASE);
					_t = _t->getFirstChild();
					taskid = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 630 "staticsemantics.g"
					
					string name = taskid->getText();
					if (!c->module->tasks->contains(name))
					throw LineError(taskid, "undeclared task " + name);
					TaskSymbol *ts = dynamic_cast<TaskSymbol*>(c->module->tasks->get(name));
					assert(ts); 
					TaskCall *tc = new TaskCall(ts);
					ex->calls.push_back(tc);
					
#line 1729 "EsterelTreeParser.cpp"
					procArgs(_t,c, tc);
					_t = _retTree;
					tsig = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 641 "staticsemantics.g"
					
					string sname = tsig->getText();
					if (!c->signals->contains(sname))
					throw LineError(tsig, "undeclared signal " + name);
					SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(sname));
					assert(ss);
					if (ss->kind != SignalSymbol::Return)
					throw LineError(tsig, "signal " + sname + " is not return");
					tc->signal = ss;
					Statement *s;
					
#line 1747 "EsterelTreeParser.cpp"
					{
					if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
						_t = ASTNULL;
					switch ( _t->getType()) {
					case SEQUENCE:
					case DOWATCHING:
					case DOUPTO:
					case RUN:
					case COLEQUALS:
					case PARALLEL:
					case LITERAL_nothing:
					case LITERAL_pause:
					case LITERAL_halt:
					case LITERAL_emit:
					case LITERAL_sustain:
					case LITERAL_call:
					case LITERAL_present:
					case LITERAL_if:
					case LITERAL_repeat:
					case LITERAL_abort:
					case LITERAL_await:
					case LITERAL_loop:
					case LITERAL_every:
					case LITERAL_suspend:
					case LITERAL_trap:
					case LITERAL_exit:
					case LITERAL_exec:
					case LITERAL_signal:
					case LITERAL_var:
					{
						s=statement(_t,c);
						_t = _retTree;
#line 652 "staticsemantics.g"
						tc->body = s;
#line 1782 "EsterelTreeParser.cpp"
						break;
					}
					case 3:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
					}
					}
					}
					_t = __t148;
					_t = _t->getNextSibling();
				}
				else {
					if ( _cnt150>=1 ) { goto _loop150; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt150++;
			}
			_loop150:;
			}  // ( ... )+
#line 655 "staticsemantics.g"
			st = ex;
#line 1808 "EsterelTreeParser.cpp"
			_t = __t146;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_var:
		{
			RefLineAST __t151 = _t;
			RefLineAST tmp45_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_var);
			_t = _t->getFirstChild();
#line 659 "staticsemantics.g"
			
			Var *v = new Var();
			Context nc = *c;
			nc.variables = v->symbols = new SymbolTable();
				         v->symbols->parent = c->variables;
			assert(nc.variables);
			TypeSymbol *ts = 0;
			
#line 1828 "EsterelTreeParser.cpp"
			RefLineAST __t152 = _t;
			RefLineAST tmp46_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),VARS);
			_t = _t->getFirstChild();
			{ // ( ... )+
			int _cnt155=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == COLON)) {
					RefLineAST __t154 = _t;
					RefLineAST tmp47_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COLON);
					_t = _t->getFirstChild();
					vdecls = _t;
					if ( _t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) ) throw ANTLR_USE_NAMESPACE(antlr)MismatchedTokenException();
					_t = _t->getNextSibling();
					ts=type(_t,c);
					_t = _retTree;
					_t = __t154;
					_t = _t->getNextSibling();
#line 669 "staticsemantics.g"
					variableDeclList(vdecls, c, &nc, ts);
#line 1852 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt155>=1 ) { goto _loop155; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt155++;
			}
			_loop155:;
			}  // ( ... )+
			_t = __t152;
			_t = _t->getNextSibling();
#line 672 "staticsemantics.g"
			Statement *s;
#line 1866 "EsterelTreeParser.cpp"
			s=statement(_t,&nc);
			_t = _retTree;
#line 674 "staticsemantics.g"
			
			v->body = s;
			st = v;
			
#line 1874 "EsterelTreeParser.cpp"
			_t = __t151;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_signal:
		{
			RefLineAST __t156 = _t;
			RefLineAST tmp48_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_signal);
			_t = _t->getFirstChild();
#line 681 "staticsemantics.g"
			
			Signal *sig = new Signal();
			Context nc = *c;
			nc.signals = sig->symbols = new SymbolTable();
			sig->symbols->parent = c->signals;
			
#line 1892 "EsterelTreeParser.cpp"
			RefLineAST __t157 = _t;
			RefLineAST tmp49_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SIGS);
			_t = _t->getFirstChild();
			{ // ( ... )+
			int _cnt159=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == SDECL)) {
					signalDecl(_t,&nc, "local", false);
					_t = _retTree;
				}
				else {
					if ( _cnt159>=1 ) { goto _loop159; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt159++;
			}
			_loop159:;
			}  // ( ... )+
			_t = __t157;
			_t = _t->getNextSibling();
#line 688 "staticsemantics.g"
			Statement *s;
#line 1918 "EsterelTreeParser.cpp"
			s=statement(_t,&nc);
			_t = _retTree;
#line 690 "staticsemantics.g"
			
			sig->body = s;
			st = sig;
			
#line 1926 "EsterelTreeParser.cpp"
			_t = __t156;
			_t = _t->getNextSibling();
			break;
		}
		case RUN:
		{
			RefLineAST __t160 = _t;
			RefLineAST tmp50_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),RUN);
			_t = _t->getFirstChild();
			module = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 696 "staticsemantics.g"
			Run *r = new Run(module->getText(), c->signals);
#line 1942 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case ID:
			{
				oldMod = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
				_t = _t->getNextSibling();
#line 697 "staticsemantics.g"
				r->old_name = oldMod->getText();
#line 1954 "EsterelTreeParser.cpp"
				break;
			}
			case 3:
			case TRENAME:
			case CRENAME:
			case SRENAME:
			case TARENAME:
			case FRENAME:
			case PRENAME:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			{ // ( ... )*
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				switch ( _t->getType()) {
				case TRENAME:
				{
					RefLineAST __t163 = _t;
					RefLineAST tmp51_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TRENAME);
					_t = _t->getFirstChild();
#line 698 "staticsemantics.g"
					TypeSymbol *t;
#line 1986 "EsterelTreeParser.cpp"
					t=type(_t,c);
					_t = _retTree;
					tID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
					_t = __t163;
					_t = _t->getNextSibling();
#line 699 "staticsemantics.g"
					r->types.push_back(new TypeRenaming(tID->getText(), t));
#line 1996 "EsterelTreeParser.cpp"
					break;
				}
				case CRENAME:
				{
					RefLineAST __t164 = _t;
					RefLineAST tmp52_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CRENAME);
					_t = _t->getFirstChild();
#line 700 "staticsemantics.g"
					Expression *e;
#line 2007 "EsterelTreeParser.cpp"
					e=expression(_t,c);
					_t = _retTree;
					cID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
					_t = __t164;
					_t = _t->getNextSibling();
#line 701 "staticsemantics.g"
					r->constants.
					push_back(new ConstantRenaming(cID->getText(), e));
#line 2018 "EsterelTreeParser.cpp"
					break;
				}
				case FRENAME:
				{
					RefLineAST __t165 = _t;
					RefLineAST tmp53_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FRENAME);
					_t = _t->getFirstChild();
					func = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
					funcName(_t);
					_t = _retTree;
					fID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 704 "staticsemantics.g"
					
					string name = func->getText();
					if (!c->module->functions->contains(name))
					throw LineError(func, "undeclared function " + name);
					FunctionSymbol *fs =
					dynamic_cast<FunctionSymbol*>(c->module->functions->get(name));
					assert(fs);
					r->functions.push_back(new FunctionRenaming(fID->getText(), fs));
					
#line 2043 "EsterelTreeParser.cpp"
					_t = __t165;
					_t = _t->getNextSibling();
					break;
				}
				case PRENAME:
				{
					RefLineAST __t166 = _t;
					RefLineAST tmp54_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PRENAME);
					_t = _t->getFirstChild();
					pro = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
					pID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 715 "staticsemantics.g"
					
					string name = pro->getText();
					if (!c->module->procedures->contains(name))
					throw LineError(pro, "undeclared procedure " + name);
					ProcedureSymbol *ps =
					dynamic_cast<ProcedureSymbol*>(c->module->procedures->get(name));
					assert(ps);
					r->procedures.push_back(new ProcedureRenaming(pID->getText(),
					ps));
					
#line 2071 "EsterelTreeParser.cpp"
					_t = __t166;
					_t = _t->getNextSibling();
					break;
				}
				case TARENAME:
				{
					RefLineAST __t167 = _t;
					RefLineAST tmp55_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TARENAME);
					_t = _t->getFirstChild();
					task = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
					trID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 727 "staticsemantics.g"
					
					string name = task->getText();
					if (!c->module->tasks->contains(name))
					throw LineError(task, "undeclared tasks " + name);
					ProcedureSymbol *ps =
					dynamic_cast<ProcedureSymbol*>(c->module->tasks->get(name));
					assert(ps);
					r->tasks.push_back(new ProcedureRenaming(trID->getText(),
					ps));
					
#line 2099 "EsterelTreeParser.cpp"
					_t = __t167;
					_t = _t->getNextSibling();
					break;
				}
				case SRENAME:
				{
					RefLineAST __t168 = _t;
					RefLineAST tmp56_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SRENAME);
					_t = _t->getFirstChild();
					sig = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
					sID = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 739 "staticsemantics.g"
					
					string name = sig->getText();
					if (!c->signals->contains(name))
					throw LineError(sig, "undeclared signal " + name);
					SignalSymbol *ss =
					dynamic_cast<SignalSymbol*>(c->signals->get(name));
					assert(ss);
					r->signals.push_back(new SignalRenaming(sID->getText(), ss));
					
#line 2126 "EsterelTreeParser.cpp"
					_t = __t168;
					_t = _t->getNextSibling();
					break;
				}
				default:
				{
					goto _loop169;
				}
				}
			}
			_loop169:;
			} // ( ... )*
#line 750 "staticsemantics.g"
			st = r;
#line 2141 "EsterelTreeParser.cpp"
			_t = __t160;
			_t = _t->getNextSibling();
			break;
		}
		case DOWATCHING:
		{
			RefLineAST __t170 = _t;
			RefLineAST tmp57_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DOWATCHING);
			_t = _t->getFirstChild();
#line 753 "staticsemantics.g"
			Expression *e; Statement *s1, *s2 = 0;
#line 2154 "EsterelTreeParser.cpp"
			s1=statement(_t,c);
			_t = _retTree;
			de = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			e=delayExpression(_t,c);
			_t = _retTree;
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case SEQUENCE:
			case DOWATCHING:
			case DOUPTO:
			case RUN:
			case COLEQUALS:
			case PARALLEL:
			case LITERAL_nothing:
			case LITERAL_pause:
			case LITERAL_halt:
			case LITERAL_emit:
			case LITERAL_sustain:
			case LITERAL_call:
			case LITERAL_present:
			case LITERAL_if:
			case LITERAL_repeat:
			case LITERAL_abort:
			case LITERAL_await:
			case LITERAL_loop:
			case LITERAL_every:
			case LITERAL_suspend:
			case LITERAL_trap:
			case LITERAL_exit:
			case LITERAL_exec:
			case LITERAL_signal:
			case LITERAL_var:
			{
				s2=statement(_t,c);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
#line 757 "staticsemantics.g"
			st = new DoWatching( s1, e, s2);
#line 2206 "EsterelTreeParser.cpp"
			_t = __t170;
			_t = _t->getNextSibling();
			break;
		}
		case DOUPTO:
		{
			RefLineAST __t172 = _t;
			RefLineAST tmp58_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DOUPTO);
			_t = _t->getFirstChild();
#line 760 "staticsemantics.g"
			Expression *e; Statement *s;
#line 2219 "EsterelTreeParser.cpp"
			s=statement(_t,c);
			_t = _retTree;
			e=delayExpression(_t,c);
			_t = _retTree;
#line 762 "staticsemantics.g"
			st = new DoUpto(s, e);
#line 2226 "EsterelTreeParser.cpp"
			_t = __t172;
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 765 "staticsemantics.g"
		
		assert(st);
		
#line 2241 "EsterelTreeParser.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return st;
}

void EsterelTreeParser::typeDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST typeDecl_AST_in = _t;
	RefLineAST typeName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		typeName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 211 "staticsemantics.g"
		
		string name = typeName->getText();
		if (c->module->types->local_contains(name))
		throw LineError(typeName, "redeclaration of type " + name);
		c->module->types->enter(new TypeSymbol(name) );
		
#line 2269 "EsterelTreeParser.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::constantDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST constantDecl_AST_in = _t;
	RefLineAST constantName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST expr = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST typeToken = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST ids = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 219 "staticsemantics.g"
	
	Expression *e = 0;
	TypeSymbol *t;
	
#line 2292 "EsterelTreeParser.cpp"
	
	try {      // for error handling
		RefLineAST __t45 = _t;
		RefLineAST tmp59_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CDECL);
		_t = _t->getFirstChild();
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ID:
		{
			constantName = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 226 "staticsemantics.g"
			string name = constantName->getText();
#line 2310 "EsterelTreeParser.cpp"
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case EQUALS:
			{
				RefLineAST __t48 = _t;
				RefLineAST tmp60_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EQUALS);
				_t = _t->getFirstChild();
				expr = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
				e=expression(_t,c);
				_t = _retTree;
				_t = __t48;
				_t = _t->getNextSibling();
				break;
			}
			case ID:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			typeToken = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			t=type(_t,c);
			_t = _retTree;
#line 229 "staticsemantics.g"
			create_constant(constantName, c, name, e, t);
#line 2343 "EsterelTreeParser.cpp"
			break;
		}
		case COMMA:
		{
			RefLineAST __t49 = _t;
			ids = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COMMA);
			_t = _t->getFirstChild();
			{ // ( ... )+
			int _cnt51=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == ID)) {
					RefLineAST tmp61_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
				}
				else {
					if ( _cnt51>=1 ) { goto _loop51; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt51++;
			}
			_loop51:;
			}  // ( ... )+
			_t = __t49;
			_t = _t->getNextSibling();
			t=type(_t,c);
			_t = _retTree;
#line 232 "staticsemantics.g"
			
			for ( RefLineAST id = ids->getFirstChild() ; id ;
			id=id->getNextSibling() )
			create_constant(ids, c, id->getText(), 0, t);
			
#line 2380 "EsterelTreeParser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		_t = __t45;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::signalDecl(RefLineAST _t,
	Context *c, string direction, bool isGlobal
) {
	RefLineAST signalDecl_AST_in = _t;
	RefLineAST signalName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST expr = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST typeToken = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST func = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST pcf = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t53 = _t;
		RefLineAST tmp62_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SDECL);
		_t = _t->getFirstChild();
		signalName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 243 "staticsemantics.g"
		
			 string name = signalName->getText();
			 if (c->signals->local_contains(name))
			   throw LineError(signalName, "Redeclaration of " + name);
			 Expression *e = 0;
		
#line 2425 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case COLEQUALS:
		{
			RefLineAST __t55 = _t;
			RefLineAST tmp63_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COLEQUALS);
			_t = _t->getFirstChild();
			expr = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			e=expression(_t,c);
			_t = _retTree;
			_t = __t55;
			_t = _t->getNextSibling();
			break;
		}
		case 3:
		case ID:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 250 "staticsemantics.g"
		TypeSymbol *t = 0; FunctionSymbol *fs = 0;
#line 2456 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ID:
		{
			typeToken = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			t=type(_t,c);
			_t = _retTree;
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case ID:
			{
				func = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
				_t = _t->getNextSibling();
#line 253 "staticsemantics.g"
				
					         string name = func->getText();
				if (!c->module->functions->local_contains(name))
					           throw LineError(func, "Undeclared function " + name);
					     Symbol *sym = c->module->functions->get(name);
					         fs = dynamic_cast<FunctionSymbol*>(sym);
					         assert(fs);
					
#line 2484 "EsterelTreeParser.cpp"
				break;
			}
			case PLUS:
			case STAR:
			case LITERAL_or:
			case LITERAL_and:
			{
				pcf = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
				predefinedCombineFunction(_t);
				_t = _retTree;
#line 262 "staticsemantics.g"
				
				string name = pcf->getText();
				assert(c->module->functions->contains(name));
				Symbol *sym = c->module->functions->get(name);
				fs = dynamic_cast<BuiltinFunctionSymbol*>(sym);
					         assert(fs);
				
#line 2503 "EsterelTreeParser.cpp"
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 271 "staticsemantics.g"
		
		SignalSymbol::kinds kind =
		direction == "input" ? SignalSymbol::Input :
		direction == "output" ? SignalSymbol::Output :
		direction == "inputoutput" ? SignalSymbol::Inputoutput :
		direction == "sensor" ? SignalSymbol::Sensor :
		direction == "return" ? SignalSymbol::Return :
		direction == "local" ? SignalSymbol::Local :
		SignalSymbol::Unknown;
		
		assert(kind != SignalSymbol::Unknown);
		
		c->signals->enter(new SignalSymbol(name, t, kind, fs, e, NULL));
			      if (e && (e->type != t))
			        throw LineError(signalName, 
		"initializer does not match type of signal");
		
#line 2546 "EsterelTreeParser.cpp"
		_t = __t53;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::functionDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST functionDecl_AST_in = _t;
	RefLineAST functionName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t65 = _t;
		RefLineAST tmp64_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FDECL);
		_t = _t->getFirstChild();
		functionName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 322 "staticsemantics.g"
		
			 string name = functionName->getText();
			 if (c->module->functions->contains(name))
			   throw LineError(functionName, "redeclaration of function " + name);
		FunctionSymbol *fs = new FunctionSymbol(name);
		c->module->functions->enter(fs);	 
		TypeSymbol *ts;
		
#line 2581 "EsterelTreeParser.cpp"
		RefLineAST __t66 = _t;
		RefLineAST tmp65_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPES);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				ts=type(_t,c);
				_t = _retTree;
#line 330 "staticsemantics.g"
				fs->arguments.push_back(ts);
#line 2595 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop68;
			}
			
		}
		_loop68:;
		} // ( ... )*
		_t = __t66;
		_t = _t->getNextSibling();
		ts=type(_t,c);
		_t = _retTree;
#line 331 "staticsemantics.g"
		fs->result = ts;
#line 2610 "EsterelTreeParser.cpp"
		_t = __t65;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::procedureDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST procedureDecl_AST_in = _t;
	RefLineAST procedureName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t70 = _t;
		RefLineAST tmp66_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PDECL);
		_t = _t->getFirstChild();
		procedureName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 337 "staticsemantics.g"
		
			 string name = procedureName->getText();
			 if (c->module->procedures->contains(name))
			   throw LineError(procedureName,
		"redeclaration of procedure " + name);
		ProcedureSymbol *ps = new ProcedureSymbol(name);
		c->module->procedures->enter(ps);	 
		TypeSymbol *ts;
		
#line 2646 "EsterelTreeParser.cpp"
		RefLineAST __t71 = _t;
		RefLineAST tmp67_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPES);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				ts=type(_t,c);
				_t = _retTree;
#line 346 "staticsemantics.g"
				ps->reference_arguments.push_back(ts);
#line 2660 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop73;
			}
			
		}
		_loop73:;
		} // ( ... )*
		_t = __t71;
		_t = _t->getNextSibling();
		RefLineAST __t74 = _t;
		RefLineAST tmp68_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPES);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				ts=type(_t,c);
				_t = _retTree;
#line 347 "staticsemantics.g"
				ps->value_arguments.push_back(ts);
#line 2684 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop76;
			}
			
		}
		_loop76:;
		} // ( ... )*
		_t = __t74;
		_t = _t->getNextSibling();
		_t = __t70;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::taskDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST taskDecl_AST_in = _t;
	RefLineAST taskName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t78 = _t;
		RefLineAST tmp69_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TADECL);
		_t = _t->getFirstChild();
		taskName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 353 "staticsemantics.g"
		
			 string name = taskName->getText();
			 if (c->module->tasks->contains(name))
			   throw LineError(taskName, "redeclaration of task " + name);
		TaskSymbol *ps = new TaskSymbol(name);
		c->module->tasks->enter(ps);	 
		TypeSymbol *ts;
		
#line 2729 "EsterelTreeParser.cpp"
		RefLineAST __t79 = _t;
		RefLineAST tmp70_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPES);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				ts=type(_t,c);
				_t = _retTree;
#line 361 "staticsemantics.g"
				ps->reference_arguments.push_back(ts);
#line 2743 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop81;
			}
			
		}
		_loop81:;
		} // ( ... )*
		_t = __t79;
		_t = _t->getNextSibling();
		RefLineAST __t82 = _t;
		RefLineAST tmp71_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPES);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				ts=type(_t,c);
				_t = _retTree;
#line 362 "staticsemantics.g"
				ps->value_arguments.push_back(ts);
#line 2767 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop84;
			}
			
		}
		_loop84:;
		} // ( ... )*
		_t = __t82;
		_t = _t->getNextSibling();
		_t = __t78;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::relationDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST relationDecl_AST_in = _t;
	RefLineAST sig1 = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST sig2 = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST sig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IMPLIES:
		{
			RefLineAST __t86 = _t;
			RefLineAST tmp72_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IMPLIES);
			_t = _t->getFirstChild();
			sig1 = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			sig2 = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			_t = __t86;
			_t = _t->getNextSibling();
#line 368 "staticsemantics.g"
			
			string s1 = sig1->getText();
			if (!c->signals->contains(s1))
			throw LineError(sig1, "undeclared signal " + s1);
			string s2 = sig2->getText();
			if (!c->signals->contains(s2))
			throw LineError(sig2, "undeclared signal " + s2);
			SignalSymbol *ss1 = dynamic_cast<SignalSymbol*>(c->signals->get(s1));
			assert(ss1);
			SignalSymbol *ss2 = dynamic_cast<SignalSymbol*>(c->signals->get(s2));
			assert(ss2);
			c->module->relations.push_back(new Implication(ss1, ss2));
			
#line 2829 "EsterelTreeParser.cpp"
			break;
		}
		case POUND:
		{
			RefLineAST __t87 = _t;
			RefLineAST tmp73_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),POUND);
			_t = _t->getFirstChild();
#line 382 "staticsemantics.g"
			
			Exclusion *ex = new Exclusion();
			c->module->relations.push_back(ex);
			
#line 2843 "EsterelTreeParser.cpp"
			{ // ( ... )+
			int _cnt89=0;
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType() == ID)) {
					sig = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
					_t = _t->getNextSibling();
#line 387 "staticsemantics.g"
					
					string name = sig->getText();
					if (!c->signals->contains(name))
					throw LineError(sig, "undeclared signal " + name);
					SignalSymbol *ss =
					dynamic_cast<SignalSymbol*>(c->signals->get(name));
					assert(ss);
						   ex->signals.push_back(ss);
					
#line 2863 "EsterelTreeParser.cpp"
				}
				else {
					if ( _cnt89>=1 ) { goto _loop89; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
				}
				
				_cnt89++;
			}
			_loop89:;
			}  // ( ... )+
			_t = __t87;
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

Expression * EsterelTreeParser::expression(RefLineAST _t,
	Context *c
) {
#line 829 "staticsemantics.g"
	Expression *e;
#line 2896 "EsterelTreeParser.cpp"
	RefLineAST expression_AST_in = _t;
	RefLineAST sig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST trap = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST func = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 831 "staticsemantics.g"
		
		Expression *e1 = 0, *e2 = 0; // for safety
		e = 0; // for safety
		
#line 2908 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case Integer:
		{
			RefLineAST tmp74_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),Integer);
			_t = _t->getNextSibling();
#line 836 "staticsemantics.g"
			e = new Literal(expression_AST_in->getText(), c->integer_type);
#line 2920 "EsterelTreeParser.cpp"
			break;
		}
		case FloatConst:
		{
			RefLineAST tmp75_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FloatConst);
			_t = _t->getNextSibling();
#line 837 "staticsemantics.g"
			e = new Literal(expression_AST_in->getText(), c->float_type);
#line 2930 "EsterelTreeParser.cpp"
			break;
		}
		case DoubleConst:
		{
			RefLineAST tmp76_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DoubleConst);
			_t = _t->getNextSibling();
#line 838 "staticsemantics.g"
			e = new Literal(expression_AST_in->getText(), c->double_type);
#line 2940 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_true:
		{
			RefLineAST tmp77_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_true);
			_t = _t->getNextSibling();
#line 839 "staticsemantics.g"
			e = new LoadVariableExpression(c->true_constant);
#line 2950 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_false:
		{
			RefLineAST tmp78_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_false);
			_t = _t->getNextSibling();
#line 840 "staticsemantics.g"
			e = new LoadVariableExpression(c->false_constant);
#line 2960 "EsterelTreeParser.cpp"
			break;
		}
		case StringConstant:
		{
			RefLineAST tmp79_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),StringConstant);
			_t = _t->getNextSibling();
#line 841 "staticsemantics.g"
			e = new Literal(expression_AST_in->getText(), c->string_type);
#line 2970 "EsterelTreeParser.cpp"
			break;
		}
		case ID:
		{
			RefLineAST tmp80_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 843 "staticsemantics.g"
			
			string name = expression_AST_in->getText();
			if (!c->variables->contains(name))
			throw LineError(expression_AST_in, "undeclared variable " + name);
			Symbol *s = c->variables->get(name);
			assert(s);
			VariableSymbol *vs = dynamic_cast<VariableSymbol*>(s);
			assert(vs);
			e = new LoadVariableExpression(vs);
			
#line 2989 "EsterelTreeParser.cpp"
			break;
		}
		case QUESTION:
		{
			RefLineAST __t190 = _t;
			RefLineAST tmp81_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),QUESTION);
			_t = _t->getFirstChild();
			sig = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			_t = __t190;
			_t = _t->getNextSibling();
#line 854 "staticsemantics.g"
			
			string name = sig->getText();
			if (!c->signals->contains(name))
			throw LineError(sig, "unknown signal " + name);
			SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(name));
			assert(ss);
			if (!ss->type)
			throw LineError(sig, "attempting to take value of pure signal " + name);
			e = new LoadSignalValueExpression(ss);
			
#line 3014 "EsterelTreeParser.cpp"
			break;
		}
		case DQUESTION:
		{
			RefLineAST __t191 = _t;
			RefLineAST tmp82_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DQUESTION);
			_t = _t->getFirstChild();
			trap = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			_t = __t191;
			_t = _t->getNextSibling();
#line 865 "staticsemantics.g"
			
			string name = trap->getText();
			if (!c->traps->contains(name))
			throw LineError(trap, "unknown trap " + name);
			SignalSymbol *ts = dynamic_cast<SignalSymbol*>(c->traps->get(name));
			assert(ts);
			if (!ts->type)
			throw LineError(trap, "attempting to take value of pure trap " + name);
			e = new LoadSignalValueExpression(ts);
			
#line 3039 "EsterelTreeParser.cpp"
			break;
		}
		case CALL:
		{
			RefLineAST __t192 = _t;
			RefLineAST tmp83_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CALL);
			_t = _t->getFirstChild();
			func = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 876 "staticsemantics.g"
			
				 string name = func->getText();
				 if (!c->module->functions->contains(name))
				   throw LineError(func, "undeclared function " + name);
			FunctionSymbol *fs =
			dynamic_cast<FunctionSymbol*>(c->module->functions->get(name));
			assert(fs);
			FunctionCall *fc = new FunctionCall(fs);
			
#line 3061 "EsterelTreeParser.cpp"
			{ // ( ... )*
			for (;;) {
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_tokenSet_1.member(_t->getType()))) {
					e1=expression(_t,c);
					_t = _retTree;
#line 885 "staticsemantics.g"
					fc->arguments.push_back(e1);
#line 3071 "EsterelTreeParser.cpp"
				}
				else {
					goto _loop194;
				}
				
			}
			_loop194:;
			} // ( ... )*
#line 886 "staticsemantics.g"
			
				 if ( fc->arguments.size() != fs->arguments.size() )
			throw LineError(func, "argument count wrong for call to " + name);
			for ( size_t i = 0 ; i < fc->arguments.size() ; i++ )
			if (fc->arguments[i]->type != fs->arguments[i])
			throw LineError(func, "mismatched argument type");
			e = fc;         
			
#line 3089 "EsterelTreeParser.cpp"
			_t = __t192;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_or:
		{
			RefLineAST __t195 = _t;
			RefLineAST tmp84_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_or);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t195;
			_t = _t->getNextSibling();
#line 897 "staticsemantics.g"
			e = boolean_binop(expression_AST_in, c, "or", e1, e2);
#line 3108 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_and:
		{
			RefLineAST __t196 = _t;
			RefLineAST tmp85_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_and);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t196;
			_t = _t->getNextSibling();
#line 899 "staticsemantics.g"
			e = boolean_binop(expression_AST_in, c, "and", e1, e2);
#line 3125 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_not:
		{
			RefLineAST __t197 = _t;
			RefLineAST tmp86_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_not);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			_t = __t197;
			_t = _t->getNextSibling();
#line 901 "staticsemantics.g"
			
			if (e1->type != c->boolean_type)
			throw LineError(expression_AST_in, "argument of not must be boolean");
			e = new UnaryOp(c->boolean_type, "not", e1);
			
#line 3144 "EsterelTreeParser.cpp"
			break;
		}
		case PLUS:
		{
			RefLineAST __t198 = _t;
			RefLineAST tmp87_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PLUS);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t198;
			_t = _t->getNextSibling();
#line 908 "staticsemantics.g"
			e = numeric_binop(expression_AST_in, c, "+", e1, e2);
#line 3161 "EsterelTreeParser.cpp"
			break;
		}
		case DASH:
		{
			RefLineAST __t199 = _t;
			RefLineAST tmp88_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DASH);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			{
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case 3:
			{
#line 911 "staticsemantics.g"
				
				if (e1->type != c->integer_type &&
				e1->type != c->float_type &&
					       e1->type != c->double_type)
				throw LineError(expression_AST_in,
				"argument of unary - must be numeric, not " + e1->type->name);
				e = new UnaryOp(e1->type, "-", e1);
				
#line 3187 "EsterelTreeParser.cpp"
				break;
			}
			case CALL:
			case Integer:
			case FloatConst:
			case DoubleConst:
			case EQUALS:
			case ID:
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
			case QUESTION:
			case LITERAL_pre:
			case DQUESTION:
			case LITERAL_true:
			case LITERAL_false:
			case StringConstant:
			{
				e2=expression(_t,c);
				_t = _retTree;
#line 919 "staticsemantics.g"
				e = numeric_binop(expression_AST_in, c, "-", e1, e2);
#line 3220 "EsterelTreeParser.cpp"
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t199;
			_t = _t->getNextSibling();
			break;
		}
		case STAR:
		{
			RefLineAST __t201 = _t;
			RefLineAST tmp89_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t201;
			_t = _t->getNextSibling();
#line 923 "staticsemantics.g"
			e = numeric_binop(expression_AST_in, c, "*", e1, e2);
#line 3247 "EsterelTreeParser.cpp"
			break;
		}
		case SLASH:
		{
			RefLineAST __t202 = _t;
			RefLineAST tmp90_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SLASH);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t202;
			_t = _t->getNextSibling();
#line 925 "staticsemantics.g"
			e = numeric_binop(expression_AST_in, c, "/", e1, e2);
#line 3264 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_mod:
		{
			RefLineAST __t203 = _t;
			RefLineAST tmp91_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_mod);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t203;
			_t = _t->getNextSibling();
#line 927 "staticsemantics.g"
			e = numeric_binop(expression_AST_in, c, "mod", e1, e2);
#line 3281 "EsterelTreeParser.cpp"
			break;
		}
		case EQUALS:
		{
			RefLineAST __t204 = _t;
			RefLineAST tmp92_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EQUALS);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t204;
			_t = _t->getNextSibling();
#line 930 "staticsemantics.g"
			e = equality(expression_AST_in, c, "=", e1, e2);
#line 3298 "EsterelTreeParser.cpp"
			break;
		}
		case NEQUAL:
		{
			RefLineAST __t205 = _t;
			RefLineAST tmp93_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),NEQUAL);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t205;
			_t = _t->getNextSibling();
#line 932 "staticsemantics.g"
			e = equality(expression_AST_in, c, "<>", e1, e2);
#line 3315 "EsterelTreeParser.cpp"
			break;
		}
		case LESSTHAN:
		{
			RefLineAST __t206 = _t;
			RefLineAST tmp94_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LESSTHAN);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t206;
			_t = _t->getNextSibling();
#line 935 "staticsemantics.g"
			e = comparison(expression_AST_in, c, "<", e1, e2);
#line 3332 "EsterelTreeParser.cpp"
			break;
		}
		case LEQUAL:
		{
			RefLineAST __t207 = _t;
			RefLineAST tmp95_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LEQUAL);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t207;
			_t = _t->getNextSibling();
#line 937 "staticsemantics.g"
			e = comparison(expression_AST_in, c, "<=", e1, e2);
#line 3349 "EsterelTreeParser.cpp"
			break;
		}
		case GREATERTHAN:
		{
			RefLineAST __t208 = _t;
			RefLineAST tmp96_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GREATERTHAN);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t208;
			_t = _t->getNextSibling();
#line 939 "staticsemantics.g"
			e = comparison(expression_AST_in, c, ">", e1, e2);
#line 3366 "EsterelTreeParser.cpp"
			break;
		}
		case GEQUAL:
		{
			RefLineAST __t209 = _t;
			RefLineAST tmp97_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GEQUAL);
			_t = _t->getFirstChild();
			e1=expression(_t,c);
			_t = _retTree;
			e2=expression(_t,c);
			_t = _retTree;
			_t = __t209;
			_t = _t->getNextSibling();
#line 941 "staticsemantics.g"
			e = comparison(expression_AST_in, c, ">=", e1, e2);
#line 3383 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_pre:
		{
			RefLineAST tmp98_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_pre);
			_t = _t->getNextSibling();
#line 943 "staticsemantics.g"
			throw LineError(expression_AST_in, "pre not supported");
#line 3393 "EsterelTreeParser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 945 "staticsemantics.g"
		
		assert(e);
		assert(e->type);
		
#line 3407 "EsterelTreeParser.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return e;
}

TypeSymbol * EsterelTreeParser::type(RefLineAST _t,
	Context *c
) {
#line 400 "staticsemantics.g"
	TypeSymbol *t;
#line 3423 "EsterelTreeParser.cpp"
	RefLineAST type_AST_in = _t;
	RefLineAST type = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 401 "staticsemantics.g"
		t = 0;
#line 3430 "EsterelTreeParser.cpp"
		type = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 403 "staticsemantics.g"
		
		string name = type->getText();
		if (!c->module->types->local_contains(name))
		throw LineError(type, "Unknown type " + name);
		t = dynamic_cast<TypeSymbol*>(c->module->types->get(name));
		assert(t);
		// std::cout << "Type " << t->name << std::endl;
		
#line 3443 "EsterelTreeParser.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return t;
}

void EsterelTreeParser::predefinedCombineFunction(RefLineAST _t) {
	RefLineAST predefinedCombineFunction_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case PLUS:
		{
			RefLineAST tmp99_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PLUS);
			_t = _t->getNextSibling();
			break;
		}
		case STAR:
		{
			RefLineAST tmp100_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_or:
		{
			RefLineAST tmp101_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_or);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_and:
		{
			RefLineAST tmp102_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_and);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::trapDecl(RefLineAST _t,
	Context *c
) {
	RefLineAST trapDecl_AST_in = _t;
	RefLineAST trapName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t60 = _t;
		RefLineAST tmp103_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TDECL);
		_t = _t->getFirstChild();
		trapName = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
		_t = _t->getNextSibling();
#line 300 "staticsemantics.g"
		
		string name = trapName->getText();
		if (c->traps->local_contains(name))
		throw LineError(trapName, "redeclaration of trap " + name);
		Expression *e = 0;
		
#line 3524 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case COLEQUALS:
		{
			RefLineAST __t62 = _t;
			RefLineAST tmp104_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COLEQUALS);
			_t = _t->getFirstChild();
			e=expression(_t,c);
			_t = _retTree;
			_t = __t62;
			_t = _t->getNextSibling();
			break;
		}
		case 3:
		case ID:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 307 "staticsemantics.g"
		TypeSymbol *ts = NULL;
#line 3554 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ID:
		{
			ts=type(_t,c);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 309 "staticsemantics.g"
		
		c->traps->enter(new SignalSymbol(name, ts, SignalSymbol::Trap,
		NULL, e, NULL));
		if (e && e->type != ts)
		throw LineError(trapName,
		"initializer does not match type of trap " + name);
		
#line 3583 "EsterelTreeParser.cpp"
		_t = __t60;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::procArgs(RefLineAST _t,
	Context *c, ProcedureCall *pc
) {
	RefLineAST procArgs_AST_in = _t;
	RefLineAST var = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t182 = _t;
		RefLineAST tmp105_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),VARS);
		_t = _t->getFirstChild();
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ID)) {
				var = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
				_t = _t->getNextSibling();
#line 813 "staticsemantics.g"
				
				string name = var->getText();
				if (!c->variables->contains(name))
				throw LineError(var, "undefined variable " + name);
				VariableSymbol *vs =
				dynamic_cast<VariableSymbol*>(c->variables->get(name));
				assert(vs);
				pc->reference_args.push_back(vs);
				
#line 3624 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop184;
			}
			
		}
		_loop184:;
		} // ( ... )*
		_t = __t182;
		_t = _t->getNextSibling();
		RefLineAST __t185 = _t;
		RefLineAST tmp106_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ARGS);
		_t = _t->getFirstChild();
#line 824 "staticsemantics.g"
		Expression *e;
#line 3641 "EsterelTreeParser.cpp"
		{ // ( ... )*
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				e=expression(_t,c);
				_t = _retTree;
#line 825 "staticsemantics.g"
				pc->value_args.push_back(e);
#line 3651 "EsterelTreeParser.cpp"
			}
			else {
				goto _loop187;
			}
			
		}
		_loop187:;
		} // ( ... )*
		_t = __t185;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

Expression * EsterelTreeParser::sigExpression(RefLineAST _t,
	Context *c
) {
#line 951 "staticsemantics.g"
	Expression *e;
#line 3676 "EsterelTreeParser.cpp"
	RefLineAST sigExpression_AST_in = _t;
	RefLineAST sig = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 952 "staticsemantics.g"
		Expression *e1, *e2; e = 0;
#line 3683 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_and:
		{
			RefLineAST __t212 = _t;
			RefLineAST tmp107_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_and);
			_t = _t->getFirstChild();
			e1=sigExpression(_t,c);
			_t = _retTree;
			e2=sigExpression(_t,c);
			_t = _retTree;
			_t = __t212;
			_t = _t->getNextSibling();
#line 954 "staticsemantics.g"
			e = new BinaryOp(c->boolean_type, "and", e1, e2);
#line 3702 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_or:
		{
			RefLineAST __t213 = _t;
			RefLineAST tmp108_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_or);
			_t = _t->getFirstChild();
			e1=sigExpression(_t,c);
			_t = _retTree;
			e2=sigExpression(_t,c);
			_t = _retTree;
			_t = __t213;
			_t = _t->getNextSibling();
#line 956 "staticsemantics.g"
			e = new BinaryOp(c->boolean_type, "or", e1, e2);
#line 3719 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_not:
		{
			RefLineAST __t214 = _t;
			RefLineAST tmp109_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_not);
			_t = _t->getFirstChild();
			e1=sigExpression(_t,c);
			_t = _retTree;
			_t = __t214;
			_t = _t->getNextSibling();
#line 958 "staticsemantics.g"
			e = new UnaryOp(c->boolean_type, "not", e1);
#line 3734 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_pre:
		{
			RefLineAST tmp110_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_pre);
			_t = _t->getNextSibling();
#line 960 "staticsemantics.g"
			throw LineError(sigExpression_AST_in, "pre not supported");
#line 3744 "EsterelTreeParser.cpp"
			break;
		}
		case ID:
		{
			sig = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 962 "staticsemantics.g"
			
			string name = sig->getText();
			if (!c->signals->contains(name))
			throw LineError(sig, "unrecognized signal " + name);
			SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(name));
			e = new LoadSignalExpression(c->boolean_type, ss);
			
#line 3760 "EsterelTreeParser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return e;
}

Expression * EsterelTreeParser::delayExpression(RefLineAST _t,
	Context *c
) {
#line 972 "staticsemantics.g"
	Expression *e;
#line 3784 "EsterelTreeParser.cpp"
	RefLineAST delayExpression_AST_in = _t;
	RefLineAST expr2 = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 973 "staticsemantics.g"
		Expression *e1, *e2; e = 0;
#line 3791 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ID:
		case LITERAL_or:
		case LITERAL_and:
		case LITERAL_not:
		case LITERAL_pre:
		{
			e=sigExpression(_t,c);
			_t = _retTree;
			break;
		}
		case LITERAL_immediate:
		{
			RefLineAST __t217 = _t;
			RefLineAST tmp111_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_immediate);
			_t = _t->getFirstChild();
			e1=sigExpression(_t,c);
			_t = _retTree;
			_t = __t217;
			_t = _t->getNextSibling();
#line 976 "staticsemantics.g"
			e = new Delay( c->boolean_type, e1, 0, true, 0 );
#line 3818 "EsterelTreeParser.cpp"
			break;
		}
		case DELAY:
		{
			RefLineAST __t218 = _t;
			RefLineAST tmp112_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DELAY);
			_t = _t->getFirstChild();
			expr2 = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			e2=expression(_t,c);
			_t = _retTree;
			e1=sigExpression(_t,c);
			_t = _retTree;
			_t = __t218;
			_t = _t->getNextSibling();
#line 978 "staticsemantics.g"
			
			Counter *cntr = new Counter();
			assert(cntr);
			c->module->counters.push_back(cntr);
			e = new Delay( c->boolean_type, e1, e2, false, cntr );
			if (e2->type != c->integer_type)
			throw LineError(expr2, "delay count must be integer");
			
#line 3843 "EsterelTreeParser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return e;
}

Expression * EsterelTreeParser::trapExpression(RefLineAST _t,
	Context *c
) {
#line 989 "staticsemantics.g"
	Expression *e;
#line 3867 "EsterelTreeParser.cpp"
	RefLineAST trapExpression_AST_in = _t;
	RefLineAST trap = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
#line 990 "staticsemantics.g"
		Expression *e1, *e2; e = 0;
#line 3874 "EsterelTreeParser.cpp"
		{
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_and:
		{
			RefLineAST __t221 = _t;
			RefLineAST tmp113_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_and);
			_t = _t->getFirstChild();
			e1=trapExpression(_t,c);
			_t = _retTree;
			e2=trapExpression(_t,c);
			_t = _retTree;
			_t = __t221;
			_t = _t->getNextSibling();
#line 992 "staticsemantics.g"
			e = new BinaryOp(c->boolean_type, "and", e1, e2);
#line 3893 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_or:
		{
			RefLineAST __t222 = _t;
			RefLineAST tmp114_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_or);
			_t = _t->getFirstChild();
			e1=trapExpression(_t,c);
			_t = _retTree;
			e2=trapExpression(_t,c);
			_t = _retTree;
			_t = __t222;
			_t = _t->getNextSibling();
#line 994 "staticsemantics.g"
			e = new BinaryOp(c->boolean_type, "or", e1, e2);
#line 3910 "EsterelTreeParser.cpp"
			break;
		}
		case LITERAL_not:
		{
			RefLineAST __t223 = _t;
			RefLineAST tmp115_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_not);
			_t = _t->getFirstChild();
			e1=trapExpression(_t,c);
			_t = _retTree;
			_t = __t223;
			_t = _t->getNextSibling();
#line 996 "staticsemantics.g"
			e = new UnaryOp(c->boolean_type, "not", e1);
#line 3925 "EsterelTreeParser.cpp"
			break;
		}
		case ID:
		{
			trap = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
#line 998 "staticsemantics.g"
			
			string name = trap->getText();
			if (!c->traps->contains(name))
			throw LineError(trap, "unrecognized trap " + name);
			SignalSymbol *ts = dynamic_cast<SignalSymbol*>(c->traps->get(name));
			assert(ts);
			assert(ts->kind == SignalSymbol::Trap);
			e = new LoadSignalExpression(c->boolean_type, ts); 
			
#line 3943 "EsterelTreeParser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return e;
}

void EsterelTreeParser::funcName(RefLineAST _t) {
	RefLineAST funcName_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ID:
		{
			RefLineAST tmp116_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_and:
		{
			RefLineAST tmp117_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_and);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_or:
		{
			RefLineAST tmp118_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_or);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_not:
		{
			RefLineAST tmp119_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_not);
			_t = _t->getNextSibling();
			break;
		}
		case PLUS:
		{
			RefLineAST tmp120_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PLUS);
			_t = _t->getNextSibling();
			break;
		}
		case DASH:
		{
			RefLineAST tmp121_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DASH);
			_t = _t->getNextSibling();
			break;
		}
		case STAR:
		{
			RefLineAST tmp122_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR);
			_t = _t->getNextSibling();
			break;
		}
		case SLASH:
		{
			RefLineAST tmp123_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SLASH);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_mod:
		{
			RefLineAST tmp124_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_mod);
			_t = _t->getNextSibling();
			break;
		}
		case LESSTHAN:
		{
			RefLineAST tmp125_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LESSTHAN);
			_t = _t->getNextSibling();
			break;
		}
		case GREATERTHAN:
		{
			RefLineAST tmp126_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GREATERTHAN);
			_t = _t->getNextSibling();
			break;
		}
		case LEQUAL:
		{
			RefLineAST tmp127_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LEQUAL);
			_t = _t->getNextSibling();
			break;
		}
		case GEQUAL:
		{
			RefLineAST tmp128_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GEQUAL);
			_t = _t->getNextSibling();
			break;
		}
		case NEQUAL:
		{
			RefLineAST tmp129_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),NEQUAL);
			_t = _t->getNextSibling();
			break;
		}
		case EQUALS:
		{
			RefLineAST tmp130_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EQUALS);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void EsterelTreeParser::variableDeclList(RefLineAST _t,
	 Context *oc, Context *nc, TypeSymbol *ts 
) {
	RefLineAST variableDeclList_AST_in = _t;
	RefLineAST variableName = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefLineAST varexpr = static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefLineAST __t174 = _t;
		RefLineAST tmp131_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),VARS);
		_t = _t->getFirstChild();
		{ // ( ... )+
		int _cnt179=0;
		for (;;) {
			if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == VDECL)) {
				RefLineAST __t176 = _t;
				RefLineAST tmp132_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),VDECL);
				_t = _t->getFirstChild();
				variableName = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ID);
				_t = _t->getNextSibling();
#line 773 "staticsemantics.g"
				Expression *e = 0;
#line 4115 "EsterelTreeParser.cpp"
				{
				if (_t == static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				switch ( _t->getType()) {
				case COLEQUALS:
				{
					RefLineAST __t178 = _t;
					RefLineAST tmp133_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),COLEQUALS);
					_t = _t->getFirstChild();
					varexpr = (_t == ASTNULL) ? static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
					e=expression(_t,oc);
					_t = _retTree;
					_t = __t178;
					_t = _t->getNextSibling();
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
				}
				}
				}
#line 775 "staticsemantics.g"
				
				string name = variableName->getText();
				if (nc->variables->local_contains(name))
				throw LineError(variableName, "redeclared variable " + name);
				assert(ts);
				if (e && ts != e->type)
				throw LineError(variableName, "type of initializer ("
				+ e->type->name +
				") does not match type of variable (" + ts->name + ")");
				nc->variables->enter(new VariableSymbol(name, ts, e));
				
#line 4155 "EsterelTreeParser.cpp"
				_t = __t176;
				_t = _t->getNextSibling();
			}
			else {
				if ( _cnt179>=1 ) { goto _loop179; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
			}
			
			_cnt179++;
		}
		_loop179:;
		}  // ( ... )+
		_t = __t174;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

RefLineAST EsterelTreeParser::getAST()
{
	return returnAST;
}

void EsterelTreeParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
}
const char* EsterelTreeParser::tokenNames[] = {
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

const unsigned long EsterelTreeParser::_tokenSet_0_data_[] = { 16802304UL, 2097152UL, 570294272UL, 120677UL, 0UL, 0UL, 0UL, 0UL };
// SEQUENCE DOWATCHING DOUPTO RUN COLEQUALS PARALLEL "nothing" "pause" 
// "halt" "emit" "sustain" "call" "present" "if" "repeat" "abort" "await" 
// "loop" "every" "suspend" "trap" "exit" "exec" "signal" "var" 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelTreeParser::_tokenSet_0(_tokenSet_0_data_,8);
const unsigned long EsterelTreeParser::_tokenSet_1_data_[] = { 2155872256UL, 251661315UL, 16383UL, 4194304UL, 0UL, 0UL, 0UL, 0UL };
// CALL Integer FloatConst DoubleConst EQUALS ID PLUS STAR "or" "and" "not" 
// NEQUAL LESSTHAN GREATERTHAN LEQUAL GEQUAL DASH SLASH "mod" QUESTION 
// "pre" DQUESTION "true" "false" StringConstant 
const ANTLR_USE_NAMESPACE(antlr)BitSet EsterelTreeParser::_tokenSet_1(_tokenSet_1_data_,8);


