/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/RIGHTS.html
 *
 * $Id: TokenBuffer.cpp,v 1.1 2004/04/18 05:17:01 sedwards Exp $
 */

#include "antlr/TokenBuffer.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/**A Stream of Token objects fed to the parser from a TokenStream that can
 * be rewound via mark()/rewind() methods.
 * <p>
 * A dynamic array is used to buffer up all the input tokens.  Normally,
 * "k" tokens are stored in the buffer.  More tokens may be stored during
 * guess mode (testing syntactic predicate), or when LT(i>k) is referenced.
 * Consumption of tokens is deferred.  In other words, reading the next
 * token is not done by conume(), but deferred until needed by LA or LT.
 * <p>
 *
 * @see antlr.Token
 * @see antlr.TokenStream
 * @see antlr.TokenQueue
 */

/** Create a token buffer */
TokenBuffer::TokenBuffer( TokenStream& input_ )
: input(input_), nMarkers(0), markerOffset(0), numToConsume(0)
{
}

/** Ensure that the token buffer is sufficiently full */
void TokenBuffer::fill(int amount)
{
	syncConsume();
	// Fill the buffer sufficiently to hold needed tokens
	while (queue.entries() < amount + markerOffset) {
		// Append the next token
		queue.append(input.nextToken());
	}
}

/** Get a lookahead token value */
int TokenBuffer::LA(int i)
{
	fill(i);
	return queue.elementAt(markerOffset+i-1)->type;
}

/** Get a lookahead token */
RefToken TokenBuffer::LT(int i)
{
	fill(i);
	return queue.elementAt(markerOffset+i-1);
}

/** Return an integer marker that can be used to rewind the buffer to
 * its current state.
 */
int TokenBuffer::mark()
{
	syncConsume();
	nMarkers++;
	return markerOffset;
}

/**Rewind the token buffer to a marker.
 * @param mark Marker returned previously from mark()
 */
void TokenBuffer::rewind(int mark)
{
	syncConsume();
	markerOffset=mark;
	nMarkers--;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
	}
#endif

