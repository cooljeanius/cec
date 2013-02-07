#ifndef INC_EsterelLexer_hpp_
#define INC_EsterelLexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.2: "esterel.g" -> "EsterelLexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "EsterelTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
#line 1 "esterel.g"

#include "LineAST.hpp"

#line 16 "EsterelLexer.hpp"
class EsterelLexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public EsterelTokenTypes
{
#line 1 "esterel.g"
#line 20 "EsterelLexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	EsterelLexer(ANTLR_USE_NAMESPACE(std)istream& in);
	EsterelLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	EsterelLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mPERIOD(bool _createToken);
	public: void mPOUND(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mDASH(bool _createToken);
	public: void mSLASH(bool _createToken);
	public: void mSTAR(bool _createToken);
	public: void mPARALLEL(bool _createToken);
	public: void mLESSTHAN(bool _createToken);
	public: void mGREATERTHAN(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mEQUALS(bool _createToken);
	public: void mSEMICOLON(bool _createToken);
	public: void mCOLON(bool _createToken);
	public: void mCOLEQUALS(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mLBRACKET(bool _createToken);
	public: void mRBRACKET(bool _createToken);
	public: void mQUESTION(bool _createToken);
	public: void mDQUESTION(bool _createToken);
	public: void mLEQUAL(bool _createToken);
	public: void mGEQUAL(bool _createToken);
	public: void mNEQUAL(bool _createToken);
	public: void mIMPLIES(bool _createToken);
	public: void mID(bool _createToken);
	public: void mNumber(bool _createToken);
	protected: void mExponent(bool _createToken);
	public: void mFractionalNumber(bool _createToken);
	public: void mStringConstant(bool _createToken);
	public: void mWhitespace(bool _createToken);
	public: void mNewline(bool _createToken);
	public: void mComment(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
};

#endif /*INC_EsterelLexer_hpp_*/
