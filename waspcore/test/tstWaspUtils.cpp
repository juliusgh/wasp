#include "gtest/gtest.h"

#include "waspcore/utils.h"
using namespace wasp;
/**
 * @brief TEST tests wild card pattern matching
 *
 */
TEST(utils, string_wilds)
{
    EXPECT_TRUE(wildcard_string_match("ge?ks*", "geeksforgeeks"));  // Yes
    EXPECT_FALSE(wildcard_string_match(
        "g*k", "gee"));  // No because 'k' is not in second
    EXPECT_FALSE(wildcard_string_match(
        "*pqrs", "pqrst"));  // No because 't' is not in first
    EXPECT_TRUE(wildcard_string_match("abc*bcd", "abcdhghgbcd"));  // Yes
    EXPECT_FALSE(wildcard_string_match(
        "abc*c?d", "abcd"));  // No because second must have 2
                              // instances of 'c'
    EXPECT_TRUE(wildcard_string_match("*c*d", "abcd"));            // Yes
    EXPECT_TRUE(wildcard_string_match("*?c*d", "abcd"));           // Yes
    EXPECT_TRUE(wildcard_string_match("*", "could_be_anything"));  // Yes
}

TEST(utils, trim)
{
    EXPECT_EQ("str", trim(" str    ", " "));
    EXPECT_EQ("str", trim("str", " "));
    EXPECT_EQ("str", trim("str  ", " "));
    EXPECT_EQ("str", trim("  str", " "));
    EXPECT_EQ("tr", trim("strs", " s"));
}
TEST(utils, strip_quotes)
{
    EXPECT_EQ("", strip_quotes("\"\""));      // empty string
    EXPECT_EQ("  ", strip_quotes("\"  \""));  // empty string
    EXPECT_EQ("", strip_quotes("''"));        // empty string
    EXPECT_EQ("  ", strip_quotes("'  '"));    // empty string

    EXPECT_EQ("ted", strip_quotes("ted"));    // no quotes
    EXPECT_EQ("ted'", strip_quotes("ted'"));  // single quotes
    EXPECT_EQ("'ted", strip_quotes("'ted"));  // single quotes
    EXPECT_EQ(" ted ", strip_quotes("\" ted \""));
}

TEST(utils, dir_name)
{
    EXPECT_EQ("", dir_name("/"));
    EXPECT_EQ("", dir_name("/ted"));
    EXPECT_EQ("c:", dir_name("c:\\"));
    EXPECT_EQ("c:", dir_name("c:\\ted"));

    EXPECT_EQ("/path/to", dir_name("/path/to/stuff"));
    EXPECT_EQ("/path/to/stuff", dir_name("/path/to/stuff/"));
    EXPECT_EQ("c:\\path\\to", dir_name("c:\\path\\to\\stuff"));
    EXPECT_EQ("c:\\path\\to\\stuff", dir_name("c:\\path\\to\\stuff\\"));
}
