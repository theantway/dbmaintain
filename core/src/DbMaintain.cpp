#include <DbMaintain.h>

#include <iostream>
#include <fstream>
#include <set>

#include "ChangeScript.h"
#include "ChangeScriptRepository.h"
#include "DirectoryScanner.h"
#include "Value.h"
#include "runner/ScriptRunner.h"
#include "config/ClearOptions.h"
#include "config/FileConfig.h"
#include "config/Database.h"

using namespace std;

DbMaintain::DbMaintain(string configFile) {
    init();

    setConfigFile(configFile);
}

DbMaintain::DbMaintain() {
    init();
}

DbMaintain::~DbMaintain() {
}

void DbMaintain::setConfigFile(string configFile){
    m_config.applyFromFile(configFile);
}
void DbMaintain::init(){
    ScriptRunner::init();
}

void DbMaintain::clear() {
    const map<string, shared_ptr<Database> > databases = m_config.getDatabases();

    for (map<string, shared_ptr<Database> >::const_iterator it=databases.begin() ; it != databases.end(); it++ ){
        const shared_ptr<SqlScriptRunner> scriptRunner = m_config.getSqlRunner((*it).first);
        scriptRunner->clearDatabase((*it).second);
    }
}

void DbMaintain::clean() {
    const map<string, shared_ptr<Database> > databases = m_config.getDatabases();

    for (map<string, shared_ptr<Database> >::const_iterator it=databases.begin() ; it != databases.end(); it++ ){
        const shared_ptr<SqlScriptRunner> scriptRunner = m_config.getSqlRunner((*it).first);
        scriptRunner->cleanDatabase((*it).second);
    }
}

void DbMaintain::update(){
    shared_ptr<SqlScriptRunner> runner = m_config.getDefaultSqlRunner();

//    cout << getWelcomeString() <<endl;
    ExecutedScripts& executedScriptsSettings = m_config.getExecutedScriptsSettings();
    runner->ensureScriptsTableExists(executedScriptsSettings);
    int latestNo = getLatestVersion(*runner.get(), executedScriptsSettings);

    ChangeScriptRepository changeScriptRepository(DirectoryScanner().getChangeScriptsForDirectory(m_config.getScriptsLocation()));

    list< shared_ptr<ChangeScript> > scriptsToApply = changeScriptRepository.getScriptsToApply(latestNo);

    cout << "Found " << changeScriptRepository.getAvailableChangeScripts().size() << " scripts in file system, "
        << "the latest script no in db is "<< latestNo << ". " << endl
        << "There are " << scriptsToApply.size() << " script(s) need to apply." << endl;

    for (list< shared_ptr<ChangeScript> >::iterator it=scriptsToApply.begin() ; it != scriptsToApply.end(); it++ ){
        shared_ptr<ChangeScript> script = *it;
        cout << "  applying " << script->getFilename() <<endl;
        shared_ptr<ScriptRunner> scriptRunner = m_config.scriptRunner(script->getFilename());
        runner->beginRunScript(executedScriptsSettings, script);
        scriptRunner->run(script);
        runner->endRunScript(executedScriptsSettings, script);
    }

    cout << "Successed." <<endl;
}

int DbMaintain::getLatestVersion(SqlScriptRunner& runner, ExecutedScripts& executedScriptsSettings){
    map<string, string> latest = runner.getLatestVersion(executedScriptsSettings);

    if(latest.count("script_no")){
        return atoi(latest["script_no"].c_str());
    }

    return 0;
}

void DbMaintain::validate() {
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

void DbMaintain::checkForRequiredParameter(string parameterValue, string parameterName) {
    // if (parameterValue == null || parameterValue.length() == 0) {
    //         UsageException.throwForMissingRequiredValue(parameterName);
    //     }
}

string DbMaintain::getWelcomeString() {
    // InputStream stream = getClass().getClassLoader().getResourceAsStream("welcome.txt");
    //     try {
    //         return new BufferedReader(new InputStreamReader(stream)).readLine();
    //     } catch (IOException e) {
    //         return null;
    //     }
    return "Welcome to use DbDeploy++";
}
