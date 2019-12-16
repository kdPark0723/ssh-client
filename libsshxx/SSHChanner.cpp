#include "SSHChanner.h"

#include <sstream>


SSHChanner::SSHChanner(const SSHSession & session)
    : SSHObject{ "SSHChanner" }, channel{ nullptr }, timeout_millisecond{ 1000 } {
    init(session);
}

SSHChanner::~SSHChanner() {
    close();
    free();
}

void SSHChanner::open() {
    throwErrorIfNotOk(
        ssh_channel_open_session(channel),
        "Cant open channel"
    );
}

bool SSHChanner::isOpen() const {
    return ssh_channel_is_open(channel);
}

void SSHChanner::close() noexcept {
    if (isOpen())
        ssh_channel_close(channel);
}

void SSHChanner::requestPty() {
    throwErrorIfNotOk(
        ssh_channel_request_pty(channel),
        "Cant request pty"
    );
}

void SSHChanner::changePtySize(int rows, int cols) {
    throwErrorIfNotOk(
        ssh_channel_change_pty_size(channel, rows, cols),
        "Cant change pty size"
    );
}

void SSHChanner::requestShell() {
    throwErrorIfNotOk(
        ssh_channel_request_shell(channel),
        "Cant change pty size"
    );
}

std::string SSHChanner::reuestAndGetResult(const std::string & request) {
    requestExec(request);

    auto result{ read() };
    sendEof();

    return result;
}

void SSHChanner::requestExec(const std::string & request) {
    throwErrorIfNotOk(
        ssh_channel_request_exec(channel, request.c_str()),
        "Cant request Exec"
    );
}

std::string SSHChanner::read() {
    char buffer[256];
    int nbytes;

    std::stringstream stream{};
    while (!isEof()) {
        nbytes = ssh_channel_read_timeout(channel, buffer, sizeof(buffer), 0, timeout_millisecond);
        if (nbytes <= 0)
            break;

        stream << buffer;
    }        

    return stream.str();
}

void SSHChanner::write(const std::string &data) {
    ssh_channel_write(channel, data.c_str(), data.size());
}

void SSHChanner::sendEof() {
    ssh_channel_send_eof(channel);
}

bool SSHChanner::isEof() const {
    return ssh_channel_is_eof(channel);
}

void SSHChanner::init(const SSHSession & session) {
    if (channel == nullptr)
        channel = ssh_channel_new(session.getInternal());

    if (channel == nullptr)
        throwError("Fail to create session");
}

void SSHChanner::free() noexcept {
    ssh_channel_free(channel);
}
