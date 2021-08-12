flex -Caf -v HIT.lex
sed -i '' 's/#include <FlexLexer.h>/#include \"waspcore\/FlexLexer.h\"/' HITLexer.cpp 
bison --version
bison HIT.bison --report=state --report=lookahead
sed -i '' 's/class HITParser/class WASP_PUBLIC HITParser/' HITParser.hpp
#bison HIT.bison --report=state --report=lookahead --report=solved  -g
