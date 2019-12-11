#pragma once

#include "SSHObject.h"

class SSHKeyContent : public SSHObject {
public:
    SSHKeyContent(ssh_key key);
    ~SSHKeyContent();

    ssh_key key;
};

