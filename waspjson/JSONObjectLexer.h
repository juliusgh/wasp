#ifndef WASP_JSONOBJECTLEXER_H
#define WASP_JSONOBJECTLEXER_H

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define YY_DECL                                                        \
    wasp::JSONObjectParser::token_type wasp::JSONObjectLexerImpl::lex( \
        wasp::JSONObjectParser::semantic_type* yylval,                 \
        wasp::JSONObjectParser::location_type* yylloc)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer JSONObjectFlexLexer
#include "waspcore/FlexLexer.h"
#undef yyFlexLexer
#endif

#include "JSONObjectParser.hpp"
#include "waspcore/wasp_node.h"
#include "waspcore/decl.h"

namespace wasp
{
/** JSONObjectLexerImpl is a derived class to add some extra function to the
 * scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to JSONObjectLexer. However we change the context of the generated
 * yylex() function to be contained within the JSONObjectLexerImpl class. This
 * is required
 * because the yylex() defined in JSONObjectLexer has no parameters. */
class WASP_PUBLIC JSONObjectLexerImpl : public JSONObjectFlexLexer
{
  public:
    /** Create a new scanner object. The streams arg_yyin and arg_yyout default
     * to cin and cout, but that assignment is only made when initializing in
     * yylex(). */
    JSONObjectLexerImpl(std::istream* arg_yyin  = 0,
                        std::ostream* arg_yyout = 0);

    /** Required for virtual functions */
    virtual ~JSONObjectLexerImpl();

    /** This is the main lexing function. It is generated by flex according to
     * the macro declaration YY_DECL above. The generated bison parser then
     * calls this virtual function to fetch new tokens. */
    virtual JSONObjectParser::token_type
    lex(JSONObjectParser::semantic_type* yylval,
        JSONObjectParser::location_type* yylloc);

    /** Enable debug output (via arg_yyout) if compiled into the scanner. */
    void set_debug(bool b);

  private:
};

}  // namespace wasp

#endif  // WASP_JSONOBJECTLEXER_H
