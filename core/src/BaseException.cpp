#include "BaseException.h"

BaseException::BaseException(string message):m_message(message){
}

BaseException::~BaseException(){
}

string BaseException::getMessage(){
    return m_message;
}
