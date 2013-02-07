#ifndef INC_TokenStream_hpp__
#define INC_TokenStream_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: TokenStream.hpp,v 1.1 2004/04/18 05:16:56 sedwards Exp $
 */

#include <antlr/config.hpp>
#include <antlr/Token.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This interface allows any object to pretend it is a stream
 * of tokens.
 * @author Terence Parr, MageLang Institute
 */
class ANTLR_API TokenStream {
public:
	virtual RefToken nextToken()=0;
	virtual ~TokenStream()
	{
	}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TokenStream_hpp__
