#ifndef HALITE_DATA_ACCESSOR_H
#define HALITE_DATA_ACCESSOR_H

#include "waspcore/Object.h"
#include "waspexpr/ExprContext.h"

namespace wasp{

class DataAccessor : public Context {

public:
    DataAccessor();
    DataAccessor(const DataAccessor& orig);
    virtual ~DataAccessor();

private:
};

} // wasp
#endif
