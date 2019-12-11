#pragma once

class SSHHashKeyContent{
public:
    SSHHashKeyContent(unsigned char *hash, size_t length, bool is_public);
    ~SSHHashKeyContent();

    unsigned char *hash;
    size_t length;

private:
    bool is_public;
};

