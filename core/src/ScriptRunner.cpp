#include "ScriptRunner.h"

#include <iostream>

#include "Value.h"
#include "ChangeScript.h"
#include "config/ClearOptions.h"

#include "PostgresSqlScriptRunner.h"

map<string, shared_ptr<ScriptRunner> > ScriptRunner::runners;

ScriptRunner::ScriptRunner(){
}

ScriptRunner::~ScriptRunner(){
}

shared_ptr<ScriptRunner> ScriptRunner::getRunner(string dbengine){
    return runners[dbengine];
}

void ScriptRunner::registRunner(string name, shared_ptr<ScriptRunner> runner){
    runners[name] = runner;
}

void ScriptRunner::init(){
    registRunner("postgres", shared_ptr<ScriptRunner>(new PostgresSqlScriptRunner()));
}


SqlScriptRunner::SqlScriptRunner(){
}

SqlScriptRunner::~SqlScriptRunner(){
}

RunnerAdder::RunnerAdder(string name, ScriptRunner* runner){
    cout << "add runner" << name<<endl;
//  ScriptRunner::registRunner(name, runner);
}
