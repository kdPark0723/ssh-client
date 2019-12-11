#pragma once

#include "SSHObject.h"
#include "SSHSession.h"

class SSHChanner: public SSHObject {
public:
    SSHChanner(const SSHSession& session);
    ~SSHChanner();

    void open();
    void close() noexcept;

    std::string requestAndGetResult(const std::string &request);
    void requestExec(const std::string &request);
    std::string read();
    void sendEof();

private:
    void init(const SSHSession & session);
    void free() noexcept;

    ssh_channel channel;
    bool is_open;
};

