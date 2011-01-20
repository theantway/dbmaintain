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

    TEST_FIXTURE (StringUtilTest, ReplaceAll)
    {
        string str="//localhost/test/another.java";

        string result = StringUtil::replaceAll(str, "/", "\\");

        CHECK_EQUAL("\\\\localhost\\test\\another.java", result);

        str="E:\\localhost/test/another.java";
        result = StringUtil::replaceAll(str, "/", "\\");
        result = StringUtil::replaceAll(str, "test", "testdest");

        CHECK_EQUAL("E:\\localhost\\testdest\\another.java", result);
    }

    TEST_FIXTURE (StringUtilTest, Escape)
    {
        string str="it's impossible\". the end.";

        string result = StringUtil::escape(str);
        CHECK_EQUAL("it\\\'s impossible\\\". the end.", result);
    }

    TEST_FIXTURE (StringUtilTest, TrimSpace)
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

    TEST_FIXTURE (StringUtilTest, LeftTrimSpace)
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

    TEST_FIXTURE (StringUtilTest, RightTrimSpace)
    {
        string str1="the word ";
        string str2=" the word";
        string str3=" the word ";

        CHECK_EQUAL("the word", StringUtil::trimRight(str1, " "));
        CHECK_EQUAL(" the word", StringUtil::trimRight(str2, " "));
        CHECK_EQUAL(" the word", StringUtil::trimRight(str3, " "));
    }

    TEST_FIXTURE (StringUtilTest, TrimMultipleChars)
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

    TEST_FIXTURE (StringUtilTest, TrimLeftMultipleChars)
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

    TEST_FIXTURE (StringUtilTest, TrimLeftFirstMultipleChars)
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

    TEST_FIXTURE (StringUtilTest, StartWithString)
    {
        string str1="the word a";

        CHECK(StringUtil::startsWith(str1, "the"));
        CHECK(StringUtil::startsWith(str1, "the "));

        CHECK(!StringUtil::startsWith(str1, "the word b"));
    }

    TEST_FIXTURE (StringUtilTest, EndWithString)
    {
        string str1="the word";

        CHECK(StringUtil::endsWith(str1, "word"));
        CHECK(StringUtil::endsWith(str1, "the word"));

        CHECK(!StringUtil::endsWith(str1, "a word"));
    }

    TEST_FIXTURE (StringUtilTest, ContainsString)
    {
        string str1="the word a";

        CHECK(StringUtil::contains(str1, "word"));
        CHECK( ! StringUtil::contains(str1, "word-not-exist"));
    }

    TEST_FIXTURE (StringUtilTest, SplitString)
    {
        string str1="key=value";
        vector<string> expected;
        expected.push_back("key");
        expected.push_back("value");

        CHECK_ARRAY_EQUAL(StringUtil::split("key=value", "=", 2), expected, expected.size());
        CHECK_ARRAY_EQUAL(StringUtil::split("key = value", "=", 2), expected, expected.size());
        CHECK_ARRAY_EQUAL(StringUtil::split(" \tkey \t= \tvalue\t ", "=", 2), expected, expected.size());
    }

    TEST_FIXTURE (StringUtilTest, SplitStringWithMultipleDelimiters)
    {
        string str1="key=value=value2";
        vector<string> expected;
        expected.push_back("key");
        expected.push_back("value=value2");

        CHECK_ARRAY_EQUAL(StringUtil::split("key=value=value2", "=", 2), expected, expected.size());
    }

}
