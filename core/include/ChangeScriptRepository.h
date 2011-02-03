#ifndef CHANGESCRIPTREPOSITORY_H_
#define CHANGESCRIPTREPOSITORY_H_

#include <list>

#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScript;

class ChangeScriptRepository {
public:
    ChangeScriptRepository(list< shared_ptr<ChangeScript> > scripts);
    virtual ~ChangeScriptRepository();

	void validateScripts();

	list< shared_ptr<ChangeScript> > getScriptsToApply(int latestNo);

	list< shared_ptr<ChangeScript> > getAvailableChangeScripts();

private:
	list< shared_ptr<ChangeScript> > scripts;
};

#endif /* CHANGESCRIPTREPOSITORY_H_ */
