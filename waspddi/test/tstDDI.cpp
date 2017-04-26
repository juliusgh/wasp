#include "waspddi/DDInterpreter.h"
#include "waspddi/DDINodeView.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
using namespace wasp;

TEST(DDInterpreter,comments)
{
    std::stringstream input;
    input <<R"I( # comments
 #comments here
 #comments there

 # comments everywhere
)I"<<std::endl;
    DDInterpreter<> ddi;
    ASSERT_TRUE( ddi.parse(input) );
    ASSERT_EQ(4, ddi.root().child_count());
}

TEST(DDInterpreter,comment_placement)
{
    std::stringstream input;
    input <<R"I( # comment of document
 sect1 "a"
    # comment for section 1
    sect1.1,
        sect1.1.1 # comment for sect 1.1.1
        #comment still 1.1.1
        sect1.1.2
            #comment for 1.1.2
    sect1.2
        # comment of 1.2
        sect1.2.1
# sect 1.2.1
        sect1.2.2,
        sect1.2.3 = "value"
    #sect 1.2.3
    sect1.3
 sect2
)I"<<std::endl;
    DDInterpreter<> ddi;
    auto * sect1 = ddi.definition()->create("sect1");
    auto * sect1_1 = sect1->create("sect1.1");
    sect1_1->create("sect1.1.1");
    sect1_1->create("sect1.1.2");
    auto * sect1_2 = sect1->create("sect1.2");
    sect1_2->create("sect1.2.1");
    sect1_2->create("sect1.2.2");
    sect1_2->create("sect1.2.3");
    auto * sect1_3 = sect1->create("sect1.3");
    ddi.definition()->create("sect2");
    EXPECT_TRUE( ddi.parse(input) );
    std::stringstream paths;
    ddi.root().paths(paths);
    std::stringstream expected;
    expected <<R"I(/
/comment (# comment of document)
/sect1
/sect1/decl (sect1)
/sect1/value ("a")
/sect1/comment (# comment for section 1)
/sect1/sect1.1
/sect1/sect1.1/decl (sect1.1)
/sect1/sect1.1/, (,)
/sect1/sect1.1/sect1.1.1
/sect1/sect1.1/sect1.1.1/decl (sect1.1.1)
/sect1/sect1.1/sect1.1.1/comment (# comment for sect 1.1.1)
/sect1/sect1.1/sect1.1.1/comment (#comment still 1.1.1)
/sect1/sect1.1/sect1.1.2
/sect1/sect1.1/sect1.1.2/decl (sect1.1.2)
/sect1/sect1.1/sect1.1.2/comment (#comment for 1.1.2)
/sect1/sect1.2
/sect1/sect1.2/decl (sect1.2)
/sect1/sect1.2/comment (# comment of 1.2)
/sect1/sect1.2/sect1.2.1
/sect1/sect1.2/sect1.2.1/decl (sect1.2.1)
/sect1/sect1.2/sect1.2.1/comment (# sect 1.2.1)
/sect1/sect1.2/sect1.2.2
/sect1/sect1.2/sect1.2.2/decl (sect1.2.2)
/sect1/sect1.2/sect1.2.2/, (,)
/sect1/sect1.2/sect1.2.3
/sect1/sect1.2/sect1.2.3/decl (sect1.2.3)
/sect1/sect1.2/sect1.2.3/= (=)
/sect1/sect1.2/sect1.2.3/value ("value")
/sect1/sect1.2/sect1.2.3/comment (#sect 1.2.3)
/sect1/sect1.3
/sect1/sect1.3/decl (sect1.3)
/sect2
/sect2/decl (sect2)
)I";

    ASSERT_EQ(expected.str(), paths.str());
}
TEST(DDInterpreter,passing_flat)
{
    std::stringstream input;
    input <<R"I( sect1
sect2 1
sect3 = 1
sect4 1.1
sect5 = 1.e29
sect6 "ted"
sect7 = "ted"
sect8 = 1 2 3.4 "five"
sect9  1 2 3.4 "five"
)I"<<std::endl;
    DDInterpreter<> ddi;
    ddi.definition()->create("sect1");
    ddi.definition()->create("sect2");
    ddi.definition()->create("sect3");
    ddi.definition()->create("sect4");
    ddi.definition()->create("sect5");
    ddi.definition()->create("sect6");
    ddi.definition()->create("sect7");
    ddi.definition()->create("sect8");
    ddi.definition()->create("sect9");
    ASSERT_TRUE( ddi.parse(input) );
    std::stringstream expected;
expected<<R"I(/
/sect1
/sect1/decl (sect1)
/sect2
/sect2/decl (sect2)
/sect2/value (1)
/sect3
/sect3/decl (sect3)
/sect3/= (=)
/sect3/value (1)
/sect4
/sect4/decl (sect4)
/sect4/value (1.1)
/sect5
/sect5/decl (sect5)
/sect5/= (=)
/sect5/value (1.e29)
/sect6
/sect6/decl (sect6)
/sect6/value ("ted")
/sect7
/sect7/decl (sect7)
/sect7/= (=)
/sect7/value ("ted")
/sect8
/sect8/decl (sect8)
/sect8/= (=)
/sect8/value (1)
/sect8/value (2)
/sect8/value (3.4)
/sect8/value ("five")
/sect9
/sect9/decl (sect9)
/sect9/value (1)
/sect9/value (2)
/sect9/value (3.4)
/sect9/value ("five")
)I";
    std::stringstream paths;
    DDINodeView<decltype(ddi.root())> root = ddi.root();
    root.paths(paths);
    ASSERT_EQ(expected.str(), paths.str());

    ASSERT_EQ(1,root.child_at(1) // sect2
       .to_int());
}

TEST(DDInterpreter,passing_simple_sections)
{
    std::stringstream input;
    input <<R"I( sect1 1
sect2 2
    sect2.2 2.2
)I"<<std::endl;
    DDInterpreter<> ddi;
    ddi.definition()->create("sect1")->create("sect1.1");
    ddi.definition()->create("sect2")->create("sect2.2");
    ASSERT_TRUE( ddi.parse(input) );
    std::stringstream expected;
expected<<R"I(/
/sect1
/sect1/decl (sect1)
/sect2/value (1)
/sect2
/sect2/decl (sect2)
/sect2/value (2)
/sect2/sec2.2
/sect2/sect2.2/decl (sect2.2)
/sect2/sect2.2/value (2.2)
)I";
    std::stringstream paths;
    ddi.root().paths(paths);
}

/**
 * @brief TEST ensures that an unknown section produces an expected error
 * Specifically tests unknown in 'decl values' logic
 */
TEST(DDInterpreter,failing_decl_values)
{
    std::stringstream input;
    input <<R"I( sect1
sect_eek 1
)I"<<std::endl;
    std::stringstream errors;
    DDInterpreter<> ddi(errors);
    ddi.definition()->create("sect1");

    EXPECT_FALSE( ddi.parse(input) );
    std::string msg = "stream input:2.1-8: 'sect_eek' is unknown.";
    ASSERT_EQ(msg, errors.str());

}
/**
 * @brief TEST ensures that an unknown section produces an expected error
 * Specifically tests unknown in 'decl' logic
 */
TEST(DDInterpreter,failing_decl)
{
    std::stringstream input;
    input <<R"I( sect1
 sect_eek
)I"<<std::endl;
    std::stringstream errors;
    DDInterpreter<> ddi(errors);
    ddi.definition()->create("sect1");

    EXPECT_FALSE( ddi.parse(input) );
    std::string msg = "stream input:2.2-9: 'sect_eek' is unknown.";
    ASSERT_EQ(msg, errors.str());

}
/**
 * @brief TEST ensures that an unknown section produces an expected error
 * Specifically tests unknown in 'decl=values' logic
 */
TEST(DDInterpreter,failing_decl_assign_values)
{
    std::stringstream input;
    input <<R"I( sect1
  boom="boom"
)I"<<std::endl;
    std::stringstream errors;
    DDInterpreter<> ddi(errors);
    ddi.definition()->create("sect1");

    EXPECT_FALSE( ddi.parse(input) );
    std::string msg = "stream input:2.3-6: 'boom' is unknown.";
    ASSERT_EQ(msg, errors.str());

}
