#include <DbDeploy.h>

#include <iostream>
#include <fstream>

#include "ChangeScriptRepository.h"
#include "DirectoryScanner.h"

using namespace std;

DbDeploy::DbDeploy() {
    lastChangeToApply = 999999;
    deltaset = "Main";

}

DbDeploy::~DbDeploy() {
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

void DbDeploy::go(){
    cout << getWelcomeString() <<endl;

//
//    validate();

    ChangeScriptRepository changeScriptRepository(DirectoryScanner().getChangeScriptsForDirectory(scriptdirectory));

	
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
