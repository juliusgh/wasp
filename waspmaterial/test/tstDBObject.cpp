#include <cstdlib>
#include <sstream>
#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "waspjson/JSONObjectParser.hpp"
#include "waspcore/Object.h"
#include "googletest\googletest\include\gtest\gtest.h"
#include "waspmaterial/MaterialDatabase.h"

TEST(JSON, material_object)
{
Database db;

    // DB buid tests
// ASSERT_TRUE(db.build("../materials/ARH-600.json", std::cerr));

    // Internal Checks
db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/PNNL-15870r2_eff.json", std::cerr);
for (int i=0; i<db.getMats(); i++) {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
}