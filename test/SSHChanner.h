#pragma once

#include "SSHObject.h"
#include "SSHSession.h"

class SSHChanner: public SSHObject {
public:
    SSHChanner(const SSHSession& session);
    ~SSHChanner();

    void open();
    void close() noexcept;

    void getReuestResult(const std::string &request);
    void requestExec(const std::string &request);
    std::string read();

private:
    void init(const SSHSession & session);
    void free() noexcept;

    ssh_channel channel;
    bool is_open;
};

