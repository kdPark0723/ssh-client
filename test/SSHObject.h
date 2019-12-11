#pragma once

#include <libssh/libssh.h>
#include <string>

class SSHObject {
public:
    SSHObject(const std::string &name);
    ~SSHObject() noexcept;

protected:
    void throwErrorIfNotOk(int code, const std::string &message);
    virtual void throwError(const std::string &message);

    std::string name;
};

