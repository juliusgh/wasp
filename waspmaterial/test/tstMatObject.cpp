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

TEST(JSON, material_object)
{
string massPath = "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NISTmasses.json";
MaterialCompositionLib mcl;
Masses ms;
Database db;
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ARH-600.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/CRC_ICdatabase.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/CRC_OCdatabase.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/doe-std-3013-2018.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRP89.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRU44.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRU46.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/MarksTable6.1.5.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_124_mats.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_126_table2.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/PNNL-15870r2_eff.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/RxHndbkVol1Ch19-Ch48.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/RxHndbkVol1Ch51.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/scaleAlloyMixs.json", massPath, std::cerr));
// Not working    ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/scaleCompounds.json", massPath, std::cerr));
ASSERT_TRUE(mcl.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ShultisAndFaw1996.json", massPath, std::cerr));

// ASSERT_TRUE(db.build("PNNL-15870r2_eff.json", std::cerr));
// ASSERT_TRUE(ms.build("C:/Users/k12jsti/source/repos/materialsdatabase/NISTmasses.json", std::cerr));
}