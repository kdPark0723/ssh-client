#include "SSHSesstionException.h"



SSHSesstionException::SSHSesstionException(const std::string &message)
    : std::runtime_error(message)
{
}


SSHSesstionException::~SSHSesstionException()
{
}
