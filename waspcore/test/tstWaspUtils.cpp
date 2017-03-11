#include "gtest/gtest.h"

#include "waspcore/utils.h"
using namespace wasp;
/**
 * @brief TEST tests wild card pattern matching
 *
 */
TEST( utils, string_wilds)
{
    EXPECT_TRUE(wildcard_string_match("ge?ks*", "geeksforgeeks")); // Yes
    EXPECT_FALSE(wildcard_string_match("g*k", "gee"));  // No because 'k' is not in second
    EXPECT_FALSE(wildcard_string_match("*pqrs", "pqrst")); // No because 't' is not in first
    EXPECT_TRUE(wildcard_string_match("abc*bcd", "abcdhghgbcd")); // Yes
    EXPECT_FALSE(wildcard_string_match("abc*c?d", "abcd")); // No because second must have 2
                                                           // instances of 'c'
    EXPECT_TRUE(wildcard_string_match("*c*d", "abcd")); // Yes
    EXPECT_TRUE(wildcard_string_match("*?c*d", "abcd")); // Yes
    EXPECT_TRUE(wildcard_string_match("*", "could_be_anything")); // Yes
}


template<class T>
struct format_test{
    std::string format;
    T arg;
    std::string expected;
    format_test(const std::string & f, T a, const std::string & e)
    {
        format = f;
        arg = a;
        expected = e;
    }
};

TEST( utils, format_fixed_precision )
{
    std::vector<format_test<double>> tests={
        {"|%7f|",3.14159265,"|3.141593|"}
       ,{"|%8f|",3.14159265,"|3.141593|"}
       ,{"|%9f|",3.14159265,"| 3.141593|"}
       ,{"|%10f|",3.14159265,"|  3.141593|"}
       ,{"|%.0f|",3.14159265,"|3|"}
       ,{"|%.1f|",3.14159265,"|3.1|"}
       ,{"|%.2f|",3.14159265,"|3.14|"}
       ,{"|%.3f|",3.14159265,"|3.142|"}
       ,{"|%1.0f|",3.14159265,"|3|"}
       ,{"|%4.1f|",3.14159265,"| 3.1|"}
       ,{"|%4.8f|",3.14159265,"|3.14159265|"}
       ,{"|%8.2f|",3.14159265,"|    3.14|"}
       ,{"|%10.3f|",3.14159265,"|     3.142|"}
       ,{"|%01.0f|",3.14159265,"|3|"}
       ,{"|%04.1f|",3.14159265,"|03.1|"}
       ,{"|%04.8f|",3.14159265,"|3.14159265|"}
       ,{"|%08.2f|",3.14159265,"|00003.14|"}
       ,{"|%010.3f|",3.14159265,"|000003.142|"}
    };
    for( const auto & tst : tests )
    {
        SCOPED_TRACE(tst.format);
        std::cout<<"Testing fixed format of "<<tst.format<<std::endl;
        std::stringstream out, err;
        ASSERT_TRUE(wasp::format(out,err,tst.format.c_str(),tst.arg));
        EXPECT_EQ( tst.expected, out.str() );
    }
}

TEST( utils, format_scientific_precision )
{
    std::vector<format_test<double>> tests={
        {"|%.0g|",3.14159265,"|3|"}
        ,{"|%.1g|",3.14159265,"|3|"}
        ,{"|%.2g|",3.14159265,"|3.1|"}
        ,{"|%.3g|",3.14159265,"|3.14|"}
        ,{"|%1.0g|",3.14159265,"|3|"}
        ,{"|%4.1g|",3.14159265,"|   3|"}
        ,{"|%4.8g|",3.14159265,"|3.1415927|"}
        ,{"|%8.2g|",3.14159265,"|     3.1|"}
        ,{"|%10.3g|",3.14159265,"|      3.14|"}
        ,{"|%01.0g|",3.14159265,"|3|"}
        ,{"|%04.1g|",3.14159265,"|0003|"}
        ,{"|%04.8g|",3.14159265,"|3.1415927|"}
        ,{"|%08.2g|",3.14159265,"|000003.1|"}
        ,{"|%010.3g|",3.14159265,"|0000003.14|"}
        ,{"|%.4g|",1e-4,"|0.0001000|"}
        ,{"|%10.4g|",1e-4,"| 0.0001000|"}
        ,{"|%.5g|",1e-4,"|0.00010000|"}
        ,{"|%.5g|",1e-5,"|1.0000e-05|"}
        ,{"|%.6g|",1e-6,"|1.00000e-06|"}
        ,{"|%.7g|",1e-2,"|0.01000000|"}
        ,{"|%.7g|",1e-2,"|0.01000000|"}
        ,{"|%10.7g|",1e-2,"|0.01000000|"}
        ,{"|%10.7g|",1e2,"|  100.0000|"}
    };
    for( const auto & tst : tests )
    {
        SCOPED_TRACE(tst.format);
        std::cout<<"Testing scientific format of "<<tst.format<<std::endl;
        std::stringstream out, err;
        ASSERT_TRUE(wasp::format(out,err,tst.format.c_str(),tst.arg));
        EXPECT_EQ( tst.expected, out.str() );
    }
}
