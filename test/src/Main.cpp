#include "UnitTest++.h"
#include "StdoutTestReporter.h"
#include "UnitXTestReporter.h"
#include "CompositeTestReporter.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include "TestEnvironment.h"

using namespace std;

using namespace UnitTest;

string getPath(string& fullpath, string& fileName){
    string::size_type pos = fullpath.find_last_of("/");
    if(pos != string::npos) {
        fileName = fullpath.substr(pos + 1);
        fullpath.erase(pos + 1);
    }

    return fullpath;
}

int main(int argc, char const *argv[])
{
    string execPath = argv[0];
    string execName;

    char lbuf[1024] ;
    if ( getcwd( lbuf , sizeof( lbuf ) ) )
    {
        // cout << "===============" <<endl;
        //        cout << "current dir=" <<lbuf<<endl;
        //        cout << "exec=" << argv[0]<<endl;
    }

    string path;
    if(execPath[0] == '/'){
        path = execPath;
    }else{
        path += lbuf;
        path += "/";
        path += execPath;
    }

    getPath(path, execName);

    TestEnvironment::Instance()->SetExecDirectory(path);

	string outputFileName = path + execName + ".xml";

	cout<< "================================" <<endl
        << "Running " << execName <<endl
        << "Saving result file to " << outputFileName <<endl
        << "================================" <<endl;

	std::ofstream f(outputFileName.c_str());
	UnitXTestReporter unitXReporter(outputFileName, f);
	StdoutTestReporter stdoutReporter;

	CompositeTestReporter reporters;
	reporters.AddReporter(&unitXReporter);
	reporters.AddReporter(&stdoutReporter);

	TestRunner runner(reporters);
	return runner.RunTestsIf(Test::GetTestList(), NULL, True(), 0);
}
