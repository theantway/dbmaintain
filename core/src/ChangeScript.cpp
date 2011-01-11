#include "ChangeScript.h"
#include "StringUtil.h"

#include <openssl/md5.h>

#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
ChangeScript::ChangeScript(int id, string filename, string directory):
    id(id), filename(filename), directory(directory)
{
}

ChangeScript::~ChangeScript() {
}

string ChangeScript::getFilename() {
    return filename;
}

int ChangeScript::getId() {
    return id;
}

string ChangeScript::getDescription() {
    return description;
}

int ChangeScript::compare(shared_ptr<ChangeScript> first, shared_ptr<ChangeScript> second) {
    return first->id < second->id;
}

string ChangeScript::toString() {
    ostringstream oss;
    oss << "#" << id  << ": " << description;
    return oss.str();
}

string ChangeScript::getContent() {
    return getFileContents();
}

string ChangeScript::getFullPath(){
    string dir(directory);
    dir = StringUtil::trimRight(dir, "/\\");
    return dir + '/' + filename;
}

string ChangeScript::getFileContents() {
    ifstream is;
    ostringstream oss;

    is.open(getFullPath().c_str(), ios::binary);

    if(!is.good()){
        return oss.str();
    }

    oss << is.rdbuf();
    is.close();

    return oss.str();
}

time_t ChangeScript::getLastModifiedAt(){
    struct stat pathStat;
    stat(getFullPath().c_str(), &pathStat);

    return pathStat.st_mtime;
}

string ChangeScript::getCheckSum(){
    unsigned char result[MD5_DIGEST_LENGTH];
    string content = getFileContents();
    MD5((unsigned char*) content.c_str(), content.length(), result);

    ostringstream oss;
    for(int i=0; i < MD5_DIGEST_LENGTH; i++) {
        oss << setw( 2 ) << setfill( '0' ) << hex << int(result[i]);
    }

    return oss.str();
}
