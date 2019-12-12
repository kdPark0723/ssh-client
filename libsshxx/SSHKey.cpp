#include "SSHKey.h"

SSHKey::SSHKey(ssh_key key)
    : SSHObject{ "SSHKey" }, content{ new SSHKeyContent{ key } } {
}

SSHKey::~SSHKey() noexcept {
}

bool SSHKey::isPublic() const {
    return ssh_key_is_public(content->key);
}

SSHHashKey SSHKey::hash()
{
    unsigned char *hash = nullptr;
    size_t hlen;

    if (isPublic()) {
        SSHObject::throwErrorIfNotOk(
            ssh_get_publickey_hash(
                content->key,
                SSH_PUBLICKEY_HASH_SHA1,
                &hash,
                &hlen
            ),
            "Cant hash publickey"
        );

        return SSHHashKey{ hash, hlen, true };
    }

    SSHObject::throwError("Unsupport function");
    return SSHHashKey{ nullptr, 0, false };
}
