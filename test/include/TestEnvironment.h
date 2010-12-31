#ifndef _TEST_ENVIRONMENT_H_
#define _TEST_ENVIRONMENT_H_

#include <string>

using namespace std;

class TestEnvironment
{
public:
	virtual string GetFilePath(string file);
	static TestEnvironment* Instance();


	virtual ~TestEnvironment();


	virtual void SetExecDirectory(string path){
		m_path = path;
	};

	virtual string GetExecDirectory(){
		return m_path;
	};

protected:
	static TestEnvironment* m_instance;

	string m_path;
	TestEnvironment();
};

#endif // _TEST_ENVIRONMENT_H_
