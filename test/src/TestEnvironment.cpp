// TestEnvironment.cpp: implementation of the TestEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "TestEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TestEnvironment* TestEnvironment::m_instance = NULL;

TestEnvironment::TestEnvironment()
{

}

TestEnvironment::~TestEnvironment()
{

}

TestEnvironment* TestEnvironment::Instance(){
    static TestEnvironment m_instance;

    return &m_instance;
}

string TestEnvironment::GetFilePath(string file)
{
    return GetExecDirectory() + file;
}
