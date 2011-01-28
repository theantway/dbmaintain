#ifndef CORE_DBMAINTAIN_H_
#define CORE_DBMAINTAIN_H_

#include <string>

#include "config/Config.h";

using namespace std;

class ScriptRunner;

class DbMaintain {
public:
    DbMaintain(string configFile);
    DbMaintain();
    virtual ~DbMaintain();

    void setConfigFile(string configFile);
    void init();

    void clear();

    void clean();

    shared_ptr<ScriptRunner> initRunner();

    void update();

    int getLatestVersion(shared_ptr<SqlScriptRunner> runner, string tableName);

    void validate();

    void checkForRequiredParameter(string parameterValue, string parameterName);

    string getWelcomeString();

private:
    Config m_config;
};

#endif /* CORE_DBMAINTAIN_H_ */
