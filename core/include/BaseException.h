#ifndef BASEEXCEPTION_H_
#define BASEEXCEPTION_H_

#include <string>
#include <exception>
using namespace std;

class BaseException : public std::exception {
public:
	BaseException(string message) throw();
	virtual ~BaseException() throw();

	virtual const char* what() const throw();
private:
	string m_message;
};

#endif /* BASEEXCEPTION_H_ */
