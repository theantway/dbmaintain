#ifndef STDOUTTESTREPORTER_H_
#define STDOUTTESTREPORTER_H_

#include "DeferredTestReporter.h"

#include <iosfwd>
#include <string>

using namespace std;

namespace UnitTest
{
class StdoutTestReporter : public DeferredTestReporter {
public:
    explicit StdoutTestReporter();

    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);

private:
    StdoutTestReporter(StdoutTestReporter const&);
    StdoutTestReporter& operator=(StdoutTestReporter const&);

    void BeginTest(std::ostream& os, DeferredTestResult const& result);
    void AddFailure(std::ostream& os, DeferredTestResult const& result);
    void EndTest(std::ostream& os, DeferredTestResult const& result);
};

}

#endif /* STDOUTTESTREPORTER_H_ */
