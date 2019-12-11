#pragma once

#include <stdexcept>

class SSHException : public std::runtime_error {
public:
    SSHException(const std::string &message);
    ~SSHException() noexcept;
};

