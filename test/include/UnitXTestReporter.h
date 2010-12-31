#ifndef UNITTEST_XMLTESTREPORTER_H
#define UNITTEST_XMLTESTREPORTER_H

#include "DeferredTestReporter.h"

#include <iosfwd>
#include <string>

using namespace std;

namespace UnitTest
{
class UnitXTestReporter : public DeferredTestReporter
{
public:
    explicit UnitXTestReporter(string testsuiteName, std::ostream& ostream);

    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);

private:
    UnitXTestReporter(UnitXTestReporter const&);
    UnitXTestReporter& operator=(UnitXTestReporter const&);

    void AddXmlElement(std::ostream& os, char const* encoding);
    void BeginResults(std::ostream& os, int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
    void EndResults(std::ostream& os);
    void BeginTest(std::ostream& os, DeferredTestResult const& result);
    void AddFailure(std::ostream& os, DeferredTestResult const& result);
    void EndTest(std::ostream& os, DeferredTestResult const& result);

    std::ostream& m_ostream;
    string m_testSuiteName;
};

}

#endif
