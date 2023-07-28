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
Masses m;
Database db;

//     Masses Checks
ASSERT_TRUE(m.build("/materials/NISTmasses.json", std::cerr));
// m.build("/materials/NISTmasses.json", std::cerr);
ASSERT_TRUE(m.check());

    // Internal Checks
    // TODO: Add and verify testing for all code input formats and types
ASSERT_TRUE(db.build("/materials/ARH-600.json", std::cerr));
EXPECT_TRUE(db.check()); // Checks the entire database for compositional consistency but does not throw an exception
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check()); // Does the same check individually but throws an exception if found.
}


db.build("/materials/CRC_ICdatabase.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/CRC_OCdatabase.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/doe-std-3013-2018.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/ICRP89.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/ICRU44.json", std::cerr); //12 sets of fracs in db that don't sum to 1
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/ICRU46.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/MarksTable6.1.5.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/MCNP5APrimer.json", std::cerr); //3 sets of fracs in db that don't sum to 1
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/NIST_124_mats.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/NIST_126_table2.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/PNNL-15870r2_eff.json", std::cerr); //1 set of fracs in db that doesn't sum to 1
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    auto mat = db.getMat(i);
    ASSERT_TRUE(mat.check());
    
    // #1) WF Iso- Tests are not complete
    string code = mat.getInputFormat("MAVRIC/KENO", "Weight Fractions", "Isotopic", db.getDB());
    while (code.find("\n")!=-1) {code.replace(code.find("\n"), 1, "");}
    
    // Possibly to remove "\\" from values stored in the map
    // while (scalePNNLWFIsoMap[mat.getName()].find("\\") != -1) {scalePNNLWFIsoMap[mat.getName()].replace(scalePNNLWFIsoMap[mat.getName()].find("\\"), 1, "");}
    // cout << mat.getName() << endl;

    EXPECT_TRUE(scalePNNLWFIsoMap[mat.getName()] == code);

    code = mat.getInputFormat("ORIGEN", "Weight Fractions", "Isotopic", db.getDB());
    while (code.find("\n")!=-1) {code.replace(code.find("\n"), 1, "");}
    EXPECT_TRUE(origenPNNLWFIsoMap[mat.getName()] == code);

    code = mat.getInputFormat("MCNP", "Weight Fractions", "Isotopic", db.getDB());
    while (code.find("\n")!=-1) {code.replace(code.find("\n"), 1, "");}
    EXPECT_TRUE(mcnpPNNLWFIsoMap[mat.getName()] == code);

    code = mat.getInputFormat("Generic", "Weight Fractions", "Isotopic", db.getDB());
    while (code.find("\n")!=-1) {code.replace(code.find("\n"), 1, "");}
    EXPECT_TRUE(genPNNLWFIsoMap[mat.getName()] == code);
}

db.build("/materials/RxHndbkVol1Ch19-Ch48.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/RxHndbkVol1Ch51.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/scaleAlloyMixs.json", std::cerr); //1 set of fracs in db that doesn't sum to 1
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/scaleCompounds.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

db.build("/materials/ShultisAndFaw1996.json", std::cerr);
EXPECT_TRUE(db.check());
for (int i=0; i<db.getMats(); i++) {
    ASSERT_TRUE(db.getMat(i).check());
}

}