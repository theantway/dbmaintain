#include "runner/ScriptRunner.h"

#include <iostream>

#include "Value.h"
#include "ChangeScript.h"
#include "config/ClearOptions.h"

#include "runner/PostgresSqlScriptRunner.h"

std::map<std::string, FactoryFunction> ScriptRunnerFactory::m_FactoryFunctions;

map<string, shared_ptr<ScriptRunner> > ScriptRunner::runners;
map<string, shared_ptr<SqlScriptRunner> > ScriptRunner::sqlRunners;

ScriptRunner::ScriptRunner(){
}

ScriptRunner::~ScriptRunner(){
}

shared_ptr<ScriptRunner> ScriptRunner::getRunner(string dbengine){
    return shared_ptr<ScriptRunner>();
}
shared_ptr<SqlScriptRunner> ScriptRunner::getSqlRunner(string dbengine){
    return sqlRunners[dbengine];
}

void ScriptRunner::registRunner(string name, shared_ptr<ScriptRunner> runner){
    runners[name] = runner;
}

void ScriptRunner::registSqlRunner(string name, shared_ptr<SqlScriptRunner> runner){
    sqlRunners[name] = runner;
}

void ScriptRunner::init(){
    ScriptRunnerFactory::Register("postgres", &PostgresSqlScriptRunner::createInstance);
//    registSqlRunner("postgres", shared_ptr<SqlScriptRunner>(new PostgresSqlScriptRunner()));
}


SqlScriptRunner::SqlScriptRunner(){
}

SqlScriptRunner::~SqlScriptRunner(){
}

RunnerAdder::RunnerAdder(string name, ScriptRunner* runner){
    cout << "add runner" << name<<endl;
//  ScriptRunner::registRunner(name, runner);
}
