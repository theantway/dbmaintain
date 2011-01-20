#ifndef CORE_CONFIG_CONFIGEXCEPTION_H_
#define CORE_CONFIG_CONFIGEXCEPTION_H_

#include "BaseException.h"
#include <string>

using namespace std;

class ConfigException : public BaseException{
public:
	ConfigException(string message): BaseException(message){};
	virtual ~ConfigException(){};
};

#endif /* CORE_CONFIG_CONFIGEXCEPTION_H_ */
