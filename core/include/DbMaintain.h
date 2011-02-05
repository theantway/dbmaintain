#ifndef CORE_DBMAINTAIN_H_
#define CORE_DBMAINTAIN_H_

#include <string>

#include "config/Config.h"

using namespace std;

class ScriptRunner;
class ExecutedScripts;

class DbMaintain {
public:
    DbMaintain();
    DbMaintain(string configFile);
    virtual ~DbMaintain();

    void setConfigFile(string configFile);

    void validate();
    void clear();
    void clean();
    void update();

private:
    int getLatestVersion(SqlScriptRunner& runner, ExecutedScripts& executedScriptsSettings);
    void init();
    void checkForRequiredParameter(string parameterValue, string parameterName);

    string getWelcomeString();

    Config m_config;
};

#endif /* CORE_DBMAINTAIN_H_ */
