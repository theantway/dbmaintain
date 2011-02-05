#include "../UnitTest++.h"
#include "../TestResults.h"
#include "../TimeHelpers.h"
#include "RecordingReporter.h"
#include "ScopedCurrentTest.h"

using namespace UnitTest;

namespace
{

TEST(TimeConstraintSucceedsWithFastTest)
{
    TestResults result;
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(200, TestDetails("", "", "", 0));
        TimeHelpers::SleepMs(5);
    }
    CHECK_EQUAL(0, result.GetFailureCount());
}

TEST(TimeConstraintFailsWithSlowTest)
{
    TestResults result;
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(10, TestDetails("", "", "", 0));
        TimeHelpers::SleepMs(20);
    }
    CHECK_EQUAL(1, result.GetFailureCount());
}

TEST(TimeConstraintFailureIncludesCorrectData)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);

        TestDetails const details("testname", "suitename", "filename", 10);
        TimeConstraint t(10, details);
        TimeHelpers::SleepMs(20);
    }

    using namespace std;

    CHECK(reporter.lastFailedFile.find("filename") != reporter.lastFailedFile.npos);
    CHECK_EQUAL(10, reporter.lastFailedLine);
    CHECK(reporter.lastFailedTest.find("testname") != reporter.lastFailedTest.npos);
}

TEST(TimeConstraintFailureIncludesTimeoutInformation)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);
        TimeConstraint t(10, TestDetails("", "", "", 0));
        TimeHelpers::SleepMs(20);
    }

    using namespace std;

    CHECK(reporter.lastFailedMessage.find("ime constraint") != reporter.lastFailedMessage.npos);
    CHECK(reporter.lastFailedMessage.find("under 10ms") != reporter.lastFailedMessage.npos);
}

}
