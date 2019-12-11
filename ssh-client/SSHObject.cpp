#include "stdafx.h"
#include "SSHObject.h"
#include "SSHException.h"

SSHObject::SSHObject(const std::string & name) 
    : name{ name } {
}

SSHObject::~SSHObject() noexcept {
}

void SSHObject::throwErrorIfNotOk(int code, const std::string & message) {
    if (code != SSH_OK)
        throwError(message);
}

void SSHObject::throwError(const std::string & message)
{
    throw SSHException(name + ": " + message);
}
