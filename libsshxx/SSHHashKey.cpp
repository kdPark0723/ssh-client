#include "SSHHashKey.h"

SSHHashKey::SSHHashKey(unsigned char * hash, size_t length, bool is_public)
    : content{ new SSHHashKeyContent{ hash, length, is_public } } {
}

SSHHashKey::~SSHHashKey() {
}

unsigned char * SSHHashKey::getHash() const {
    return content->hash;
}

size_t SSHHashKey::getLegnth() const {
    return content->length;
}
