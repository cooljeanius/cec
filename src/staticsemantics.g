header "post_include_hpp" {

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

}

header "post_include_cpp" {

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

}

options {
  language="Cpp";
}

class EsterelTreeParser extends TreeParser;

options {
  importVocab = Esterel;
  ASTLabelType = "RefLineAST";  // Expect AST nodes with line numbers
}

{
}

file [Modules *ms, string filename]
    : {
        assert(ms);
      }
        ( module[ms] )+
    ;

module [Modules* modules]
    : #( "module" moduleName:ID
         {
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
        }

       declarations[&c]
       s=statement[&c] { m->body = s; }
    )
    ;

declarations [Context *c]
  : #( DECLS
       ( #( "type" ( typeDecl[c] )+ )
       | #( "constant" (constantDecl[c])+ )
       | #( "input" ( signalDecl[c, "input", true] )+ )
       | #( "output" ( signalDecl[c, "output", true ] )+ )
       | #( "inputoutput" ( signalDecl[c, "inputoutput", true] )+ )
       | #( "return" ( signalDecl[c, "return", true] )+ )
       | #( "function" ( functionDecl[c] )+ )
       | #( "procedure" ( procedureDecl[c] )+ )
       | #( "task" ( taskDecl[c] )+ )
       | #( "sensor" ( signalDecl[c, "sensor", true] )+ )
       | #( "relation" ( relationDecl[c] )+ )
       )*
     )
  ;

typeDecl [Context *c]
  : typeName:ID {
     string name = typeName->getText();
     if (c->module->types->local_contains(name))
       throw LineError(typeName, "redeclaration of type " + name);
     c->module->types->enter(new TypeSymbol(name) );
    }
  ;

constantDecl [Context *c]
 {
   Expression *e = 0;
   TypeSymbol *t;
 }
  : #(CDECL
      (
        constantName:ID { string name = constantName->getText(); }
        ( #(EQUALS e=expr:expression[c]) )?
        t=typeToken:type[c]
        {  create_constant(constantName, c, name, e, t); }

      | #(ids:COMMA (ID)+) t=type[c]
        {
          for ( RefLineAST id = #ids->getFirstChild() ; id ;
            id=id->getNextSibling() )
            create_constant(ids, c, id->getText(), 0, t);
        }
      )
    )
  ;

signalDecl [Context *c, string direction, bool isGlobal]
  : #( SDECL signalName:ID
       {
	 string name = signalName->getText();
	 if (c->signals->local_contains(name))
	   throw LineError(signalName, "Redeclaration of " + name);
	 Expression *e = 0;
       }
       ( #(COLEQUALS e=expr:expression[c]) )?
       { TypeSymbol *t = 0; FunctionSymbol *fs = 0; }
       (t=typeToken:type[c]
         ( func:ID
           {
	         string name = func->getText();
             if (!c->module->functions->local_contains(name))
  	           throw LineError(func, "Undeclared function " + name);
       	     Symbol *sym = c->module->functions->get(name);
	         fs = dynamic_cast<FunctionSymbol*>(sym);
	         assert(fs);
     	   }
         | pcf:predefinedCombineFunction
           {
             string name = pcf->getText();
             assert(c->module->functions->contains(name));
             Symbol *sym = c->module->functions->get(name);
             fs = dynamic_cast<BuiltinFunctionSymbol*>(sym);
	         assert(fs);
           }
         )?
       )?
       {
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
       }
     )
  ;

predefinedCombineFunction
  : PLUS
  | STAR
  | "or"
  | "and"
  ;

trapDecl [Context *c]
  : #( TDECL trapName:ID
       {
         string name = trapName->getText();
         if (c->traps->local_contains(name))
           throw LineError(trapName, "redeclaration of trap " + name);
         Expression *e = 0;
       }
       ( #( COLEQUALS e=expression[c] ) )?
       { TypeSymbol *ts = NULL; }
       ( ts=type[c] )?
       {
         c->traps->enter(new SignalSymbol(name, ts, SignalSymbol::Trap,
                         NULL, e, NULL));
         if (e && e->type != ts)
           throw LineError(trapName,
             "initializer does not match type of trap " + name);
       }
     )
  ;


functionDecl [Context *c]
  : #( FDECL functionName:ID
       {
	 string name = functionName->getText();
	 if (c->module->functions->contains(name))
	   throw LineError(functionName, "redeclaration of function " + name);
         FunctionSymbol *fs = new FunctionSymbol(name);
         c->module->functions->enter(fs);	 
         TypeSymbol *ts;
       }
       #( TYPES (ts=type[c] { fs->arguments.push_back(ts); } )* )
       ts=type[c] { fs->result = ts; }
     )
  ;

procedureDecl [Context *c]
  : #( PDECL procedureName:ID
       {
	 string name = procedureName->getText();
	 if (c->module->procedures->contains(name))
	   throw LineError(procedureName,
                           "redeclaration of procedure " + name);
         ProcedureSymbol *ps = new ProcedureSymbol(name);
         c->module->procedures->enter(ps);	 
         TypeSymbol *ts;
       }
       #( TYPES ( ts=type[c] { ps->reference_arguments.push_back(ts); } )* )
       #( TYPES ( ts=type[c] { ps->value_arguments.push_back(ts); } )* )
     )
  ;

taskDecl [Context *c]
  : #( TADECL taskName:ID
       {
	 string name = taskName->getText();
	 if (c->module->tasks->contains(name))
	   throw LineError(taskName, "redeclaration of task " + name);
         TaskSymbol *ps = new TaskSymbol(name);
         c->module->tasks->enter(ps);	 
         TypeSymbol *ts;
       }
       #( TYPES ( ts=type[c] { ps->reference_arguments.push_back(ts); } )* )
       #( TYPES ( ts=type[c] { ps->value_arguments.push_back(ts); } )* )
     )
  ;

relationDecl [Context *c]
  : #( IMPLIES sig1:ID sig2:ID )
    {
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
    }
  | #( POUND
       {
         Exclusion *ex = new Exclusion();
         c->module->relations.push_back(ex);
       }
       ( sig:ID
         {
           string name = sig->getText();
           if (!c->signals->contains(name))
             throw LineError(sig, "undeclared signal " + name);
           SignalSymbol *ss =
             dynamic_cast<SignalSymbol*>(c->signals->get(name));
           assert(ss);
	   ex->signals.push_back(ss);
         }
       )+
     )
  ;

type [Context *c] returns [TypeSymbol *t]
  : { t = 0; }
    type:ID
    {
      string name = type->getText();
      if (!c->module->types->local_contains(name))
        throw LineError(type, "Unknown type " + name);
      t = dynamic_cast<TypeSymbol*>(c->module->types->get(name));
      assert(t);
      // std::cout << "Type " << t->name << std::endl;
    }
  ;


statement [Context *c] returns [Statement *st]
  : { st = 0; }
    ( #( SEQUENCE { StatementList *sl = new StatementList(); }
         (st=statement[c] { sl->statements.push_back(st); } )+
       ) { st = sl; }

    | #( PARALLEL
         { ParallelStatementList *sl = new ParallelStatementList(); }
         (st=statement[c] { sl->threads.push_back(st); } )+
       ) { st = sl; }

    | "nothing" { st = new Nothing(); }

    | "pause" { st = new Pause(); }

    | "halt" { st = new Halt(); }

    | #( "emit"
         esig:ID        
         {
           string name = esig->getText();
           if (!c->signals->contains(name))
             throw LineError(esig, "undeclared signal " + name);
           SignalSymbol *ss =
             dynamic_cast<SignalSymbol*>(c->signals->get(name));
           assert(ss);
           Expression *e = 0;
         }
         ( e=expression[c] )?
         {
           st = new Emit(ss, e);
           if (e) {
             if (ss->type == NULL)
               throw LineError(esig, "emission of value on pure signal " + name);
             if (ss->type != e->type)
               throw LineError(esig, "expression type mismatch on emit " + name);
           }
           // A valued signal does not have to have a valued emission (!)
         }
      )

    | #( "sustain"
         ssig:ID
         {
           string name = ssig->getText();
           if (!c->signals->contains(name))
             throw LineError(ssig, "undeclared signal " + name);
           SignalSymbol *ss =
             dynamic_cast<SignalSymbol*>(c->signals->get(name));
           assert(ss);
           Expression *e = 0;
         }
         ( e=expression[c] )?
         {
           st = new Sustain(ss, e);
           if (e) {
             if (ss->type == NULL)
               throw LineError(ssig, "sustain of value on pure signal " + name);
             if (ss->type != e->type)
               throw LineError(ssig, "expression type mismatch on sustain " + name);
           }
           // A valued signal does not have to have a valued emission
         }
       )

     | { Expression *e; }
       #( COLEQUALS var:ID e=expression[c] )
       {        
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
       }

     | #( "call" proc:ID
          {
            string name = proc->getText();
            if (!c->module->procedures->contains(name))
              throw LineError(proc, "undeclared procedure " + name);
            ProcedureSymbol *ps =
              dynamic_cast<ProcedureSymbol*>(c->module->procedures->get(name));
            assert(ps);
            ProcedureCall *pc = new ProcedureCall(ps);
          }
          procArgs[c, pc]
          { st = pc; }
        )

     | #( "present" { Present *p = new Present(); }
          ( #( CASE { Expression *e; Statement *s = 0; }
               e=sigExpression[c] (s=statement[c])? ) { p->newCase(s, e); }
          )*
          ( #( "else" { Statement *s; }
               s=statement[c]
               { p->default_stmt = s; }
            )
          )?
          { st = p; }
        )

     | #( "if" { If *i = new If(); Expression *e; }
          e=ife:expression[c]
  	  { Statement *s = 0; }
          ( #( "then" s=statement[c] ) )?
          { i->newCase(s, e); }
	  ( #( elif:"elsif" e=expression[c] s=statement[c] )
               { i->newCase(s, e); }
	  )*
          ( #( "else" s=statement[c] { i->default_stmt = s; } ) )?
          { st = i; }
        )

     | #( "loop" { Statement *s; Expression *e; }
          s=statement[c]
          ( /* nothing */ { st = new Loop(s); }
          | e=delayExpression[c] { st = new LoopEach(s, e); }
          )
        )

     | #( "repeat" { bool positive = false; Expression *e; }
          ( "positive" { positive = true; } )?
          e=expr:expression[c]
          {
            if (e->type != c->integer_type)
	      throw LineError(expr,
                              "repeat type is not integer: " + e->type->name);
            Statement *s;
          }
          s=statement[c]
          {
            Counter *cntr = new Counter();
            c->module->counters.push_back(cntr);
            st = new Repeat(s, e, positive, cntr);
          }
        )

     | #( "abort" { bool isWeak = false; Statement *s; }
          ( "weak" { isWeak = true; } )?
          s=statement[c]
          { Abort *a = new Abort(s, isWeak); }
          ( #( CASE { Expression *e; s = 0; }
               e=delayExpression[c] ( s=statement[c] )? ) { a->newCase(s, e); }
          )+
          { st = a; }
        )

     | #( "await" { Await *a = new Await(); }
          ( #( CASE { Expression *e; Statement *s = 0; }
               e=delayExpression[c] ( s=statement[c] )? ) { a->newCase(s, e); }
          )+
          { st = a; }
        )

     | #( "every" { Expression *e; Statement *s; }
          e=delayExpression[c] s=statement[c]
          { st = new Every(s, e); }
        )

     | #( "suspend" { Expression *e; Statement *s; }
          s=statement[c] e=delayExpression[c]
          { st = new Suspend(s, e); }
        )

     | #( "trap"
          {         
	    Trap *t = new Trap();
            // Set up a new context by copying the old and opening up
	    // a new symbol table for traps
            Context nc = *c;
            nc.traps = t->symbols = new SymbolTable();
	    t->symbols->parent = c->traps;            
            Statement *s; Expression *e;
          }
          #( TRAPS ( trapDecl[&nc] )+ )
          s=statement[&nc]
          { t->body = s; }
          ( #( "handle"
               e=trapExpression[&nc] s=statement[&nc] { t->newHandler(e, s); }
             )
          )*
          { st = t; }
        )
	        
      | #( "exit" etrap:ID { Expression *e = 0; } ( e=texpr:expression[c] )?
           {
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
           }
         )

      | #( "exec" { Exec *ex = new Exec(); }         
           ( #( CASE taskid:ID
                {
                  string name = taskid->getText();
                  if (!c->module->tasks->contains(name))
                    throw LineError(taskid, "undeclared task " + name);
                  TaskSymbol *ts = dynamic_cast<TaskSymbol*>(c->module->tasks->get(name));
                  assert(ts); 
                  TaskCall *tc = new TaskCall(ts);
                  ex->calls.push_back(tc);
                }
                procArgs [c, tc]
                tsig:ID
                {
                  string sname = tsig->getText();
                  if (!c->signals->contains(sname))
                    throw LineError(tsig, "undeclared signal " + name);
                  SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(sname));
                  assert(ss);
                  if (ss->kind != SignalSymbol::Return)
                    throw LineError(tsig, "signal " + sname + " is not return");
                  tc->signal = ss;
                  Statement *s;
                }
                ( s=statement[c] { tc->body = s; } )?
              )
           )+
           { st = ex; }
         )

      | #( "var"
           {
             Var *v = new Var();
             Context nc = *c;
             nc.variables = v->symbols = new SymbolTable();
	         v->symbols->parent = c->variables;
             assert(nc.variables);
             TypeSymbol *ts = 0;
           }
           #( VARS
              ( #( COLON vdecls:. ts=type[c] )
                 { variableDeclList(vdecls, c, &nc, ts); }
              )+
            )
           { Statement *s; }
           s=statement[&nc]
           {
             v->body = s;
             st = v;
           }
         )

       | #( "signal"
            {
               Signal *sig = new Signal();
               Context nc = *c;
               nc.signals = sig->symbols = new SymbolTable();
               sig->symbols->parent = c->signals;
            }
            #( SIGS ( signalDecl[&nc, "local", false] )+ )
            { Statement *s; }
            s=statement[&nc]
            {
               sig->body = s;
               st = sig;
            }
          )

      | #( RUN module:ID { Run *r = new Run(module->getText(), c->signals); }
           ( oldMod:ID { r->old_name = oldMod->getText(); } )?
           ( #( TRENAME { TypeSymbol *t; } t=type[c] tID:ID )
                { r->types.push_back(new TypeRenaming(tID->getText(), t)); }
           | #( CRENAME { Expression *e; } e=expression[c] cID:ID )
                { r->constants.
                   push_back(new ConstantRenaming(cID->getText(), e)); }
           | #( FRENAME func:funcName fID:ID
                {
                  string name = func->getText();
                  if (!c->module->functions->contains(name))
                    throw LineError(func, "undeclared function " + name);
                  FunctionSymbol *fs =
                    dynamic_cast<FunctionSymbol*>(c->module->functions->get(name));
                  assert(fs);
                  r->functions.push_back(new FunctionRenaming(fID->getText(), fs));
                }
              )
           | #( PRENAME pro:ID pID:ID
                {
                  string name = pro->getText();
                  if (!c->module->procedures->contains(name))
                    throw LineError(pro, "undeclared procedure " + name);
                  ProcedureSymbol *ps =
                    dynamic_cast<ProcedureSymbol*>(c->module->procedures->get(name));
                  assert(ps);
                  r->procedures.push_back(new ProcedureRenaming(pID->getText(),
                                                                ps));
                }
              )
           | #( TARENAME task:ID trID:ID
                {
                  string name = task->getText();
                  if (!c->module->tasks->contains(name))
                    throw LineError(task, "undeclared tasks " + name);
                  ProcedureSymbol *ps =
                    dynamic_cast<ProcedureSymbol*>(c->module->tasks->get(name));
                  assert(ps);
                  r->tasks.push_back(new ProcedureRenaming(trID->getText(),
                                                           ps));
                }
              )
           | #( SRENAME sig:ID sID:ID
                {
                  string name = sig->getText();
                  if (!c->signals->contains(name))
                    throw LineError(sig, "undeclared signal " + name);
                  SignalSymbol *ss =
                    dynamic_cast<SignalSymbol*>(c->signals->get(name));
                  assert(ss);
                  r->signals.push_back(new SignalRenaming(sID->getText(), ss));
                }
              )
           )*
           { st = r; }
         )

      | #( DOWATCHING { Expression *e; Statement *s1, *s2 = 0; }
           s1=statement[c]
           e=de:delayExpression[c]
           ( s2=statement[c] )?
           { st = new DoWatching( s1, e, s2); }
         )

      | #( DOUPTO { Expression *e; Statement *s; }
           s=statement[c] e=delayExpression[c]
           { st = new DoUpto(s, e); }
         )
  )
  {
    assert(st);
  }
  ;

variableDeclList [ Context *oc, Context *nc, TypeSymbol *ts ]
  : #( VARS
       ( #( VDECL variableName:ID
          { Expression *e = 0; }
          ( #( COLEQUALS e=varexpr:expression[oc] ) )?
          {
            string name = variableName->getText();
            if (nc->variables->local_contains(name))
               throw LineError(variableName, "redeclared variable " + name);
            assert(ts);
            if (e && ts != e->type)
              throw LineError(variableName, "type of initializer ("
                     + e->type->name +
                     ") does not match type of variable (" + ts->name + ")");
            nc->variables->enter(new VariableSymbol(name, ts, e));
          }
          )
       )+
     )
  ;

funcName
   : ID
   | "and"
   | "or"
   | "not"
   | PLUS
   | DASH
   | STAR
   | SLASH
   | "mod"
   | LESSTHAN
   | GREATERTHAN
   | LEQUAL
   | GEQUAL
   | NEQUAL
   | EQUALS
   ;

// FIXME: Add type checking
procArgs [Context *c, ProcedureCall *pc]
  : #( VARS
       ( var:ID
         {
           string name = var->getText();
           if (!c->variables->contains(name))
             throw LineError(var, "undefined variable " + name);
           VariableSymbol *vs =
             dynamic_cast<VariableSymbol*>(c->variables->get(name));
           assert(vs);
           pc->reference_args.push_back(vs);
         }
       )*
     )
    #( ARGS { Expression *e; }
       ( e=expression[c] { pc->value_args.push_back(e); } )*
     )
  ;

expression [Context *c] returns [Expression *e]
  :
  {
    Expression *e1 = 0, *e2 = 0; // for safety
    e = 0; // for safety
  }
  (
    Integer { e = new Literal(#expression->getText(), c->integer_type); }
  | FloatConst { e = new Literal(#expression->getText(), c->float_type); }
  | DoubleConst { e = new Literal(#expression->getText(), c->double_type); }
  | "true" { e = new LoadVariableExpression(c->true_constant); }
  | "false" { e = new LoadVariableExpression(c->false_constant); }
  | StringConstant { e = new Literal(#expression->getText(), c->string_type); }
  | ID
    {
      string name = #expression->getText();
      if (!c->variables->contains(name))
        throw LineError(#expression, "undeclared variable " + name);
      Symbol *s = c->variables->get(name);
      assert(s);
      VariableSymbol *vs = dynamic_cast<VariableSymbol*>(s);
      assert(vs);
      e = new LoadVariableExpression(vs);
    }
  | #( QUESTION sig:ID )
    {
      string name = sig->getText();
      if (!c->signals->contains(name))
        throw LineError(sig, "unknown signal " + name);
      SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(name));
      assert(ss);
      if (!ss->type)
        throw LineError(sig, "attempting to take value of pure signal " + name);
      e = new LoadSignalValueExpression(ss);
    }
  | #( DQUESTION trap:ID )
    {
      string name = trap->getText();
      if (!c->traps->contains(name))
        throw LineError(trap, "unknown trap " + name);
      SignalSymbol *ts = dynamic_cast<SignalSymbol*>(c->traps->get(name));
      assert(ts);
      if (!ts->type)
        throw LineError(trap, "attempting to take value of pure trap " + name);
      e = new LoadSignalValueExpression(ts);
    }
  | #( CALL func:ID
       {
	 string name = func->getText();
	 if (!c->module->functions->contains(name))
	   throw LineError(func, "undeclared function " + name);
         FunctionSymbol *fs =
           dynamic_cast<FunctionSymbol*>(c->module->functions->get(name));
         assert(fs);
         FunctionCall *fc = new FunctionCall(fs);
       }
       ( e1=expression[c] { fc->arguments.push_back(e1); } )*
       {
	 if ( fc->arguments.size() != fs->arguments.size() )
           throw LineError(func, "argument count wrong for call to " + name);
         for ( size_t i = 0 ; i < fc->arguments.size() ; i++ )
           if (fc->arguments[i]->type != fs->arguments[i])
             throw LineError(func, "mismatched argument type");
         e = fc;         
       }
     )

  | #( "or" e1=expression[c] e2=expression[c] )
    { e = boolean_binop(#expression, c, "or", e1, e2); }
  | #( "and" e1=expression[c] e2=expression[c] )
    { e = boolean_binop(#expression, c, "and", e1, e2); }
  | #( "not" e1=expression[c] )
    {
      if (e1->type != c->boolean_type)
        throw LineError(#expression, "argument of not must be boolean");
      e = new UnaryOp(c->boolean_type, "not", e1);
    }

  | #( PLUS e1=expression[c] e2=expression[c] )
    { e = numeric_binop(#expression, c, "+", e1, e2); }
  | #( DASH e1=expression[c]
       ( /* nothing: corresponds to unary negation */
         {
           if (e1->type != c->integer_type &&
               e1->type != c->float_type &&
	       e1->type != c->double_type)
             throw LineError(#expression,
                "argument of unary - must be numeric, not " + e1->type->name);
           e = new UnaryOp(e1->type, "-", e1);
         }
       | e2=expression[c] { e = numeric_binop(#expression, c, "-", e1, e2); }
       )
     )
  | #( STAR e1=expression[c] e2=expression[c] )
    { e = numeric_binop(#expression, c, "*", e1, e2); }
  | #( SLASH e1=expression[c] e2=expression[c] )
    { e = numeric_binop(#expression, c, "/", e1, e2); }
  | #( "mod" e1=expression[c] e2=expression[c] )
    { e = numeric_binop(#expression, c, "mod", e1, e2); }

  | #( EQUALS e1=expression[c] e2=expression[c] )
    { e = equality(#expression, c, "=", e1, e2); }
  | #( NEQUAL e1=expression[c] e2=expression[c] )
    { e = equality(#expression, c, "<>", e1, e2); }

  | #( LESSTHAN e1=expression[c] e2=expression[c] )
    { e = comparison(#expression, c, "<", e1, e2); }
  | #( LEQUAL e1=expression[c] e2=expression[c] )
    { e = comparison(#expression, c, "<=", e1, e2); }
  | #( GREATERTHAN e1=expression[c] e2=expression[c] )
    { e = comparison(#expression, c, ">", e1, e2); }
  | #( GEQUAL e1=expression[c] e2=expression[c] )
    { e = comparison(#expression, c, ">=", e1, e2); }
  | "pre"
    { throw LineError(#expression, "pre not supported"); }
  )
  {
    assert(e);
    assert(e->type);
  }
  ;

sigExpression [Context *c] returns [Expression *e]
  : { Expression *e1, *e2; e = 0; }
  ( #( "and" e1=sigExpression[c] e2=sigExpression[c] )
    { e = new BinaryOp(c->boolean_type, "and", e1, e2); }   
  | #( "or" e1=sigExpression[c] e2=sigExpression[c] )
    { e = new BinaryOp(c->boolean_type, "or", e1, e2); }
  | #( "not" e1=sigExpression[c] )
    { e = new UnaryOp(c->boolean_type, "not", e1); }
  | "pre"
    { throw LineError(#sigExpression, "pre not supported"); }
  | sig:ID
    {
      string name = sig->getText();
      if (!c->signals->contains(name))
        throw LineError(sig, "unrecognized signal " + name);
      SignalSymbol *ss = dynamic_cast<SignalSymbol*>(c->signals->get(name));
      e = new LoadSignalExpression(c->boolean_type, ss);
    }
  )
  ;

delayExpression [Context *c] returns [Expression *e]
  : { Expression *e1, *e2; e = 0; }
  ( e=sigExpression[c]
  | #( "immediate" e1=sigExpression[c] )
    { e = new Delay( c->boolean_type, e1, 0, true, 0 ); }
  | #( DELAY e2=expr2:expression[c] e1=sigExpression[c] )
    {
      Counter *cntr = new Counter();
      assert(cntr);
      c->module->counters.push_back(cntr);
      e = new Delay( c->boolean_type, e1, e2, false, cntr );
      if (e2->type != c->integer_type)
        throw LineError(#expr2, "delay count must be integer");
    }
  )
  ;

trapExpression [Context *c] returns [Expression *e]
  : { Expression *e1, *e2; e = 0; }
  ( #( "and" e1=trapExpression[c] e2=trapExpression[c] )
    { e = new BinaryOp(c->boolean_type, "and", e1, e2); }
  | #( "or" e1=trapExpression[c] e2=trapExpression[c] )
    { e = new BinaryOp(c->boolean_type, "or", e1, e2); }
  | #( "not" e1=trapExpression[c] )
    { e = new UnaryOp(c->boolean_type, "not", e1); }
  | trap:ID
    {
      string name = trap->getText();
      if (!c->traps->contains(name))
        throw LineError(trap, "unrecognized trap " + name);
      SignalSymbol *ts = dynamic_cast<SignalSymbol*>(c->traps->get(name));
      assert(ts);
      assert(ts->kind == SignalSymbol::Trap);
      e = new LoadSignalExpression(c->boolean_type, ts); 
    }
  )
  ;
