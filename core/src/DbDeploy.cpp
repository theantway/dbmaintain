#include <DbDeploy.h>

#include <iostream>
#include <fstream>
#include <set>

#include <boost/foreach.hpp>

#include "ChangeScript.h"
#include "ChangeScriptRepository.h"
#include "DirectoryScanner.h"
#include "Value.h"
#include "runner/ScriptRunner.h"
#include "config/ClearOptions.h"
#include "config/Database.h"

using namespace std;

DbDeploy::DbDeploy() {
    lastChangeToApply = 999999;
    deltaset = "Main";

    init();
}

DbDeploy::~DbDeploy() {
}

void DbDeploy::init(){
    ScriptRunner::init();
}

void DbDeploy::setDriver(string driver) {
    this->driver = driver;
}

void DbDeploy::setUrl(string url) {
    this->url = url;
}

void DbDeploy::setUserid(string userid) {
    this->userid = userid;
}

void DbDeploy::setPassword(string password) {
    this->password = password;
}

void DbDeploy::setScriptDirectory(string scriptdirectory) {
    this->scriptdirectory = scriptdirectory;
}

void DbDeploy::setLastChangeToApply(int lastChangeToApply) {
    this->lastChangeToApply = lastChangeToApply;
}

void DbDeploy::setDeltaset(string deltaset) {
    this->deltaset = deltaset;
}

void DbDeploy::clear(Config& config) {
    const map<string, shared_ptr<Database> > databases = config.getDatabases();

    for (map<string, shared_ptr<Database> >::const_iterator it=databases.begin() ; it != databases.end(); it++ ){
        const shared_ptr<SqlScriptRunner> scriptRunner = config.getSqlRunner((*it).first);
        scriptRunner->clearDatabase((*it).second);
    }
}

void DbDeploy::clean(Config& config) {
    const map<string, shared_ptr<Database> > databases = config.getDatabases();

    for (map<string, shared_ptr<Database> >::const_iterator it=databases.begin() ; it != databases.end(); it++ ){
        const shared_ptr<SqlScriptRunner> scriptRunner = config.getSqlRunner((*it).first);
        scriptRunner->cleanDatabase((*it).second);
    }
}

shared_ptr<ScriptRunner> DbDeploy::initRunner(){
    shared_ptr<ScriptRunner> runner = ScriptRunner::getRunner("postgres");
//    runner->setConnectionString("dbname=dbmaintain_test user=postgres");
    return runner;
}

void DbDeploy::update(Config& config){
    shared_ptr<SqlScriptRunner> runner = config.getDefaultSqlRunner();

    cout << getWelcomeString() <<endl;
    string tableName = "script_table";
    map<string, string> fieldsMap;
    runner->ensureScriptsTableExists(tableName, fieldsMap);
    int latestNo = getLatestVersion(runner, tableName);

    ChangeScriptRepository changeScriptRepository(DirectoryScanner().getChangeScriptsForDirectory(config.getScriptsLocation()));

    list< shared_ptr<ChangeScript> > scriptsToApply = changeScriptRepository.getScriptsToApply(latestNo);

    cout << "Found " << changeScriptRepository.getAvailableChangeScripts().size() << " scripts in file system, "
        << "the latest script no in db is "<< latestNo << ". " << endl
        << "There are " << scriptsToApply.size() << " script(s) need to apply." << endl;

    for (list< shared_ptr<ChangeScript> >::iterator it=scriptsToApply.begin() ; it != scriptsToApply.end(); it++ ){
        shared_ptr<ChangeScript> script = *it;
        cout << "  applying " << script->getFilename() <<endl;
        shared_ptr<ScriptRunner> scriptRunner = config.scriptRunner(script->getFilename());
        runner->beginRunScript(tableName, fieldsMap, script);
        scriptRunner->run(script);
        runner->endRunScript(tableName, fieldsMap, script);
    }

    cout << "Successed." <<endl;
}

int DbDeploy::getLatestVersion(shared_ptr<SqlScriptRunner> runner, string tableName){
    map<string, shared_ptr<Value> > latest = runner->getLatestVersion(tableName);
    shared_ptr<Value> no = latest["script_no"];

    int latestNo = 0;
    if(no.get() != NULL){
        latestNo = no->asInt();
    }
    return latestNo;
}

void DbDeploy::validate() {
    // checkForRequiredParameter(userid, "userid");
    //     checkForRequiredParameter(driver, "driver");
    //     checkForRequiredParameter(url, "url");
    //     checkForRequiredParameter(dbms, "dbms");
    //     checkForRequiredParameter(outputfile, "outputfile");
    //     checkForRequiredParameter(scriptdirectory, "dir");
    //
    //     if (scriptdirectory == null || !scriptdirectory.isDirectory()) {
    //         throw new UsageException("Script directory must point to a valid directory");
    //     }
}

void DbDeploy::checkForRequiredParameter(string parameterValue, string parameterName) {
    // if (parameterValue == null || parameterValue.length() == 0) {
    //         UsageException.throwForMissingRequiredValue(parameterName);
    //     }
}

//PrintStream
void DbDeploy::createUndoOutputPrintStream(string undoOutputFile) {
    // if (undoOutputstring != null) {
    //         return new PrintStream(undoOutputFile);
    //     } else {
    //         return null;
    //     }
}

string DbDeploy::getUserid() {
    return userid;
}

string DbDeploy::getUrl() {
    return url;
}

string DbDeploy::getPassword() {
    return password;
}

string DbDeploy::getScriptDirectory() {
    return scriptdirectory;
}

int DbDeploy::getLastChangeToApply() {
    return lastChangeToApply;
}

string DbDeploy::getDeltaset() {
    return deltaset;
}

string DbDeploy::getDriver() {
    return driver;
}

string DbDeploy::getWelcomeString() {
    // InputStream stream = getClass().getClassLoader().getResourceAsStream("welcome.txt");
    //     try {
    //         return new BufferedReader(new InputStreamReader(stream)).readLine();
    //     } catch (IOException e) {
    //         return null;
    //     }
    return "Welcome to use DbDeploy++";
}
