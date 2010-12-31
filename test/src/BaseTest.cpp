#include "BaseTest.h"

namespace UnitTest {

void CheckStringsNotEqual(TestResults& results, char const* expected, char const* actual,
                       TestDetails const& details)
{
	using namespace std;

    if (!(strcmp(expected, actual)))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected not " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

void CheckNotEqual(TestResults& results, char const* expected, char const* actual,
                TestDetails const& details)
{
	CheckStringsNotEqual(results, expected, actual, details);
}

void CheckNotEqual(TestResults& results, char* expected, char* actual,
                TestDetails const& details)
{
	CheckStringsNotEqual(results, expected, actual, details);
}

void CheckNotEqual(TestResults& results, char* expected, char const* actual,
                TestDetails const& details)
{
	CheckStringsNotEqual(results, expected, actual, details);
}

void CheckNotEqual(TestResults& results, char const* expected, char* actual,
                TestDetails const& details)
{
	CheckStringsNotEqual(results, expected, actual, details);
}
}
