/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: ASTRefCount.cpp,v 1.1 2004/04/18 05:16:58 sedwards Exp $
 */
#include "antlr/ASTRefCount.hpp"
#include "antlr/AST.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

ASTRef::ASTRef(AST* p)
: ptr(p), count(1)
{
	if (p && !p->ref)
		p->ref = this;
}

ASTRef::~ASTRef()
{
	delete ptr;
}

ASTRef* ASTRef::getRef(const AST* p)
{
	if (p) {
		AST* pp = const_cast<AST*>(p);
		if (pp->ref)
			return pp->ref->increment();
		else
			return new ASTRef(pp);
	} else
		return 0;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

