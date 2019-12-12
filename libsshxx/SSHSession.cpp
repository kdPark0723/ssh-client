#include "SSHSession.h"
#include "SSHSesstionException.h"
#include "SSHKeyFactory.h"
#include "SSHHaxa.h"

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
    : SSHObject{ "SSHSessoin" }, session{ nullptr }, is_connecting{ false } {
    init();
    
    setHostOption(info.host);
    setPortOption(info.port);

    if (!info.user.empty())
        setUserOption(info.user);
}

SSHSession::SSHSession(const SSHSession & rhs)
    : SSHObject{ "SSHSessoin" }, session{ nullptr }, is_connecting{ false } {
    init();
    ssh_options_copy(rhs.session, &session);
}

SSHSession::~SSHSession() noexcept {
    disconnect();
    free();
}

SSHSession & SSHSession::operator=(const SSHSession & rhs) {
    disconnect();
    free();

    init();
    ssh_options_copy(rhs.session, &session);

    return *this;
}

SSHSession & SSHSession::operator=(SSHSession && rhs) {
    disconnect();
    free();

    is_connecting = rhs.is_connecting;
    session = rhs.session;

    rhs.is_connecting = false;
    rhs.session = nullptr;

    return *this;
}

void SSHSession::connect() {
    throwErrorIfNotOk(ssh_connect(session));

    is_connecting = true;
}

void SSHSession::disconnect() noexcept {
    if (is_connecting)
        ssh_disconnect(session);

    is_connecting = false;
}

void SSHSession::init() {
    if (session == nullptr)
        session = ssh_new();

    if (session == nullptr)
        throwError("Fail to create session");
}

void SSHSession::free() noexcept {
    if (session != nullptr)
        ssh_free(session);
    session = nullptr;
}

void SSHSession::setHostOption(const std::string & host) {
    setOptions(SSH_OPTIONS_HOST, host.c_str());
}

void SSHSession::setPortOption(unsigned int port) {
    setOptions(SSH_OPTIONS_PORT, &port);
}

void SSHSession::setUserOption(const std::string & user) {
    setOptions(SSH_OPTIONS_USER, user.c_str());
}

void SSHSession::setSSHDIrOption() {
    setOptions(SSH_OPTIONS_SSH_DIR, nullptr);
}

void SSHSession::setSSHDIrOption(const std::string & dir) {
    setOptions(SSH_OPTIONS_SSH_DIR, dir.c_str());
}

void SSHSession::setOptions(ssh_options_e type, const void * value) {
    SSHObject::throwErrorIfNotOk(
        ssh_options_set(session, type, value),
        "Cant set session's option"
    );
}

void SSHSession::optionsParseConfig() {
    SSHObject::throwErrorIfNotOk(
        ssh_options_parse_config(session, nullptr),
        "Cant options parse config"
    );
}

void SSHSession::optionsParseConfig(const std::string & config) {
    SSHObject::throwErrorIfNotOk(
        ssh_options_parse_config(session, config.c_str()),
        "Cant options parse config"
    );
}

void SSHSession::userauthPassword(std::string password) {
    throwErrorIfNotOk(ssh_userauth_password(session, nullptr, password.c_str()));
}

void SSHSession::verifyKnownhost() {
    /*
    SSHKeyFactory keyFactory{ *this };

    auto hashedPublicKey{ keyFactory.createServerPublickey().hash() };

    switch (isKnownServer()) {
    case SSH_KNOWN_HOSTS_OK:
        break;
    case SSH_KNOWN_HOSTS_CHANGED:
        SSHHaxa::print(hashedPublicKey);

        SSHObject::throwError(
            "Host key for server changed : it is now : \n"
            "For security reasons, connection will be stopped\n"
        );
    case SSH_KNOWN_HOSTS_OTHER:
        SSHObject::throwError(
            "The host key for this server was not found but an other type of key exists.\n"
            "An attacker might change the default server key to confuse your client into thinking the key does not exist\n"
        );
    case SSH_KNOWN_HOSTS_NOT_FOUND:
        SSHObject::throwError(
            "Could not find known host file.\n"
            "If you accept the host key here, the file will be automatically created.\n"
        );
    case SSH_KNOWN_HOSTS_UNKNOWN:
        updateKnownHosts();
        break;
    case SSH_KNOWN_HOSTS_ERROR:
        SSHObject::throwError(getError());
    }*/
}

int SSHSession::isKnownServer() const {
    return ssh_is_server_known(session);
}

void SSHSession::updateKnownHosts() {
    //throwErrorIfNotOk(ssh_session_update_known_hosts(session));
}

std::string SSHSession::getError() const {
    return ssh_get_error(session);
}

const ssh_session SSHSession::getInternal() const {
    return session;
}

void SSHSession::throwErrorIfNotOk(int code) {
    SSHObject::throwErrorIfNotOk(code, getError());
}
