#include "stdafx.h"
#include "SSHHaxa.h"

#include <libssh/libssh.h>

SSHHaxa::SSHHaxa(unsigned char * hash, size_t length) 
    : content{ ssh_get_hexa(hash, length) } {
}

SSHHaxa::~SSHHaxa() {
    if (content != nullptr)
        ssh_string_free_char(content);
}

void SSHHaxa::print(const SSHHashKey & key) {
    SSHHaxa::print(key.getHash(), key.getLegnth());
}

void SSHHaxa::print(unsigned char * hash, size_t length) {
    ssh_print_hexa("Public key hash", hash, length);
}
