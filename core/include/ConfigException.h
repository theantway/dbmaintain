#ifndef CONFIGEXCEPTION_H_
#define CONFIGEXCEPTION_H_

#include <string>
using namespace std;

class ConfigException {
public:
	ConfigException(string message);
	virtual ~ConfigException();
private:
	string m_message;
};

#endif /* CONFIGEXCEPTION_H_ */
