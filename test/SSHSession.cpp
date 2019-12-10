#include "SSHSession.h"
#include "CantCreateSSHSesstionException.h"

SSHSession::SSHSession(const SSHInfo &info)
    : session{ ssh_new()},
    is_connecting{ false }
{
    if (session == nullptr)
        throw CantCreateSSHSesstionException("session should not null");
    
    ssh_options_set(session, SSH_OPTIONS_HOST, info.host.c_str());
    ssh_options_set(session, SSH_OPTIONS_PORT, &info.port);
    if (!info.user.empty())
        ssh_options_set(session, SSH_OPTIONS_USER, info.user.c_str());
}

SSHSession::~SSHSession()
{
    disconnect();

    ssh_free(session);
}

void SSHSession::connect()
{
    is_connecting = true;

    auto rc{ ssh_connect(session) };
    if (rc != SSH_OK)
        throw CantCreateSSHSesstionException(ssh_get_error(session));
}

void SSHSession::disconnect()
{
    if (is_connecting)
        ssh_disconnect(session);
}
