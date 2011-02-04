#ifndef CORE_CONFIG_CONFIGEXCEPTION_H_
#define CORE_CONFIG_CONFIGEXCEPTION_H_

#include "BaseException.h"
#include <string>

using namespace std;

class ConfigException : public BaseException{
public:
	ConfigException(string message) throw(): BaseException(message){};
};

#endif /* CORE_CONFIG_CONFIGEXCEPTION_H_ */
