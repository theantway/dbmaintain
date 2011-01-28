#ifndef CORE_DBMAINTAIN_H_
#define CORE_DBMAINTAIN_H_

#include <string>

#include "config/Config.h";

using namespace std;

class ScriptRunner;

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
    int getLatestVersion(shared_ptr<SqlScriptRunner> runner, string tableName);
    void init();
    void checkForRequiredParameter(string parameterValue, string parameterName);

    string getWelcomeString();
    shared_ptr<ScriptRunner> initRunner();

    Config m_config;
};

#endif /* CORE_DBMAINTAIN_H_ */
