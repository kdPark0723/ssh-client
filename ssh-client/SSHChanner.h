#pragma once

#include "SSHObject.h"
#include "SSHSession.h"

class SSHChanner: public SSHObject {
public:
    SSHChanner(const SSHSession& session);
    ~SSHChanner();

    void open();
    void close() noexcept;

    void requestPty();
    void changePtySize(int cols, int rows);

    std::string requestAndGetResult(const std::string &request);
    void requestExec(const std::string &request);
    void requestShell();

    std::string read();

    void sendEof();
	bool isEof() const;

private:
    void init(const SSHSession & session);
    void free() noexcept;

    ssh_channel channel;
    bool is_open;
};

