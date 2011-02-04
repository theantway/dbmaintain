#ifndef DBEXCEPTION_H_
#define DBEXCEPTION_H_

#include "BaseException.h"
#include <string>
using namespace std;

class DbException : public BaseException {
public:
	DbException(string message) throw(): BaseException(message){};
};

#endif /* DBEXCEPTION_H_ */
