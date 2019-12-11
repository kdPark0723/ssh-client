#pragma once

#include "SSHHashKey.h"

class SSHHaxa {
public:
    SSHHaxa(unsigned char *hash, size_t length);
    ~SSHHaxa();

    static void print(const SSHHashKey &key);
    static void print(unsigned char *hash, size_t length);
private:
    char *content;
};

