#pragma once

#include <libssh/libssh.h>
#include <string>
#include "SSHInfo.h"

class SSHSession {
public:
    SSHSession(const SSHInfo &info);
    ~SSHSession() noexcept;

    void setHostOption(const std::string &host);
    void setPortOption(unsigned int port);
    void setUserOption(const std::string &user);
    void setOption(enum ssh_options_e type, const void *value);

    void connect();
    void disconnect() noexcept;

    void verifyKnownhost();
private:
    void init();
    void free() noexcept;

    void throwErrorIfNotOk(int code);
    void throwErrorIfNotOk(int code, const std::string &message);
    void throwError(const std::string &message);

    ssh_session session;
    bool is_connecting;
};

