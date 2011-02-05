#include "TestDetails.h"

namespace UnitTest {

TestDetails::TestDetails(const string testName_, const string suiteName_, const string filename_, int lineNumber_)
    : suiteName(suiteName_)
    , testName(testName_)
    , filename(filename_)
    , lineNumber(lineNumber_)
{
}

TestDetails::TestDetails(const TestDetails& details, int lineNumber_)
    : suiteName(details.suiteName)
    , testName(details.testName)
    , filename(details.filename)
    , lineNumber(lineNumber_)
{
}

TestDetails::TestDetails(TestDetails const& details)
    : suiteName(details.suiteName)
    , testName(details.testName)
    , filename(details.filename)
    , lineNumber(details.lineNumber)
{
}

TestDetails& TestDetails::operator=(TestDetails const& details){
    suiteName = details.suiteName;
    testName = details.testName;
    filename = details.filename;
    lineNumber = details.lineNumber;

    return *this;
}

}
