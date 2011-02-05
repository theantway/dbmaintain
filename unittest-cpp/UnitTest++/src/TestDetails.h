#ifndef UNITTEST_TESTDETAILS_H
#define UNITTEST_TESTDETAILS_H

#include <string>
using namespace std;

namespace UnitTest {
class TestDetails
{
public:
    TestDetails(const string testName, const string suiteName, const string filename, int lineNumber);
    TestDetails(const TestDetails& details, int lineNumber);

    string suiteName;
    string testName;
    string filename;
    int lineNumber;

    TestDetails(TestDetails const&); // Why is it public? --> http://gcc.gnu.org/bugs.html#cxx_rvalbind
    TestDetails& operator=(TestDetails const&);
private:
};

}

#endif
