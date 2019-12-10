#pragma once

#include <stdexcept>

class SSHSesstionException : public std::runtime_error
{
public:
    SSHSesstionException(const std::string &message);
    ~SSHSesstionException();
};

