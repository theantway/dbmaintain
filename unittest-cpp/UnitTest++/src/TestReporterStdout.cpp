#include "TestReporterStdout.h"
#include <iostream>

#include "TestDetails.h"

// cstdio doesn't pull in namespace std on VC6, so we do it here.
#if defined(_MSC_VER) && (_MSC_VER == 1200)
    namespace std {}
#endif

namespace UnitTest {

void TestReporterStdout::ReportFailure(TestDetails const& details, char const* failure)
{
//#if defined(__APPLE__) || defined(__GNUG__)
//    char const* const errorFormat = "%s:%d: error: Failure in %s: %s\n";
//#else
//    char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";
//#endif

    using namespace std;
//    printf(errorFormat, details.filename, details.lineNumber, details.testName, failure);
    cout << details.filename << "(" << details.lineNumber << "): error: Failure in " << details.testName
            << ": " << failure<<endl;
}

void TestReporterStdout::ReportTestStart(TestDetails const& /*test*/)
{
}

void TestReporterStdout::ReportTestFinish(TestDetails const& /*test*/, float)
{
}

void TestReporterStdout::ReportSummary(int const totalTestCount, int const failedTestCount,
                                       int const failureCount, float secondsElapsed)
{
    using namespace std;

    if (failureCount > 0)
        cout << "FAILURE: " << failedTestCount << " out of " << totalTestCount << " tests failed(" << failureCount << " failures)." <<endl;
    else
        cout << "Success: " << totalTestCount << " tests passed." <<endl;

    cout << "Test time: " << secondsElapsed << " seconds." <<endl;
}

}
