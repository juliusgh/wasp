#include "GetPotInterpreter.h"

namespace wasp{

GetPotInterpreter::GetPotInterpreter(std::ostream & err)
    : Interpreter(err)
    ,m_lexer(nullptr)
{
}
GetPotInterpreter::~GetPotInterpreter()
{
}

bool GetPotInterpreter::parse(std::istream& in
                                        , size_t startLine
                                        , size_t startColumn)
{
    return parse_impl<GetPotLexerImpl
            ,GetPotParser
            ,GetPotInterpreter>(m_lexer
                ,in
                ,"input"
                ,startLine
                ,startColumn);
}

} // end of namespace
