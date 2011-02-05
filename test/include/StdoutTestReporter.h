#ifndef STDOUTTESTREPORTER_H_
#define STDOUTTESTREPORTER_H_

#include "DeferredTestReporter.h"

#include <iosfwd>
#include <string>
#include <vector>

#include "TestFailureDetails.h"

using namespace std;

namespace UnitTest
{
class StdoutTestReporter : public DeferredTestReporter {
public:
    explicit StdoutTestReporter();

    virtual void ReportTestStart(TestDetails const& test);
    virtual void ReportFailure(TestDetails const& test, char const* failure);
    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed);
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);

private:
    StdoutTestReporter(StdoutTestReporter const&);
    StdoutTestReporter& operator=(StdoutTestReporter const&);

    void BeginTest(std::ostream& os, DeferredTestResult const& result);
    void AddFailure(std::ostream& os, DeferredTestResult const& result);
    void EndTest(std::ostream& os, DeferredTestResult const& result);

    vector<TestFailureDetails> m_failedTests;
};

}

#endif /* STDOUTTESTREPORTER_H_ */
