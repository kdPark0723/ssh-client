#include "stdafx.h"
#include "SSHKeyContent.h"

SSHKeyContent::SSHKeyContent(ssh_key key)
    : SSHObject{ "SSHKeyContext" }, key{ key } {
}

SSHKeyContent::~SSHKeyContent() {
    if (key != nullptr)
        ssh_key_free(key);
    key = nullptr;
}
