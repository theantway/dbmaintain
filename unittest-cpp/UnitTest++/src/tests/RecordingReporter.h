#ifndef UNITTEST_RECORDINGREPORTER_H
#define UNITTEST_RECORDINGREPORTER_H

#include "../TestReporter.h"
#include <cstring>

#include "../TestDetails.h"

struct RecordingReporter : public UnitTest::TestReporter
{
private:
    enum { kMaxStringLength = 256 };

public:
    RecordingReporter()
        : testRunCount(0)
        , lastStartedSuite("")
        , lastStartedTest("")
        , testFailedCount(0)
        , lastFailedFile("")
        , lastFailedLine(0)
        , lastFailedSuite("")
        , lastFailedTest("")
        , lastFailedMessage("")
        , testFinishedCount(0)
        , lastFinishedSuite("")
        , lastFinishedTest("")
        , lastFinishedTestTime(0)
        , summaryTotalTestCount(0)
        , summaryFailedTestCount(0)
        , summaryFailureCount(0)
        , summarySecondsElapsed(0)

    {
    }

    virtual void ReportTestStart(UnitTest::TestDetails const& test)
    {
		using namespace std;

        ++testRunCount;
        lastStartedSuite = test.suiteName;
        lastStartedTest = test.testName;
    }

    virtual void ReportFailure(UnitTest::TestDetails const& test, char const* failure)
    {
		using namespace std;

		++testFailedCount;
        lastFailedFile = test.filename;
        lastFailedLine = test.lineNumber;
        lastFailedSuite = test.suiteName;
        lastFailedTest = test.testName;
        lastFailedMessage = failure;
    }

    virtual void ReportTestFinish(UnitTest::TestDetails const& test, float testDuration)
    {
		using namespace std;

		++testFinishedCount;
        lastFinishedSuite = test.suiteName;
        lastFinishedTest = test.testName;
        lastFinishedTestTime = testDuration;
    }

    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) 
    {
        summaryTotalTestCount = totalTestCount;
        summaryFailedTestCount = failedTestCount;
        summaryFailureCount = failureCount;
        summarySecondsElapsed = secondsElapsed;
    }

    int testRunCount;
    string lastStartedSuite;
    string lastStartedTest;

    int testFailedCount;
    string lastFailedFile;
    int lastFailedLine;
    string lastFailedSuite;
    string lastFailedTest;
    string lastFailedMessage;

    int testFinishedCount;
    string lastFinishedSuite;
    string lastFinishedTest;
    float lastFinishedTestTime;

    int summaryTotalTestCount;
    int summaryFailedTestCount;
    int summaryFailureCount;
    float summarySecondsElapsed;
};


#endif
