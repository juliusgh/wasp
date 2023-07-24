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
Masses m;

//     Masses Checks
ASSERT_TRUE(m.build("/materials/NISTmasses.json", std::cerr));
m.build("/materials/NISTmasses.json", std::cerr);
for (int j=0; j<m.getElems(); j++) {
    ASSERT_TRUE(m.getElem(j).checkAbundances());
}

    // Internal Checks

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ARH-600.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/CRC_ICdatabase.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/CRC_OCdatabase.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/doe-std-3013-2018.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRP89.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRU44.json", std::cerr); //12 sets of fracs in db that don't sum to 1
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ICRU46.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/MarksTable6.1.5.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/MCNP5APrimer.json", std::cerr); //3 sets of fracs in db that don't sum to 1
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_124_mats.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_126_table2.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/PNNL-15870r2_eff.json", std::cerr); //1 set of fracs in db that doesn't sum to 1
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/RxHndbkVol1Ch19-Ch48.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/RxHndbkVol1Ch51.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/scaleAlloyMixs.json", std::cerr); //1 set of fracs in db that doesn't sum to 1
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

    // Not Working
// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/scaleCompounds.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

// db.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/ShultisAndFaw1996.json", std::cerr);
// for (int i=0; i<db.getMats(); i++) {
//     if (db.getMat(i).getType() == "Atoms Per Molecule") {ASSERT_NEAR(0, db.getMat(i).checkFractions(), 0.00001);}
//     ASSERT_TRUE(db.getMat(i).check());
//     }

}