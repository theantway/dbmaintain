#include "StdoutTestReporter.h"

#include "Config.h"

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::ostringstream;
using std::ostream;

namespace UnitTest {

StdoutTestReporter::StdoutTestReporter()
{
}

void StdoutTestReporter::ReportSummary(int totalTestCount, int failedTestCount,
                                    int failureCount, float secondsElapsed)
{

    using namespace std;
    DeferredTestResultList const& results = GetResults();
    for (DeferredTestResultList::const_iterator i = results.begin(); i != results.end(); ++i)
    {
        if (i->failed){
            AddFailure(cout, *i);
        }
    }

    if (failureCount > 0){
        cout << " " << endl;
        printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
    }else{
        printf("Success: %d tests passed.\n", totalTestCount);
    }

    printf("Test time: %.2f seconds.\n", secondsElapsed);
}

void StdoutTestReporter::BeginTest(std::ostream& os, DeferredTestResult const& result)
{
    os << "testcase " << result.suiteName << "::" << result.testName
        << " time=" << result.timeElapsed <<endl;
}

void StdoutTestReporter::EndTest(std::ostream& os, DeferredTestResult const& result)
{
    os << endl;
}

void StdoutTestReporter::AddFailure(std::ostream& os, DeferredTestResult const& result)
{
    BeginTest(os, result);

    for (DeferredTestResult::FailureVec::const_iterator it = result.failures.begin();
         it != result.failures.end();
         ++it)
    {
        os << "  " << result.failureFile << "(" << it->first << ") : " << it->second <<endl;
    }

    EndTest(os, result);
}

}
