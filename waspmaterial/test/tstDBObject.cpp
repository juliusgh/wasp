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
// for (auto mat: db.getMats()) {ASSERT_TRUE(mat.checkFractions());}
}