#include <string>
#include <list>
#include <map>

#include <iostream>

#include "BaseTest.h"
#include "runner/ScriptRunner.h"
#include "ChangeScriptRepository.h"
#include "ChangeScript.h"
#include "Value.h"
#include "ScriptException.h"

using namespace std;

SUITE(ChangeScriptRepositoryTest){
    class ChangeScriptRepositoryTest {
    protected:
    public:
        ChangeScriptRepositoryTest() {
        }
        virtual ~ChangeScriptRepositoryTest() {
        }
    };

    TEST_FIXTURE (ChangeScriptRepositoryTest, should_get_scripts_to_apply_by_latest_no)
    {
        list< shared_ptr<ChangeScript> > scripts;
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(1, "01_initial.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(2, "02_some_change.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(3, "03_another_change.sql", "")));

        ChangeScriptRepository repository(scripts);

        list< shared_ptr<ChangeScript> > scriptsToApply = repository.getScriptsToApply(1);
        ASSERT_EQUAL(scriptsToApply.size(), size_t(2));
        ASSERT_EQUAL(scriptsToApply.front()->getId(), 2);
        ASSERT_EQUAL(scriptsToApply.back()->getId(), 3);
    }

    TEST_FIXTURE (ChangeScriptRepositoryTest, should_throw_exception_when_miss_a_no)
    {
        list< shared_ptr<ChangeScript> > scripts;
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(1, "01_initial.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(3, "03_some_change.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(4, "04_another_change.sql", "")));

        try{
            ChangeScriptRepository repository(scripts);
            FAIL("should throw exception when 02 script not found");
        }catch(ScriptException& e){
            ASSERT_EQUAL(e.getMessage(), "Script with number 2 not found, but found 3");
        }
    }

    TEST_FIXTURE (ChangeScriptRepositoryTest, should_throw_exception_when_two_script_has_same_no)
    {
        list< shared_ptr<ChangeScript> > scripts;
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(1, "01_initial.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(2, "02_some_change.sql", "")));
        scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(2, "02_another_change.sql", "")));

        try{
            ChangeScriptRepository repository(scripts);
            FAIL("should throw exception when 02 script not found");
        }catch(ScriptException& e){
            ASSERT_EQUAL(e.getMessage(), "There is more than one change script with number 2");
        }
    }
}
