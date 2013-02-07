#ifndef __LINEAST_HPP__
#define __LINEAST_HPP__
#include <antlr/CommonAST.hpp>

class LineAST;

typedef ANTLR_USE_NAMESPACE(antlr)ASTRefCount<LineAST> RefLineAST;

class LineAST : public ANTLR_USE_NAMESPACE(antlr)CommonAST {
public:
  LineAST( void ) {}
  ~LineAST( void ) {}

  void initialize( ANTLR_USE_NAMESPACE(antlr)RefToken t ) {
    ANTLR_USE_NAMESPACE(antlr)CommonAST::initialize(t);
    setLine(t->getLine());
  }

  void initialize(int t,const ANTLR_USE_NAMESPACE(std)string& txt) {
    setType(t);
    setText(txt);
    setLine(-1);
  }

  void addChild( RefLineAST c ) {
    ANTLR_USE_NAMESPACE(antlr)BaseAST::addChild(
	 static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(c) );
  }

  RefLineAST getFirstChild() {
    return static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)CommonAST::getFirstChild());
  }

  RefLineAST getNextSibling() {
    return static_cast<RefLineAST>(ANTLR_USE_NAMESPACE(antlr)CommonAST::getNextSibling());
  }

  static ANTLR_USE_NAMESPACE(antlr)RefAST factory( void ) {
    ANTLR_USE_NAMESPACE(antlr)RefAST ret =
      static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(RefLineAST(new LineAST));
    return ret;
  }

  void setLine( int l ) {
    line = l;
  }

  int getLine() const { return line; }

 private:
  int line;

};

#endif
