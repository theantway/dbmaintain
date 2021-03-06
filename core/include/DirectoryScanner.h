#ifndef DIRECTORYSCANNER_H_
#define DIRECTORYSCANNER_H_

#include "list"
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScript;

class DirectoryScanner {
public:
    DirectoryScanner();
    virtual ~DirectoryScanner();

	list< shared_ptr<ChangeScript> > getChangeScriptsForDirectory(string directory);
};

#endif /* DIRECTORYSCANNER_H_ */
