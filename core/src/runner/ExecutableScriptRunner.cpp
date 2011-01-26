#include "runner/ExecutableScriptRunner.h"

#include <sstream>

#include "ChangeScript.h"
#include "config/ConfigException.h"

using namespace std;

ExecutableScriptRunner::ExecutableScriptRunner(const string& executableName){
    m_executable = executableName;
}

ExecutableScriptRunner::~ExecutableScriptRunner(){

}

void ExecutableScriptRunner::run(shared_ptr<ChangeScript> script){
    ostringstream oss;
    oss << m_executable << " \"" << script->getFullPath() << "\"";

//    cout << "running " << oss.str()<<endl;
    if(-1 == system(oss.str().c_str())){
        ostringstream err;
        err<< "Could not execute " << oss.str();
        throw ConfigException(err.str());
    }
}
