#include "stdafx.h"
#include "SSHChanner.h"
#include <sstream>

SSHChanner::SSHChanner(const SSHSession & session)
    : SSHObject{ "SSHChanner" }, channel{ nullptr }, is_open{ false } {
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

    is_open = true;
}

void SSHChanner::close() noexcept {
    if (is_open)
        ssh_channel_close(channel);
    is_open = false;
}

std::string SSHChanner::requestAndGetResult(const std::string & request)
{
	requestPty();
	changePtySize(80, 24);
	requestShell();

    requestExec(request);

    auto result{ read() };
    sendEof();

    return result;
}

void SSHChanner::requestPty() {
	throwErrorIfNotOk(
		ssh_channel_request_pty(channel),
		"Cant request pty"
	);
}

void SSHChanner::changePtySize(int cols, int rows) {
	throwErrorIfNotOk(
		ssh_channel_change_pty_size(channel, cols, rows),
		"Cant change pty size"
	);
}

void SSHChanner::requestExec(const std::string & request) {
    throwErrorIfNotOk(
        ssh_channel_request_exec(channel, request.c_str()),
        "Cant request Exec"
    );
}

void SSHChanner::requestShell() {
	throwErrorIfNotOk(
		ssh_channel_request_shell(channel),
		"Cant request shell"
	);
}

std::string SSHChanner::read() {
    char buffer[256];
    int nbytes;

    std::stringstream stream{};
	while (!isEof()) {
		memset(buffer, 0, sizeof(buffer));

		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
        stream << buffer;
    }
    if (nbytes < 0)
        throwError("Cant read");

    return stream.str();
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
        throwError("Fail to create channer");
}

void SSHChanner::free() noexcept {
    ssh_channel_free(channel);
}
