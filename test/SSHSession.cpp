#include "SSHSession.h"
#include "SSHSesstionException.h"

#include <libssh/libssh.h>
#include <algorithm>
#include <cctype>
#include <string>

void processKnownHostsChanged(unsigned char *hash, size_t hlen) {
    ssh_print_hexa("Public key hash", hash, hlen);
    ssh_clean_pubkey_hash(&hash);

    throw SSHSesstionException(
        "Host key for server changed: it is now:\n"
        "For security reasons, connection will be stopped\n"
    );
}

SSHSession::SSHSession(const SSHInfo &info)
    : session{ ssh_new() },
    is_connecting{ false } {
    if (session == nullptr)
        throw SSHSesstionException("Session should not null");
    
    setOption(SSH_OPTIONS_HOST, info.host.c_str());
    setOption(SSH_OPTIONS_PORT, &info.port);

    // 오류 발생 지점
    setOption(SSH_OPTIONS_SSH_DIR, nullptr);

    if (!info.user.empty())
        setOption(SSH_OPTIONS_USER, info.user.c_str());
}

SSHSession::~SSHSession() noexcept {
    disconnect();
    free();
}

void SSHSession::free() noexcept {
    if (session != nullptr)
        ssh_free(session);
}

void SSHSession::setOption(ssh_options_e type, const void * value) {
    throwErrorIfNotOk(
        ssh_options_set(session, type, value),
        "Cant set session's option"
    );
}

void SSHSession::connect() {
    is_connecting = true;

    throwErrorIfNotOk(ssh_connect(session));
}

void SSHSession::disconnect() noexcept {
    if (is_connecting)
        ssh_disconnect(session);

    is_connecting = false;
}

void SSHSession::verifyKnownhost() {
    /*
    enum ssh_known_hosts_e state;
    unsigned char *hash = nullptr;
    ssh_key srv_pubkey = nullptr;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;

    throwErrorIfNotOK(
        ssh_get_server_publickey(session, &srv_pubkey),
        session
    );

    throwErrorIfNotOK(
        ssh_get_publickey_hash(
            srv_pubkey,
            SSH_PUBLICKEY_HASH_SHA1,
            &hash,
            &hlen
        ),
        session
    );

    ssh_key_free(srv_pubkey);

    state = ssh_session_is_known_server(session);

    auto hostUnknownError{ SSHSesstionException(
        "The server is unknown. Do you trust the host key?\n"
    ) };

    switch (state) {
    case SSH_KNOWN_HOSTS_OK:
        break;
    case SSH_KNOWN_HOSTS_CHANGED:
        processKnownHostsChanged(hash, hlen);

    case SSH_KNOWN_HOSTS_OTHER:
        ssh_clean_pubkey_hash(&hash);

        throw SSHSesstionException(
            "The host key for this server was not found but an other type of key exists.\n"
            "An attacker might change the default server key to confuse your client into thinking the key does not exist\n"
        );
    case SSH_KNOWN_HOSTS_NOT_FOUND:
        throw SSHSesstionException(
            "Could not find known host file.\n"
            "AIf you accept the host key here, the file will be automatically created.\n"
        );
        
    case SSH_KNOWN_HOSTS_UNKNOWN:

        hexa = ssh_get_hexa(hash, hlen);
        
        ssh_string_free_char(hexa);
        ssh_clean_pubkey_hash(&hash);
        p = fgets(buf, sizeof(buf), stdin);
        if (p == NULL)
            throw hostUnknownError;

        std::string data{ buf };
        std::transform(
            data.begin(), data.end(), data.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        if (buf != "yes")
            throw hostUnknownError;

        rc = ssh_session_update_known_hosts(session);
        if (rc < 0)
            throw hostUnknownError;

    case SSH_KNOWN_HOSTS_ERROR:
        ssh_clean_pubkey_hash(&hash);

        throw SSHSesstionException(
            ssh_get_error(session)
        );
    }
    ssh_clean_pubkey_hash(&hash);*/
}

void SSHSession::throwErrorIfNotOk(int code) {
    if (code != SSH_OK)
        throwError(ssh_get_error(session));
}

void SSHSession::throwErrorIfNotOk(int code, const std::string & message) {
    if (code != SSH_OK)
        throwError(message);
}

void SSHSession::throwError(const std::string & message)
{
    throw SSHSesstionException(std::string{ "SSHSession: " } + message);
}
