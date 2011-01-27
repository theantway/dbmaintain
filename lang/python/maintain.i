%module dbmaintain

%{
#include "DbDeploy.h"
%}

class DbDeploy {
public:
    DbDeploy();
    virtual ~DbDeploy();

    void init();

    void clear(Config& config);

    void clean(Config& config);

    shared_ptr<ScriptRunner> initRunner();

    void update(Config& config);

    int getLatestVersion(shared_ptr<SqlScriptRunner> runner, string tableName);

    void validate();

    void checkForRequiredParameter(string parameterValue, string parameterName);

    string getWelcomeString();
};