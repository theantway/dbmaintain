#include <DirectoryScanner.h>

#include <iostream>
#include <dirent.h>

DirectoryScanner::DirectoryScanner() {
}

DirectoryScanner::~DirectoryScanner() {
    //Auto-generated destructor stub
}

list< shared_ptr<ChangeScript> > DirectoryScanner::getChangeScriptsForDirectory(string directory)  {
    cout << "Reading change scripts from directory " << directory << "..."<<endl;

    list< shared_ptr<ChangeScript> > scripts;

	DIR *dp;
    struct dirent *ep;

    dp = opendir (directory.c_str());
    if (dp == NULL){
		cerr << "Couldn't open the directory" << endl;
		return scripts;
	}

    while (ep = readdir (dp)){
		if (DT_DIR == ep->d_type){
			continue;
		}
        int id = atoi(ep->d_name);

        shared_ptr<ChangeScript> script(new ChangeScript(id, ep->d_name, directory));
        scripts.push_back(script);
	}

    closedir(dp);

    return scripts;
}
