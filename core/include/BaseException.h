#ifndef BASEEXCEPTION_H_
#define BASEEXCEPTION_H_

#include <string>
using namespace std;

class BaseException {
public:
	BaseException(string message);
	virtual ~BaseException();

	string getMessage();
private:
	string m_message;
};

#endif /* BASEEXCEPTION_H_ */
