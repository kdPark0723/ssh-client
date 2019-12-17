#pragma once

#include "SSHObject.h"
#include "SSHSession.h"

class SSHChanner : public SSHObject {
public:
    SSHChanner(const SSHSession& session);
    ~SSHChanner();

    void open();
    bool isOpen() const;

    void close() noexcept;

    void requestPty();
    void changePtySize(int rows, int cols);
    void requestShell();

    std::string reuestAndGetResult(const std::string &request);
    void requestExec(const std::string &request);

    std::string read();
    void write(const std::string &);

    void sendEof();
    bool isEof() const;

private:
    void init(const SSHSession & session);
    void free() noexcept;

    ssh_channel channel;
    int timeout_millisecond;
};

