#include <string>
#include <list>
#include <map>

#include "BaseTest.h"
#include <iostream>
#include "ScriptRunner.h"
#include "ChangeScriptRepository.h"
#include "Value.h"

using namespace std;

SUITE(ChangeScriptRepositoryTest){
    class ChangeScriptRepositoryTest {
    protected:
    	shared_ptr<ChangeScriptRepository> m_repository;
    public:
    	ChangeScriptRepositoryTest() {
    		list< shared_ptr<ChangeScript> > scripts;
    		scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(1, "01_initial.sql", "")));
    		scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(2, "02_some_change.sql", "")));
    		scripts.push_back(shared_ptr<ChangeScript>(new ChangeScript(3, "03_another_change.sql", "")));

    		m_repository = shared_ptr<ChangeScriptRepository>(new ChangeScriptRepository(scripts));
        }
        virtual ~ChangeScriptRepositoryTest() {
        }
    };

    TEST_FIXTURE (ChangeScriptRepositoryTest, should_get_scripts_to_apply_by_latest_no)
    {
    	list< shared_ptr<ChangeScript> > scripts = m_repository->getScriptsToApply(1);
    	ASSERT_EQUAL(scripts.size(), size_t(2));
    	ASSERT_EQUAL(scripts.front()->getId(), 2);
    	ASSERT_EQUAL(scripts.back()->getId(), 3);
    }
}
