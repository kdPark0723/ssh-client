#include "stdafx.h"
#include "SSHHashKeyContent.h"

#include <libssh/libssh.h>

SSHHashKeyContent::SSHHashKeyContent(unsigned char * hash, size_t length, bool is_public)
    : hash{ hash }, length{ length }, is_public{ is_public } {
}

SSHHashKeyContent::~SSHHashKeyContent() {
    if (hash != nullptr) {
        if (is_public) {
            ssh_clean_pubkey_hash(&hash);
        }
    }
}
