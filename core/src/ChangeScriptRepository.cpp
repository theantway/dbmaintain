#include <ChangeScriptRepository.h>
#include <iostream>


ChangeScriptRepository::ChangeScriptRepository(list< shared_ptr<ChangeScript> > scripts) {
    this->scripts = scripts;

	this->scripts.sort(ChangeScript::compare);

    checkForDuplicateIds(this->scripts);
}

ChangeScriptRepository::~ChangeScriptRepository() {
    //Auto-generated destructor stub
}


void ChangeScriptRepository::checkForDuplicateIds(list< shared_ptr<ChangeScript> > scripts) {
    int lastId = -1;

	for (list< shared_ptr<ChangeScript> >::iterator it=scripts.begin() ; it != scripts.end(); it++ ){
		shared_ptr<ChangeScript> script = *it;
	    if (script->getId() == lastId) {
			cerr << "There is more than one change script with number " << lastId <<endl;
            // throw DuplicateChangeScriptException("There is more than one change script with number " + lastId);
        }

		// cout << "  read script " << script->toString() << endl;
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
