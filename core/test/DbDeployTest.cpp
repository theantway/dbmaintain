#include <string>
#include "BaseTest.h"

#include "DbMaintain.h"

using namespace std;
SUITE(DbDeployTest){
    class DbDeployTest {
    protected:

    public:
        DbDeployTest() {
        }

        virtual ~DbDeployTest() {
        }
    };

    TEST_FIXTURE (DbDeployTest, ShouldScanScriptFolder)
    {
        Config config;
        DbMaintain deploy();
//        deploy.setScriptDirectory("/Users/weixu/projects/msx/mobilestorm/schema/src/main/database/scripts/01_v1.0");
//        deploy.go();
    }
}
