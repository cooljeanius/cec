header "post_include_hpp" {
#include "LineAST.hpp"
}

options {
  language="Cpp";
}

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
class EsterelParser extends Parser;

options {
  k = 2;			// Lookahead
  buildAST = true;		// Construct an AST during parsing
  exportVocab = Esterel;	// Export these token types for the tree walker
  ASTLabelType = "RefLineAST";	// Create AST nodes with line numbers
  defaultErrorHandler = false; // Don't automatically catch every exception
}

tokens {
 SIGS;
 VARS;
 TYPES;
 DECLS;
 TRAPS;
 SEQUENCE;
 ARGS;
 CASE;

 DELAY;
 DOWATCHING;
 DOUPTO;

 CDECL;		// Constant Declaration
 TDECL;		// Trap Declaration
 FDECL;		// Function Delcaration
 PDECL;		// Procedure Declaration
 TADECL;	// Task Declaration
 VDECLS;	// Declaration of multiple variables with same type
 VDECL;		// Variable Declaration
 SDECL;		// Signal or Sensor Declaration

 CALL;
 RUN;

 TRENAME;
 CRENAME;
 SRENAME;
 TARENAME;
 FRENAME;
 PRENAME;

 Integer;
 FloatConst;
 DoubleConst;
}

file
  : (module)+ EOF!
  ;

module
  : "module"^ moduleIdentifier COLON!
      declarations
      statement
    (
      "end"! "module"!
    | PERIOD!  // Deprecated syntax
    )
  ;

// Interface Declaration

declarations
  : (interfaceDecls)*
    { #declarations = #([DECLS, "decls"], #declarations); }
  ;

interfaceDecls :
    typeDecls
  | constantDecls
  | functionDecls
  | procedureDecls
  | taskDecls
  | interfacesignalDecls
  | sensorDecls
  | relationDecls
  ;

// Type Declarations

typeDecls
  : "type"^ typeIdentifier (COMMA! typeIdentifier)* SEMICOLON!
  ;

// Constant Declarations

constantDecls
  : "constant"^ constantDecl (COMMA! constantDecl)* SEMICOLON!
  ;

constantDecl
  : ( constantIdentifier (constantInitializer)?
    | identifierList  /// Deprecated syntax
    ) COLON! typeIdentifier
    { #constantDecl = #([CDECL, "cdecl"], #constantDecl); }
  ;

constantInitializer
  : EQUALS^ constantAtom
  ;

identifierList
  : ID COMMA^ ID (COMMA! ID)*
  ;

// Function Declarations

functionDecls
  : "function"^ functionDecl (COMMA! functionDecl)* SEMICOLON!
  ;

functionDecl
  : functionIdentifier optTypeIdentifierList COLON! typeIdentifier
    { #functionDecl = #([FDECL, "fdecl"], #functionDecl); }
  ;

optTypeIdentifierList
  : LPAREN! ( typeIdentifier ( COMMA! typeIdentifier)* )? RPAREN!
    { #optTypeIdentifierList = #([TYPES,"types"], #optTypeIdentifierList); }
  ;
    
// Procedure Declarations

procedureDecls 
  : "procedure"^ procedureDecl (COMMA! procedureDecl)* SEMICOLON!
  ;

procedureDecl
  : procedureIdentifier optTypeIdentifierList optTypeIdentifierList
    { #procedureDecl = #([PDECL, "pdecl"], #procedureDecl); }
  ;

// Task Declarations

taskDecls
  : "task"^ taskDecl (COMMA! taskDecl)* SEMICOLON!
  ;

taskDecl
  : taskIdentifier optTypeIdentifierList optTypeIdentifierList
    { #taskDecl = #([TADECL, "tdecl"], #taskDecl); }
  ;

// Signal Declarations

/**
    The grammar allows full expressions in the initializers
    but only constants are permitted in interface signals.
*/
    
interfacesignalDecls
  : "input"^ signalDecl (COMMA! signalDecl)* SEMICOLON!
  | "output"^ signalDecl (COMMA! signalDecl)* SEMICOLON!
  | "inputoutput"^ signalDecl (COMMA! signalDecl)* SEMICOLON!
  | "return"^ signalDecl (COMMA! signalDecl)* SEMICOLON!
  ;

signalDeclList
  : signalDecl (COMMA! signalDecl)*
      { #signalDeclList = #([SIGS,"sigs"], #signalDeclList); }
  ;

signalDecl
  : signalIdentifier
      ( /* empty */
      | (signalInitializer)? COLON! channelType
      | LPAREN! channelType RPAREN! /// Deprecated syntax
      )
    { #signalDecl = #([SDECL,"sdecl"], #signalDecl); }
  ;

signalInitializer
  : COLEQUALS^ expression
  ;

channelType
  : typeIdentifier
  | "combine"! typeIdentifier "with"!
    (functionIdentifier | predefinedCombineFunction)
  ;

predefinedCombineFunction
  : PLUS
  | STAR
  | "or"
  | "and"
  ;

// Sensor Declarations

sensorDecls
  : "sensor"^ sensorDecl (COMMA! sensorDecl)* SEMICOLON!
  ;

sensorDecl
  :  sensorIdentifier
       ( COLON! typeIdentifier
       | LPAREN! typeIdentifier RPAREN! /// Deprecated syntax
       )
     { #sensorDecl = #([SDECL,"sdecl"], #sensorDecl); }
  ;

// Input Relation Declarations

relationDecls
  : "relation"^ relationDecl (COMMA! relationDecl)* SEMICOLON!
  ;

relationDecl
  : implicationDecl
  | exclusionDecl
  ;

implicationDecl
  : signalIdentifier IMPLIES^ signalIdentifier
  ;

exclusionDecl
  : signalIdentifier POUND^ signalIdentifier (POUND! signalIdentifier)*
  ;

/***********************************************************************
 *
 * Expressions
 *
 ***********************************************************************/

// Data expressions

expression
  : orexpr
  ;

orexpr
  : andexpr ("or"^ andexpr)*
  ;

andexpr
  : notexpr ("and"^ notexpr)*
  ;

notexpr
  : "not"^ cmpexpr
  | cmpexpr
  ;

cmpexpr
  : addexpr ( (EQUALS^ | NEQUAL^ | LESSTHAN^ | GREATERTHAN^ | LEQUAL^ | GEQUAL^) addexpr )*
  ;

addexpr
  : mulexpr ( (PLUS^ | DASH^) mulexpr)*
  ;

mulexpr
  : unaryexpr ( (STAR^ | SLASH^ | "mod"^) unaryexpr )*
  ;

unaryexpr
  : DASH^ unaryexpr
  | LPAREN! expression RPAREN!
  | QUESTION^ signalIdentifier
  | "pre"^ LPAREN! QUESTION! signalIdentifier RPAREN!
  | DQUESTION^ trapIdentifier
  | functionCall
  | constant
  ;

constant
  : constantLiteral
  | unsignedNumber
  ;

constantLiteral
  : constantIdentifier
  | "true"
  | "false"
  | stringConstant
  ;

constantAtom
  : constantLiteral
  | signedNumber
  ;

signedNumber
  : unsignedNumber
  | DASH^ unsignedNumber
  ;

unsignedNumber
  : Integer
  | FloatConst
  | DoubleConst
  ;

functionCall
  : functionIdentifier LPAREN! (expression (COMMA! expression)*)? RPAREN!
    { #functionCall = #([CALL,"call"], #functionCall); }
  ;

// Signal expressions

signalExpression
  : sandexpr ("or"^ sandexpr)*
  ;

sandexpr
  : sunaryexpr ("and"^ sunaryexpr)*
  ;

sunaryexpr
  : signalIdentifier
  | "pre"^ LPAREN! signalIdentifier RPAREN!
  | "not"^ sunaryexpr
  | LPAREN! signalExpression RPAREN!
  ;

// Delay expressions

delayExpression
  : ( (expression bracketedSignalExpression) => delayPair
    | bracketedSignalExpression
    | "immediate"^ bracketedSignalExpression
    )
  ;

delayPair
  : expression bracketedSignalExpression
    { #delayPair = #([DELAY, "delay"], #delayPair); }
  ;
  
bracketedSignalExpression
  :  signalIdentifier
  | LBRACKET! signalExpression RBRACKET!
  ;

// statements

/// Statements in parallel are grouped in a list
statement
  : sequence (PARALLEL! sequence)*
    { if (#statement && #statement->getNextSibling()) {
        #statement = #([PARALLEL, "||"], #statement);
      }
    }
  ;

/// Statements in sequence are grouped in a list
sequence
  : atomicStatement
    (options {greedy=true;} : SEMICOLON! atomicStatement)*
    (SEMICOLON!)?
    { if (#sequence && #sequence->getNextSibling()) {
        #sequence = #([SEQUENCE, ";"], #sequence);
      }
    }
  ;

atomicStatement
  : "nothing"^
  | "pause"^
  | "halt"^
  | emit
  | sustain
  | assignment
  | procedureCall
  | present
  | ifstatement
  | loop
  | repeat
  | abort
  | await
  | every
  | suspend
  | trap
  | exit
  | exec
  | localvariableDecl
  | localSignalDecl
  | runModule
  | LBRACKET! statement RBRACKET!
  | doStatements
  ;

// Signal Emission

emit
  : "emit"^ signalIdentifier (LPAREN! expression RPAREN!)?
  ;

sustain
  : "sustain"^ signalIdentifier (LPAREN! expression RPAREN!)?
  ;

// assignment and Procedure Call

assignment
  : variableIdentifier COLEQUALS^ expression
  ;

procedureCall
  : "call"^ procedureIdentifier refArgs valueArgs
  ;

/// Procedure pass-by-reference parameters (variables)
refArgs
  : LPAREN! (variableIdentifier (COMMA! variableIdentifier)*)? RPAREN!
     { #refArgs = #([VARS, "vars"], #refArgs); }
  ;

/// Procedure pass-by-value parameters (expressions)
valueArgs
  : LPAREN! (expression (COMMA! expression)*)? RPAREN!
     { #valueArgs = #([ARGS, "args"], #valueArgs); }
  ;


// The present Signal Test

present
  : "present"^
    (presentThenPart | (presentCase)+) (elsePart)? "end"! ("present"!)?
  ;

presentThenPart
  : presentEvent ("then"! statement)?
    { #presentThenPart = #([CASE,"case"], presentThenPart); }
  ;

presentEvent
  : ( signalExpression
    | LBRACKET! signalExpression RBRACKET!
    )
  ;

elsePart
  :  "else"^ statement
  ;

// This isn't redundant because the node type CASE != "case"

presentCase
  : "case"! presentEvent ("do"! statement)?
    { #presentCase = #([CASE,"case"], presentCase); }
  ;

// The if Data Test

ifstatement
  : "if"^ expression (thenPart)? (elsif)* (elsePart)? "end"! ("if"!)?
  ;

thenPart
  : "then"^ statement
  ;

elsif
  : "elsif"^ expression "then"! statement
  ;

// repeat loops

repeat
  : ("positive")? "repeat"^ expression "times"! statement "end"! ("repeat"!)?
  ;

// The abort statement

abort
  : "abort"^ statement "when"!
    ( abortOneCaseStrong
    | (acase)+ "end" ("abort")?
    )
  | "weak" "abort"^ statement "when"!
    ( abortOneCaseWeak
    | (acase)+ "end" (("weak")? "abort")?
    )
  ;

abortOneCaseStrong
  : delayExpression ("do"! statement "end"! ("abort"!)?)?
    { #abortOneCaseStrong = #([CASE,"case"], abortOneCaseStrong); }
  ;

abortOneCaseWeak
  : delayExpression ("do"! statement "end"! (("weak"!)? "abort"!)?)?
    { #abortOneCaseWeak = #([CASE,"case"], abortOneCaseWeak); }
  ;

// This isn't redundant because the node type CASE != "case"

acase
  : "case"! delayExpression ("do"! statement)?
    { #acase = #([CASE,"case"], acase); }
  ;

// The await statement

await
  : "await"^ (awaitOneCase | (acase)+ "end"! ("await"!)?)
  ;

awaitOneCase
  : delayExpression ("do"! statement "end"! ("await"!)?)?
    { #awaitOneCase = #([CASE,"case"], awaitOneCase); }
  ;

// Loops

loop
  : "loop"^ statement
   ( "end"! ("loop"!)?
   | "each"! delayExpression
   )  
  ;

every
  : "every"^ delayExpression "do"! statement "end"! ("every"!)?
  ;

// The suspend statement

suspend
  : "suspend"^ statement "when"! delayExpression
  ;

// traps

trap
  : "trap"^ trapDeclList "in"!
       statement (trapHandler)*
    "end"! ("trap"!)?
  ;

trapDeclList
  : trapDecl (COMMA! trapDecl)*
    { #trapDeclList = #([TRAPS,"traps"], #trapDeclList); }
  ;

trapDecl
  : trapIdentifier ((trapInitializer)? COLON! channelType)?
    { #trapDecl = #([TDECL,"tdecl"], #trapDecl); }
  ;

trapInitializer
  : COLEQUALS^ expression
  ;

exit
  : "exit"^ trapIdentifier (LPAREN! expression RPAREN!)?
  ;

trapHandler
  : "handle"^ trapEvent "do"! statement
  ;

trapEvent
  : eand ("or"^ eand)*
  ;

eand
  : eunary ("and"^ eunary)*
  ;

eunary
  : trapIdentifier
  | LPAREN! trapEvent RPAREN!
  | "not"^ eunary
  ;

// The exec Task Execution statement

exec
  : "exec"^ execOneCase
  | "exec"^ (execCase)+ "end"! ("exec"!)?
  ;

execOneCase
  : taskIdentifier refArgs valueArgs "return"! signalIdentifier
      ("do"! statement "end"! ("exec"!)?)?
    { #execOneCase = #([CASE,"case"], execOneCase); }
  ;

execCase
  : "case"! taskIdentifier refArgs valueArgs "return"! signalIdentifier
    ("do"! statement)?
    { #execCase = #([CASE,"case"], execCase); }
  ;

// Local Signal Declaration

localSignalDecl
  : "signal"^ signalDeclList "in"! statement "end"! ("signal"!)?
  ;

// Local Variable Declaration

localvariableDecl
  : "var"^  variableDeclList "in"! statement "end"! ("var"!)?
  ;

variableDeclList
  : variableDecls (COMMA! variableDecls)*
    { #variableDeclList = #([VARS,"vars"], #variableDeclList); }
  ;

// A comma-separated list of variables of the same type

// This differs slightly from the grammar in the V5.91 primer.  It also
// allows e.g., var a := 1, b := 2 : integer in ...

variableDecls
  :  variableDeclList2 COLON^ typeIdentifier
  ;

variableDeclList2
  : ( variableDecl (COMMA! variableDecl)* )
    { #variableDeclList2 = #([VARS,"vars"], #variableDeclList2); }
  ;

// A single variable declaration (e.g., a or a := 3)

variableDecl
  : variableIdentifier (variableInitializer)?
    { #variableDecl = #([VDECL,"vdecl"], #variableDecl); }
  ;

variableInitializer
  : COLEQUALS^ expression
  ;

// The run module Instantiation statement

runModule
  : ( "run"!
    | "copymodule"! /// Deprecated syntax
    )
    moduleIdentifier (SLASH! moduleIdentifier)?
    (LBRACKET! renaming (SEMICOLON! renaming)* RBRACKET!)?
      { #runModule = #([RUN,"run"], #runModule); }
  ;

renaming
  : "type"! typeRenaming (COMMA! typeRenaming)*
  | "constant"! constantRenaming (COMMA! constantRenaming)*
  | "function"! functionRenaming (COMMA! functionRenaming)*
  | "procedure"! procedureRenaming (COMMA! procedureRenaming)*
  | "task"! taskRenaming (COMMA! taskRenaming)*
  | "signal"! signalRenaming (COMMA! signalRenaming)*
  ;

typeRenaming
  : typeIdentifier SLASH! typeIdentifier
     { #typeRenaming = #([TRENAME, "trename"], #typeRenaming); }
  ;

constantRenaming
  : constantAtom SLASH! constantIdentifier
     { #constantRenaming = #([CRENAME, "crename"], #constantRenaming); }
  ;

functionRenaming
  : ( functionIdentifier SLASH! functionIdentifier
    | predefinedFunction SLASH! functionIdentifier )
     { #functionRenaming = #([FRENAME, "frename"], #functionRenaming); }
    
  ;

predefinedFunction
  : "and"
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

procedureRenaming
  : procedureIdentifier SLASH! procedureIdentifier
     { #procedureRenaming = #([PRENAME, "prename"], #procedureRenaming); }
  ;

taskRenaming
  : taskIdentifier SLASH! taskIdentifier
     { #taskRenaming = #([TARENAME, "trename"], #taskRenaming); }
  ;


signalRenaming
  : signalIdentifier SLASH! signalIdentifier
     { #signalRenaming = #([SRENAME, "srename"], #signalRenaming); }
  ;

// Deprecated Syntax

doStatements
  : "do"! statement
     ( "watching"! delayExpression ("timeout"! statement "end"! ("timeout"!)?)?
       { #doStatements = #([DOWATCHING, "dowatching"], #doStatements); }
     | "upto"! delayExpression
       { #doStatements = #([DOUPTO, "doupto"], #doStatements); }
     )
  ;

typeIdentifier : ID  ;
signalIdentifier : ID  ;
sensorIdentifier : ID  ;
trapIdentifier : ID  ;
constantIdentifier : ID  ;
functionIdentifier : ID  ;
variableIdentifier : ID  ;
procedureIdentifier : ID  ;
taskIdentifier : ID  ;
moduleIdentifier : ID  ;

stringConstant : StringConstant  ;

class EsterelLexer extends Lexer;

options {
  k = 2;			// Lookahead to distinguish, e.g., : and :=
  charVocabulary = '\3'..'\377'; // Handle all 8-bit characters
  exportVocab = Esterel;	// Export these token types for tree walkers
  testLiterals = false;         // Disable checking every rule against keywords
}

PERIOD :	'.' ;
POUND :         '#' ;
PLUS :		'+' ;
DASH :		'-' ;
SLASH :		'/' ;
STAR :		'*' ;
PARALLEL :      "||" ;
LESSTHAN :	'<' ;
GREATERTHAN :	'>' ;
COMMA :		',' ;
EQUALS :	'=' ;
SEMICOLON :	';' ;
COLON :		':' ;
COLEQUALS :	":=" ;
LPAREN :	'(' ;
RPAREN :	')' ;
LBRACKET :	'[' ;
RBRACKET :	']' ;
QUESTION :	'?' ;
DQUESTION :	"??" ;
LEQUAL :	"<=" ;
GEQUAL :	">=" ;
NEQUAL :	"<>" ;
IMPLIES :	"=>" ;


ID options { testLiterals = true; }
  : ('a'..'z' | 'A'..'Z') ('a'..'z' | 'A'..'Z' | '_' | '0'..'9')*
  ;

/// Numeric constants

/**
  The two types of number are split to avoid a side-effect of LL(k) parsing:
  If the two rules were combined, the lookahead set for Number would include
  a period (e.g., from ".1") followed by end-of-token e.g., from "1" by
  itself), which collides with the lookahead set for the single-period rule.
 */

Number
  : ('0'..'9')+
      ( '.' ('0'..'9')* (Exponent)?
        ( ('f'|'F') { $setType(FloatConst); }
        | /* empty */ { $setType(DoubleConst); }
        )
      | /* empty */ { $setType(Integer); }
      )
  ;

FractionalNumber
  : '.' ('0'..'9')+ (Exponent)?
        ( ('f'|'F') { $setType(FloatConst); }
        | /* empty */ { $setType(DoubleConst); }
        )
  ;

protected
Exponent
  : ('e'|'E') ('+'|'-')? ('0'..'9')+
  ;

/**
  String constants must be contained on a single line and may contain
  double quotes, e.g.,

      "This is a constant with ""double quotes""" 
*/
StringConstant
  : '"'! ( ~('"' | '\n') | ('"'! '"'))* '"'!
  ;

Whitespace
  : (' ' | '\t' | '\f')+
    { $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }
  ;

Newline
  : ( '\n' | "\r\n" | '\r' )
    { $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP);
      newline(); }
  ;

/**
  Multi-line comments start with %{ and end with %}
  Single-line comments start with %
*/

// Example from the ANTLR documentation
// CMT : "/*" (options {greedy=false;} :.)* "*/" ;

// FIXME: Should terminate lines in % ... comments with \r or \n\r, too

Comment
  : '%' ( ('{') => '{'
          ( options {greedy=false;}: // Prevents .* from eating the whole file
            (
              ('\r' '\n') => '\r' '\n'	{ newline(); }
              | '\r'			{ newline(); }
              | '\n'			{ newline(); }
              | ~( '\n' | '\r' )
            )
          )*
          "}%"
        | ((~'\n'))* '\n' { newline(); }
        )
    { $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }
  ;
