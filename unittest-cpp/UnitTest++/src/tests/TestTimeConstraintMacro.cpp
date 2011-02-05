#include "../UnitTest++.h"
#include "../TimeHelpers.h"

#include "RecordingReporter.h"
#include "ScopedCurrentTest.h"

namespace {

TEST(TimeConstraintMacroQualifiesNamespace)
{
    // If this compiles without a "using namespace UnitTest;", all is well.
    UNITTEST_TIME_CONSTRAINT(1);
}

TEST(TimeConstraintMacroUsesCorrectInfo)
{
    int testLine = 0;
    RecordingReporter reporter;

    {
      UnitTest::TestResults testResults(&reporter);
      ScopedCurrentTest scopedResults(testResults);

      UNITTEST_TIME_CONSTRAINT(10);  testLine = __LINE__;
      UnitTest::TimeHelpers::SleepMs(20);
    }

    using namespace std;

    CHECK_EQUAL(1, reporter.testFailedCount);
    CHECK(reporter.lastFailedFile.find(__FILE__) != reporter.lastFailedFile.npos);
    CHECK_EQUAL(testLine, reporter.lastFailedLine);
    CHECK(reporter.lastFailedTest.find("TimeConstraintMacroUsesCorrectInfo") != reporter.lastFailedTest.npos);
}

TEST(TimeConstraintMacroComparesAgainstPreciseActual)
{
    int testLine = 0;
    RecordingReporter reporter;

    {
        UnitTest::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        UNITTEST_TIME_CONSTRAINT(1);  testLine = __LINE__;

        // start a new timer and run until we're as little over the 1 msec
        // threshold as we can achieve; this should guarantee that the "test"
        // runs in some very small amount of time > 1 msec
        UnitTest::Timer myTimer;
        myTimer.Start();

        while (myTimer.GetTimeInMs() < 1.001)
            UnitTest::TimeHelpers::SleepMs(0);
    }

    using namespace std;

    CHECK_EQUAL(1, reporter.testFailedCount);
    CHECK(reporter.lastFailedFile.find(__FILE__) != reporter.lastFailedFile.npos);
    CHECK_EQUAL(testLine, reporter.lastFailedLine);
    CHECK(reporter.lastFailedTest.find("TimeConstraintMacroComparesAgainstPreciseActual") != reporter.lastFailedTest.npos);
}

}
