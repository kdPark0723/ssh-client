#pragma once

#include <libssh/libssh.h>
#include "SSHInfo.h"

class SSHSession
{
public:
    SSHSession(const SSHInfo &info);
    ~SSHSession();

    void connect();
    void disconnect();

private:
    ssh_session session;
    bool is_connecting;
};

