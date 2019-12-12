#include "SSHKeyFactory.h"


SSHKeyFactory::SSHKeyFactory(const SSHSession & session)
    : SSHObject{ "SSHKeyFactory" }, session { &session } {
}

SSHKeyFactory::~SSHKeyFactory() noexcept {
}

SSHKey SSHKeyFactory::createServerPublickey() {
    ssh_key srv_pubkey = nullptr;

    // throwErrorIfNotOk(ssh_get_server_publickey(session->getInternal(), &srv_pubkey));

    return SSHKey{ srv_pubkey };
}

void SSHKeyFactory::throwErrorIfNotOk(int code) {
    SSHObject::throwErrorIfNotOk(code, ssh_get_error(session->getInternal()));
}
