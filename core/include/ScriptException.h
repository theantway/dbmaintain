#ifndef SCRIPTEXCEPTION_H_
#define SCRIPTEXCEPTION_H_

#include "BaseException.h"

#include <string>
using namespace std;

class ScriptException : public BaseException {
public:
	ScriptException(string message) : BaseException(message){};
	virtual ~ScriptException(){};
};

#endif /* SCRIPTEXCEPTION_H_ */
