#include "StdoutTestReporter.h"

#include "Config.h"

#include <iostream>
#include <sstream>
#include <string>

#include "TestDetails.h"

using std::string;
using std::ostringstream;
using std::ostream;

namespace UnitTest {

StdoutTestReporter::StdoutTestReporter()
{
}

void StdoutTestReporter::ReportTestStart(TestDetails const& test){
    cout << test.suiteName << "::" << test.testName << "...";
}

void StdoutTestReporter::ReportFailure(TestDetails const& test, char const* failure){
//    TestDetails failedTest(test);
//    m_failedTests.push_back(failedTest);
    cout << endl << "  Failed: " << failure <<endl;
}

void StdoutTestReporter::ReportTestFinish(TestDetails const& test, float secondsElapsed){
//    TestDetails& lastFailed = m_failedTests.back();
//    if(lastFailed.suiteName == test.suiteName &&
//            lastFailed.testName == test.testName &&
//            lastFailed.filename == test.filename &&
//            lastFailed.lineNumber == test.lineNumber
//            ){
//        cout << "Failed"<<endl;
//    }

    cout << "OK" <<endl;
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
        cout << "FAILURE: "<< failedTestCount << " out of " << totalTestCount << " tests failed ("<< failureCount << " failures)." <<endl;
    }else{
        cout << "Success: " << totalTestCount << " tests passed." <<endl;
    }

    cout << "Test time: " << secondsElapsed << " seconds." <<endl;
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
