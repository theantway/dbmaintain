#ifndef CORE_RUNNER_EXECUTABLESCRIPTRUNNER_H_
#define CORE_RUNNER_EXECUTABLESCRIPTRUNNER_H_

#include <string>

#include "runner/ScriptRunner.h"

using namespace std;

class ExecutableScriptRunner : public ScriptRunner {
public:
    ExecutableScriptRunner(const string& executableName="");
    virtual ~ExecutableScriptRunner();

    void run(shared_ptr<ChangeScript> script);
};
#endif /* CORE_RUNNER_EXECUTABLESCRIPTRUNNER_H_ */
