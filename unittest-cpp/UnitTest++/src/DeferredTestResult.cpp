#include "DeferredTestResult.h"
#include "Config.h"

namespace UnitTest
{

DeferredTestResult::DeferredTestResult()
    : suiteName("")
    , testName("")
    , failureFile("")
    , timeElapsed(0.0f)
    , failed(false)
{
}

DeferredTestResult::DeferredTestResult(const string suite, const string test)
    : suiteName(suite)
    , testName(test)
    , failureFile("")
    , timeElapsed(0.0f)
    , failed(false)
{
}

DeferredTestResult::~DeferredTestResult()
{
}

}
