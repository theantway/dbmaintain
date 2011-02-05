#include <string>
#include "BaseTest.h"
#include "config/ExecutedScripts.h"

using namespace std;

SUITE(ExecutedScriptsTest){
    class ExecutedScriptsTest {
    protected:

    public:
        ExecutedScriptsTest() {
        }
        virtual ~ExecutedScriptsTest() {
        }
    };

    TEST_FIXTURE (ExecutedScriptsTest, should_set_property_value_and_ignore_empty_value)
    {
        ExecutedScripts settings;
        settings.setDatabaseName("new-db-name");
        ASSERT_EQUAL(settings.getDatabaseName(), "new-db-name");

        settings.setDatabaseName("");
        ASSERT_EQUAL(settings.getDatabaseName(), "new-db-name");
    }
}
