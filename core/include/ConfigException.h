#ifndef CONFIGEXCEPTION_H_
#define CONFIGEXCEPTION_H_

#include "BaseException.h"
#include <string>
using namespace std;

class ConfigException : public BaseException{
public:
	ConfigException(string message): BaseException(message){};
	virtual ~ConfigException(){};
};

#endif /* CONFIGEXCEPTION_H_ */
