#ifndef DBDEPLOY_H_
#define DBDEPLOY_H_

#include <string>

#include "config/Config.h";

using namespace std;

class ScriptRunner;

class DbDeploy {
public:
    DbDeploy(const Config& config);
    virtual ~DbDeploy();

    void init();

    void setDriver(string driver);

    void setUrl(string url);

    void setUserid(string userid);

    void setPassword(string password);

    void setScriptDirectory(string scriptdirectory);

    void setOutputfile(string outputfile);

    void setDbms(string dbms);

    void setLastChangeToApply(int lastChangeToApply);

    void setDeltaset(string deltaset);

    void setUndoOutputfile(string undoOutputfile);

    void clear(const Config& config);

    void clean(const Config& config);

    shared_ptr<ScriptRunner> initRunner();

    void update(const Config& config);

    int getLatestVersion(shared_ptr<SqlScriptRunner> runner, string tableName);

    void validate();

    void checkForRequiredParameter(string parameterValue, string parameterName);

    //PrintStream
    void createUndoOutputPrintStream(string undoOutputFile);

    string getUserid();

    string getUrl();

    string getPassword();

    string getScriptDirectory();

    string getOutputfile();

    string getUndoOutputfile();

    string getDbms();

    int getLastChangeToApply();

    string getDeltaset();

    string getDriver();

    string getWelcomeString();

private:
    string url;
    string userid;
    string password;
    string scriptdirectory;
    string outputfile;
    string undoOutputfile;
    string dbms;
    int lastChangeToApply;
    string deltaset;
    string driver;
//    Config m_config;

};

#endif /* DBDEPLOY_H_ */
