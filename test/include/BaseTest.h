#ifndef BASE_TEST_H_
#define BASE_TEST_H_

#include "UnitTest++.h"

using namespace UnitTest;
using namespace std;

#include "TestEnvironment.h"

namespace UnitTest {
template< typename Expected, typename Actual >
void CheckNotEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
{
    if ((expected == actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected not " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

void CheckNotEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);

void CheckNotEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);

void CheckNotEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);

void CheckNotEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);


#define FAIL(message) \
    do \
    { \
        try { \
                UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), message); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK(" message ")"); \
        } \
    } while (0)

#define ASSERT_EQUAL(actual, expected) \
	CHECK_EQUAL(expected, actual)

#define ASSERT_NOT_EQUAL(actual, expected) \
    do \
    { \
        try { \
            UnitTest::CheckNotEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
                    "Unhandled exception in CHECK_NOT_EQUAL(" #expected ", " #actual ")"); \
        } \
    } while (0)
}

#endif /* BASE_TEST_H_ */
