#include "ChangeScriptRepository.h"

#include <iostream>
#include <sstream>

#include "ScriptException.h"
#include "ChangeScript.h"

ChangeScriptRepository::ChangeScriptRepository(list< shared_ptr<ChangeScript> > scripts) throw (ScriptException){
    this->scripts = scripts;

    this->scripts.sort(ChangeScript::compare);

    validateScripts();
}

ChangeScriptRepository::~ChangeScriptRepository() {
}

void ChangeScriptRepository::validateScripts() throw (ScriptException){
    int lastId = 0;

    for (list< shared_ptr<ChangeScript> >::iterator it=scripts.begin() ; it != scripts.end(); it++ ){
        shared_ptr<ChangeScript> script = *it;
        if (script->getId() == lastId) {
            ostringstream oss;
            oss << "There is more than one change script with number " << lastId;
            throw ScriptException(oss.str());
        }

        if (script->getId() != lastId + 1) {
            ostringstream oss;
            oss << "Script with number " << (lastId + 1) << " not found, but found " << script->getFilename();
            throw ScriptException(oss.str());
        }

        lastId = script->getId();
    }
}

list< shared_ptr<ChangeScript> > ChangeScriptRepository::getScriptsToApply(int latestNo){
    list< shared_ptr<ChangeScript> > result;
    for (list< shared_ptr<ChangeScript> >::iterator it=scripts.begin() ; it != scripts.end(); it++ ){
        shared_ptr<ChangeScript> script = *it;
        if (script->getId() > latestNo) {
            result.push_back(script);
        }
    }

    return result;
}

list< shared_ptr<ChangeScript> > ChangeScriptRepository::getAvailableChangeScripts() {
    return scripts;
}
