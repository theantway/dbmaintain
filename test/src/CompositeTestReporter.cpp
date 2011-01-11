#include "CompositeTestReporter.h"
#include "Config.h"

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::ostringstream;
using std::ostream;

namespace UnitTest {
CompositeTestReporter::CompositeTestReporter()
{
}

CompositeTestReporter::~CompositeTestReporter()
{
}

void CompositeTestReporter::AddReporter(TestReporter* reporter){
    m_reporters.push_back(reporter);
}

void CompositeTestReporter::ReportSummary(int totalTestCount, int failedTestCount,
                                    int failureCount, float secondsElapsed)
{
    for (list<TestReporter*>::iterator ita =m_reporters.begin(); ita!=m_reporters.end(); ++ita)
    {
        (*ita)->ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);
    }
}

void CompositeTestReporter::ReportTestStart(TestDetails const& details){
    for (list<TestReporter*>::iterator ita =m_reporters.begin(); ita!=m_reporters.end(); ++ita)
    {
        (*ita)->ReportTestStart(details);
    }
}

void CompositeTestReporter::ReportFailure(TestDetails const& details, char const* failure){
    for (list<TestReporter*>::iterator ita =m_reporters.begin(); ita!=m_reporters.end(); ++ita)
    {
        (*ita)->ReportFailure(details, failure);
    }
}

void CompositeTestReporter::ReportTestFinish(TestDetails const& details, float secondsElapsed){
    for (list<TestReporter*>::iterator ita =m_reporters.begin(); ita!=m_reporters.end(); ++ita)
    {
        (*ita)->ReportTestFinish(details, secondsElapsed);
    }
}

}
