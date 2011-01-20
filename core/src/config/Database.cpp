#include "config/Database.h"

Database::Database() {
}

Database::~Database() {
}

shared_ptr<ClearOptions> Database::getClearOptions(){
    if(!m_clearOptions){
        m_clearOptions = shared_ptr<ClearOptions>(new ClearOptions());
    }
    return m_clearOptions;
}

shared_ptr<ClearOptions> Database::getCleanOptions(){
    if(!m_cleanOptions){
        m_cleanOptions = shared_ptr<ClearOptions>(new ClearOptions());
    }
    return m_cleanOptions;
}

string Database::getDialect(){
    return m_dialect;
}

string Database::getUrl(){
    return m_url;
}

void Database::setDialect(string dialect){
    m_dialect = dialect;
}

void Database::setUrl(string url){
    m_url = url;
}

void Database::setClearOptions(shared_ptr<ClearOptions> options){
    m_clearOptions = options;
}

void Database::setCleanOptions(shared_ptr<ClearOptions> options){
    m_cleanOptions = options;
}
