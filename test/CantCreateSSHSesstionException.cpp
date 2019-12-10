#include "CantCreateSSHSesstionException.h"



CantCreateSSHSesstionException::CantCreateSSHSesstionException(const std::string &message)
    : std::runtime_error(message)
{
}


CantCreateSSHSesstionException::~CantCreateSSHSesstionException()
{
}
