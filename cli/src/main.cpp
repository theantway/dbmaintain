#include "DbDeploy.h"

int main(int argc, char** argv){
    DbDeploy deploy;
    deploy.setScriptDirectory("/Users/weixu/projects/msx/mobilestorm/schema/src/main/database/scripts/01_v1.0");
    deploy.go();
    return 0;
}