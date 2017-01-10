#ifndef WASP_GETPOTLEXER_H
#define WASP_GETPOTLEXER_H


// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define YY_DECL      wasp::GetPotParser::token_type   wasp::GetPotLexerImpl::lex( \
      wasp::GetPotParser::semantic_type* yylval,         \
             wasp::GetPotParser::location_type* yylloc          \
                 )
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer GetPotFlexLexer
#include "waspcore/FlexLexer.h"
#undef yyFlexLexer
#endif

#include "GetPotParser.hpp"
#include "waspcore/wasp_node.h"
namespace wasp {

/** GetPotLexerImpl is a derived class to add some extra function to the scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to GetPotLexer. However we change the context of the generated
 * yylex() function to be contained within the GetPotLexerImpl class. This is required
 * because the yylex() defined in GetPotLexer has no parameters. */
class GetPotLexerImpl : public GetPotFlexLexer
{
public:
    /** Create a new scanner object. The streams arg_yyin and arg_yyout default
     * to cin and cout, but that assignment is only made when initializing in
     * yylex().
     * The token pool is the storage class
    */
    GetPotLexerImpl(class AbstractInterpreter & interpreter,
            std::istream* arg_yyin = 0,
           std::ostream* arg_yyout = 0);

    /** Required for virtual functions */
    virtual ~GetPotLexerImpl();

    /** This is the main lexing function. It is generated by flex according to
     * the macro declaration YY_DECL above. The generated biGetPot parser then
     * calls this virtual function to fetch new tokens. */
    virtual GetPotParser::token_type lex(
       GetPotParser::semantic_type* yylval,
       GetPotParser::location_type* yylloc
       );
    
    /** Enable debug output (via arg_yyout) if compiled into the scanner. */
    void set_debug(bool b);
    /**
     * @brief rewind - rewind the last token (yyless(0))
     */
    void rewind();    

    void capture_token(wasp::GetPotParser::semantic_type* yylval,
                       wasp::NODE type);

private:
    class AbstractInterpreter & interpreter;
    std::size_t file_offset;

};

} // namespace wasp

#endif // WASP_GETPOTLEXER_H

