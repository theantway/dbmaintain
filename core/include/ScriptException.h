#ifndef SCRIPTEXCEPTION_H_
#define SCRIPTEXCEPTION_H_

#include "BaseException.h"

#include <string>
using namespace std;

class ScriptException : public BaseException {
public:
	ScriptException(string message) throw(): BaseException(message){};
};

#endif /* SCRIPTEXCEPTION_H_ */
