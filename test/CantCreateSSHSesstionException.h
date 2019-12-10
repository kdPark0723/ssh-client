#pragma once

#include <stdexcept>

class CantCreateSSHSesstionException : public std::runtime_error
{
public:
    CantCreateSSHSesstionException(const std::string &message);
    ~CantCreateSSHSesstionException();
};

