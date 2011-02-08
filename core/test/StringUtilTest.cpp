#include "BaseTest.h"
#include "StringUtil.h"

#include <iostream>

using namespace std;
SUITE(StringUtilTest){
    class StringUtilTest{
    public:
        StringUtilTest(){
        };
        virtual ~StringUtilTest(){
        };
    };

    TEST_FIXTURE (StringUtilTest, should_replace_all_sub_strings)
    {
        string str="//localhost/test/another.java";

        string result = StringUtil::replaceAll(str, "/", "\\");

        CHECK_EQUAL("\\\\localhost\\test\\another.java", result);

        str="E:\\localhost/test/another.java";
        result = StringUtil::replaceAll(str, "/", "\\");
        result = StringUtil::replaceAll(str, "test", "testdest");

        CHECK_EQUAL("E:\\localhost\\testdest\\another.java", result);
    }

    TEST_FIXTURE (StringUtilTest, should_escape_quotes_in_string)
    {
        string str="it's impossible\". the end.";

        string result = StringUtil::escape(str);
        CHECK_EQUAL("it\\\'s impossible\\\". the end.", result);
    }

    TEST_FIXTURE (StringUtilTest, should_trim_string_from_both_side)
    {
        string str1="the word ";
        string str2=" the word";
        string str3=" the word ";

        string result = StringUtil::trim(str1, " ");
        CHECK_EQUAL("the word", result);

        result = StringUtil::trim(str2, " ");
        CHECK_EQUAL("the word", result);

        result = StringUtil::trim(str3, " ");
        CHECK_EQUAL("the word", result);
    }

    TEST_FIXTURE (StringUtilTest, should_trim_space_of_string_from_left)
    {
        string str1="the word ";
        string str2=" the word";
        string str3=" the word ";

        string result = StringUtil::trimLeft(str1, " ");
        CHECK_EQUAL("the word ", result);

        result = StringUtil::trimLeft(str2, " ");
        CHECK_EQUAL("the word", result);

        result = StringUtil::trimLeft(str3, " ");
        CHECK_EQUAL("the word ", result);
    }

    TEST_FIXTURE (StringUtilTest, should_trim_space_of_string_from_right)
    {
        string str1="the word ";
        string str2=" the word";
        string str3=" the word ";

        CHECK_EQUAL("the word", StringUtil::trimRight(str1, " "));
        CHECK_EQUAL(" the word", StringUtil::trimRight(str2, " "));
        CHECK_EQUAL(" the word", StringUtil::trimRight(str3, " "));
    }

    TEST_FIXTURE (StringUtilTest, should_trim_all_specified_characters_from_both_side)
    {
        string str1="\tthe word \n ";
        string str2="\n the word \n ";
        string str3="\n the word \n";

        string result = StringUtil::trim(str1, "\n\t ");
        CHECK_EQUAL("the word", result);

        result = StringUtil::trim(str2, "\n\t ");
        CHECK_EQUAL("the word", result);

        result = StringUtil::trim(str3, "\n\t ");
        CHECK_EQUAL("the word", result);
    }

    TEST_FIXTURE (StringUtilTest, should_trim_all_specified_characters_from_left)
    {
        string str1="\tthe word \n ";
        string str2="\n the word \n ";
        string str3="\n the word \n";

        string result = StringUtil::trimLeft(str1, "\n\t ");
        CHECK_EQUAL("the word \n ", result);

        result = StringUtil::trimLeft(str2, "\n\t ");
        CHECK_EQUAL("the word \n ", result);

        result = StringUtil::trimLeft(str3, "\n\t ");
        CHECK_EQUAL("the word \n", result);
    }

    TEST_FIXTURE (StringUtilTest, should_trim_the_first_specified_characters_from_left)
    {
        string str1="\tthe word \n ";
        string str2="\n the word \n ";
        string str3="\n the word \n";

        string result = StringUtil::trimLeft(str1, "\n\t ", false);
        CHECK_EQUAL("the word \n ", result);

        result = StringUtil::trimLeft(str2, "\n\t ", false);
        CHECK_EQUAL(" the word \n ", result);

        result = StringUtil::trimLeft(str3, "\n\t ", false);
        CHECK_EQUAL(" the word \n", result);
    }

    TEST_FIXTURE (StringUtilTest, should_check_if_a_string_starts_with_another)
    {
        string str1="the word a";

        CHECK(StringUtil::startsWith(str1, "the"));
        CHECK(StringUtil::startsWith(str1, "the "));

        CHECK(!StringUtil::startsWith(str1, "the word b"));
    }

    TEST_FIXTURE (StringUtilTest, should_check_if_a_string_ends_with_another)
    {
        string str1="the word";

        CHECK(StringUtil::endsWith(str1, "word"));
        CHECK(StringUtil::endsWith(str1, "the word"));

        CHECK(!StringUtil::endsWith(str1, "a word"));
    }

    TEST_FIXTURE (StringUtilTest, should_check_if_a_string_contains_with_another)
    {
        string str1="the word a";

        CHECK(StringUtil::contains(str1, "word"));
        CHECK( ! StringUtil::contains(str1, "word-not-exist"));
    }

    TEST_FIXTURE (StringUtilTest, should_split_string_by_delimiter)
    {
        string str1="key=value";
        vector<string> expected;
        expected.push_back("key");
        expected.push_back("value");

        CHECK_ARRAY_EQUAL(StringUtil::split("key=value", "=", 2), expected, expected.size());
        CHECK_ARRAY_EQUAL(StringUtil::split("key = value", "=", 2), expected, expected.size());
        CHECK_ARRAY_EQUAL(StringUtil::split(" \tkey \t= \tvalue\t ", "=", 2), expected, expected.size());
    }

    TEST_FIXTURE (StringUtilTest, should_split_string_up_to_specified_parts_by_delimiter)
    {
        string str1="key=value=value2";
        vector<string> expected;
        expected.push_back("key");
        expected.push_back("value=value2");

        CHECK_ARRAY_EQUAL(StringUtil::split(str1, "=", 2), expected, expected.size());
    }

    TEST_FIXTURE (StringUtilTest, should_trim_string_when_split)
    {
        string str1="table1, table2 , \t table3";
        vector<string> expected;
        expected.push_back("table1");
        expected.push_back("table2");
        expected.push_back("table3");

        CHECK_ARRAY_EQUAL(StringUtil::split(str1, ","), expected, expected.size());
    }

    TEST_FIXTURE (StringUtilTest, should_check_strings_case_insensitive_equals)
    {
        string str1="key=Value";
        CHECK(StringUtil::equalsIgnoreCase("Key=Value", "KEY=VALue"));
        CHECK(!StringUtil::equalsIgnoreCase("Key=Value", "KEY=VALue1"));
    }

}
