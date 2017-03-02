#ifndef WASP_DDIINTERPRETER_I_H
#define WASP_DDIINTERPRETER_I_H

template<class S>
DDIInterpreter<S>::DDIInterpreter()
    :Interpreter<S>(),
    traceLexing(false),
    traceParsing(false),
    singleParse(false),
    mHasFile(false)
{
}
template<class S>
DDIInterpreter<S>::DDIInterpreter(std::ostream & err)
    : Interpreter<S>(err),
    traceLexing(false),
    traceParsing(false),
    singleParse(false),
    mHasFile(false)
{
}
template<class S>
DDIInterpreter<S>::~DDIInterpreter()
{
}
template<class S>
bool DDIInterpreter<S>::parse(std::istream& in, size_t startLine, size_t startColumn)
{
    return parseStream(in, hasFile() ? Interpreter<S>::stream_name() : "stream input" , startLine,startColumn);
}
template<class S>
bool DDIInterpreter<S>::parseStream(std::istream& in
                                 , const std::string& sname
                                 , size_t start_line
                                 , size_t start_column)
{
    return Interpreter<S>::template parse_impl<DDIParser>(in
                ,sname
                ,start_line
                ,start_column);
}
template<class S>
bool DDIInterpreter<S>::parseFile(const std::string &filename, size_t line)
{
    std::ifstream in(filename.c_str());
    if (!in.good()){
        Interpreter<S>::error_stream()<<"file '"<<filename<<"' is either inaccessible or doesn't exist! Unable to read."<<std::endl;
        return false;
    }
    mHasFile = true;
    return parseStream(in, filename, line);
}
template<class S>
bool DDIInterpreter<S>::parseString(const std::string &input, const std::string& sname, size_t startLine, size_t startColumn)
{
    std::istringstream iss(input);
    return parseStream(iss, sname,startLine,startColumn);
}

template<class S>
const Definition::SP& DDIInterpreter<S>::definition()const
{
    wasp_require( m_definition != nullptr );
    return m_definition;
}
template<class S>
Definition::SP& DDIInterpreter<S>::definition()
{
    if( m_definition == nullptr ) m_definition = std::make_shared<Definition>();
    return m_definition;
}
#endif
