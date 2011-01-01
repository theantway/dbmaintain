#ifndef DBEXCEPTION_H_
#define DBEXCEPTION_H_

#include <string>
using namespace std;

class DbException {
public:
	DbException(string message);
	virtual ~DbException();
private:
	string m_message;
};

#endif /* DBEXCEPTION_H_ */
