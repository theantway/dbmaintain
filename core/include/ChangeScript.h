#ifndef CHANGESCRIPT_H_
#define CHANGESCRIPT_H_

#include <string>
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScript {
public:
	ChangeScript(int id, string filename="test", string description="test");

	string getFilename();

	int getId();

	string getDescription();

	static int compare(shared_ptr<ChangeScript> first, shared_ptr<ChangeScript> second);

	string toString();

	string getContent();

	string getUndoContent();

private:
	int id;
	string filename;
	string description;
	static const string UNDO_MARKER;// = "--//@UNDO";

	string getFileContents(bool onlyAfterUndoMarker);
};
#endif /* CHANGESCRIPT_H_ */
