#ifndef UNITTEST_COMPOSITETESTREPORTER_H
#define UNITTEST_COMPOSITETESTREPORTER_H

#include "DeferredTestReporter.h"
#include "TestReporter.h"

#include <iosfwd>
#include <string>
#include <list>

using namespace std;

namespace UnitTest
{
class CompositeTestReporter : public DeferredTestReporter
{
public:
    CompositeTestReporter();
    virtual ~CompositeTestReporter(){};

    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
    virtual void ReportTestStart(TestDetails const& details);
    virtual void ReportFailure(TestDetails const& details, char const* failure);
    virtual void ReportTestFinish(TestDetails const& details, float secondsElapsed);

    void AddReporter(TestReporter* reporter);
private:
    list<TestReporter*> m_reporters;
};

}

#endif
