#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "waspcore/version.h"
#include "waspcore/wasp_bug.h"
#include "waspgetpot/GetPotInterpreter.h"
using namespace wasp;

int main (int argc, char *argv[])
{
    if( argc == 1 )
    {
        std::cout<<"Workbench Analysis Sequence Processor (GetPot)"
                <<std::endl
                <<argv[0]<<" : An application for listing GetPot formatted input."<<std::endl;
        std::cout<<" Usage : "<<argv[0]<<" path/to/GetPot/formatted/input"
                <<std::endl;
        return 1;
    }
    for( std::size_t j = 1; j < argc; ++j)
    {
        std::ifstream input(argv[j]);
        if (input.fail() || input.bad()) {
            std::cout << "Failed to open/read " << argv[j] << std::endl;
            input.close();
            return 1;
        }
        std::stringstream errors;
        GetPotInterpreter<> interpreter(errors);
        wasp_timer(parse_time);
        wasp_timer_start(parse_time);
        bool parsed = interpreter.parse(input);
        wasp_timer_stop(parse_time);
        wasp_timer_block(std::cout << "Parse Timer duration: "
                         << parse_time.duration()
                         << " nanoseconds with "
                         << parse_time.intervals()
                         << " invervals" << std::endl);
        std::cout<<"Listing for '"<<argv[j]<<"'"<<std::endl;
        interpreter.root().paths(std::cout);
        if( !parsed )
        {
            std::cout<<errors.str()<<std::endl;
            return -1;
        }
    }
    return 0;
}
