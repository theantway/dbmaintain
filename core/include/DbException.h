#ifndef DBEXCEPTION_H_
#define DBEXCEPTION_H_

#include "BaseException.h"
#include <string>
using namespace std;

class DbException : public BaseException {
public:
	DbException(string message): BaseException(message){};
	virtual ~DbException(){};
};

#endif /* DBEXCEPTION_H_ */
