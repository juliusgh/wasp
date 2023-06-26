#include <cstdlib>
#include <sstream>
#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "waspjson/JSONObjectParser.hpp"
#include "waspcore/Object.h"
#include "googletest\googletest\include\gtest\gtest.h"
// #include "waspmaterial/MaterialDatabase.h"
// #include "waspmaterial/MassDatabase.h"
#include "waspmaterial/MatCompLib.h"
using namespace wasp;

TEST(JSON, material_object)
{

MaterialCompositionLib mcl;
// Masses ms;
// Database db;
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/CDC_ICdatabase.json", "C:/Users/k12jsti/source/repos/materialsdatabase/NISTmasses.json", std::cerr));
// ASSERT_TRUE(db.build("C:/Users/k12jsti/source/repos/materialsdatabase/ARH-600.json", std::cerr));
// ASSERT_TRUE(ms.build("C:/Users/k12jsti/source/repos/materialsdatabase/NISTmasses.json", std::cerr));
}
// "C:/Users/k12jsti/source/repos/materialsdatabase/NIST_124_mats.json"
// "C:/Users/k12jsti/source/repos/materialsdatabase/ARH-600.json"
// "C:/Users/k12jsti/source/repos/materialsdatabase/ShultisAndFaw1996.json"