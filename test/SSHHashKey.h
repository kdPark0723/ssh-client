#pragma once

#include <memory>
#include "SSHHashKeyContent.h"

class SSHHashKey {
public:
    SSHHashKey(unsigned char *hash, size_t length, bool is_public);
    ~SSHHashKey();

    unsigned char *getHash() const;
    size_t getLegnth() const;
private:
    std::shared_ptr<SSHHashKeyContent> content;
};

