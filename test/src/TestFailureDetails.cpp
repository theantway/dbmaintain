#include "TestFailureDetails.h"
#include "TestDetails.h"

namespace UnitTest {

TestFailureDetails::TestFailureDetails(const TestDetails& details, const char* failure_):
    suiteName(details.suiteName),
    testName(details.testName),
    filename(details.filename),
    failure(failure_),
    lineNumber(details.lineNumber)
{
}

TestFailureDetails::~TestFailureDetails() {
}
}
