#ifndef CHANGESCRIPTREPOSITORY_H_
#define CHANGESCRIPTREPOSITORY_H_

#include <list>
#include "ChangeScript.h"
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScriptRepository {
public:
    ChangeScriptRepository(list< shared_ptr<ChangeScript> > scripts);
    virtual ~ChangeScriptRepository();

	void checkForDuplicateIds(list< shared_ptr<ChangeScript> >);

	list< shared_ptr<ChangeScript> > getOrderedListOfDoChangeScripts();

	list< shared_ptr<ChangeScript> > getOrderedListOfUndoChangeScripts();

	list< shared_ptr<ChangeScript> > getAvailableChangeScripts();

private:
	list< shared_ptr<ChangeScript> > scripts;
};

#endif /* CHANGESCRIPTREPOSITORY_H_ */
