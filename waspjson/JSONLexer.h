#ifndef WASP_JSONLEXER_H
#define WASP_JSONLEXER_H

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define YY_DECL      wasp::JSONParser::token_type   wasp::JSONLexerImpl::lex( \
      wasp::JSONParser::semantic_type* yylval,         \
             wasp::JSONParser::location_type* yylloc          \
                 )
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer JSONFlexLexer
#include "waspcore/FlexLexer.h"
#undef yyFlexLexer
#endif

#include "JSONParser.hpp"
#include "waspcore/wasp_node.h"
#include "waspcore/decl.h"

namespace wasp {

/** JSONLexerImpl is a derived class to add some extra function to the scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to JSONLexer. However we change the context of the generated
 * yylex() function to be contained within the JSONLexerImpl class. This is required
 * because the yylex() defined in JSONLexer has no parameters. */
class WASP_PUBLIC JSONLexerImpl : public JSONFlexLexer
{
public:
    /** Create a new scanner object. The streams arg_yyin and arg_yyout default
     * to cin and cout, but that assignment is only made when initializing in
     * yylex(). */
    JSONLexerImpl(class AbstractInterpreter& interpreter,
                 std::istream* arg_yyin = 0,
           std::ostream* arg_yyout = 0);

    /** Required for virtual functions */
    virtual ~JSONLexerImpl();

    /** This is the main lexing function. It is generated by flex according to
     * the macro declaration YY_DECL above. The generated bison parser then
     * calls this virtual function to fetch new tokens. */
    virtual JSONParser::token_type lex(
       JSONParser::semantic_type* yylval,
       JSONParser::location_type* yylloc
       );

    /** Enable debug output (via arg_yyout) if compiled into the scanner. */
    void set_debug(bool b);
    /**
     * @brief rewind - rewind the last token (yyless(0))
     */
    void rewind();

    void capture_token(wasp::JSONParser::semantic_type* yylval,
                       wasp::NODE type);
private:
    class AbstractInterpreter & interpreter;
    std::size_t file_offset;
};

} // namespace wasp

#endif // WASP_JSONLEXER_H

