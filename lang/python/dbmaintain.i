%module dbmaintain

%include "std_string.i"

%{
#include "DbMaintain.h"
%}

class DbMaintain {
public:
    DbMaintain();
    DbMaintain(std::string configFile);
    virtual ~DbMaintain();

    void setConfigFile(std::string configFile);

    void validate();
    void clear();
    void clean();
    void update();
};
