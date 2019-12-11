#pragma once

#include "SSHException.h"

class SSHSesstionException : public SSHException {
public:
    SSHSesstionException(const std::string &message);
    ~SSHSesstionException() noexcept;
};

