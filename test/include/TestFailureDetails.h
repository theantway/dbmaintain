#ifndef TESTFAILUREDETAILS_H_
#define TESTFAILUREDETAILS_H_

#include <string>
using namespace std;

namespace UnitTest
{
class TestDetails;

class TestFailureDetails {
public:
    TestFailureDetails(const TestDetails& details, const char* failure);
    virtual ~TestFailureDetails();

    string suiteName;
    string testName;
    string filename;
    string failure;
    int lineNumber;
};
}
#endif /* TESTFAILUREDETAILS_H_ */
