#ifndef UNITTEST_DEFERREDTESTRESULT_H
#define UNITTEST_DEFERREDTESTRESULT_H

#include "Config.h"

#include <string>
#include <vector>
using namespace std;

namespace UnitTest
{

struct DeferredTestResult
{
    DeferredTestResult();
    DeferredTestResult(const string suite, const string test);
    virtual ~DeferredTestResult();
    
    std::string suiteName;
    std::string testName;
    std::string failureFile;
    
    typedef std::pair< int, std::string > Failure;
    typedef std::vector< Failure > FailureVec;
    FailureVec failures;
    
    float timeElapsed;
	bool failed;
};

}

#endif //UNITTEST_DEFERREDTESTRESULT_H
