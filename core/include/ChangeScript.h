#ifndef CHANGESCRIPT_H_
#define CHANGESCRIPT_H_

#include <string>
#include <sys/stat.h>
#include <boost/shared_ptr.hpp>

using namespace boost;
using namespace std;

class ChangeScript {
public:
	ChangeScript(int id, string filename, string directory);
	
	virtual ~ChangeScript();

	string getFilename();
	string getFullPath();

	int getId();

	string getDescription();

	static int compare(shared_ptr<ChangeScript> first, shared_ptr<ChangeScript> second);

	string toString();

	string getContent();

	time_t getLastModifiedAt();

	string getCheckSum();

private:
	int id;
	string filename;
	string directory;
	string description;
	long fileLastModifiedAt;
	string checksum;
	
	string getFileContents();
};
#endif /* CHANGESCRIPT_H_ */
