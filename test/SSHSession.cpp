#include "SSHSession.h"
#include "CantCreateSSHSesstionException.h"

#include <libssh/libssh.h>

void throwErrorIfNotZero(int code, const std::string &message = "code is not zero") {
    if (code != 0)
        throw CantCreateSSHSesstionException(message);
}

void throwErrorIfNotOK(int code, ssh_session session) {
    if (code != SSH_OK)
        throw CantCreateSSHSesstionException(ssh_get_error(session));
}

SSHSession::SSHSession(const SSHInfo &info)
    : session{ ssh_new()},
    is_connecting{ false }
{
    if (session == nullptr)
        throw CantCreateSSHSesstionException("session should not null");
    
    throwErrorIfNotOK(
        ssh_options_set(session, SSH_OPTIONS_HOST, info.host.c_str()),
        session
    );
    throwErrorIfNotOK(
        ssh_options_set(session, SSH_OPTIONS_PORT, &info.port),
        session
    );

    // If it removed, connect happen out-of memory.
    throwErrorIfNotOK(
        ssh_options_set(session, SSH_OPTIONS_SSH_DIR, nullptr),
        session
    );
    if (!info.user.empty()) {
        throwErrorIfNotOK(
            ssh_options_set(session, SSH_OPTIONS_USER, info.user.c_str()),
            session
        );
    }
}

SSHSession::~SSHSession()
{
    disconnect();

    ssh_free(session);
}

void SSHSession::connect()
{
    is_connecting = true;

    throwErrorIfNotOK(ssh_connect(session), session);
}

void SSHSession::disconnect()
{
    if (is_connecting)
        ssh_disconnect(session);

    is_connecting = false;
}
