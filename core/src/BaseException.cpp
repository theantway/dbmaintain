#include "BaseException.h"

BaseException::BaseException(string message) throw():m_message(message){
}

BaseException::~BaseException() throw(){
}

const char* BaseException::what() const throw(){
    return m_message.c_str();
}
