#pragma once

#include "SSHObject.h"
#include "SSHInfo.h"
#include "SSHKey.h"

class SSHSession : public SSHObject {
public:
    SSHSession(const SSHInfo &info);
    SSHSession(const SSHSession &rhs);
    ~SSHSession() noexcept;

    SSHSession& operator=(const SSHSession &rhs);
    SSHSession& operator=(SSHSession &&rhs);

    void setHostOption(const std::string &host);
    void setPortOption(unsigned int port);
    void setUserOption(const std::string &user);
    void setSSHDIrOptionDefault();
    void setSSHDIrOption(const std::string &dir);

    void setOption(enum ssh_options_e type, const void *value);

    void connect();
    void disconnect() noexcept;

    void userauthPassword(std::string password);

    void verifyKnownhost();
    int isKnownServer() const;
    void updateKnownHosts();

    std::string getError() const;

    const ssh_session getInternal() const;
private:
    void init();
    void free() noexcept;

    void throwErrorIfNotOk(int code);

    ssh_session session;
    bool is_connecting;
};

