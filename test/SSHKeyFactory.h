#pragma once

#include "SSHObject.h"
#include "SSHSession.h"
#include "SSHKey.h"

class SSHKeyFactory : public SSHObject {
public:
    SSHKeyFactory(const SSHSession &session);
    ~SSHKeyFactory() noexcept;

    SSHKey createServerPublickey();
private:
    void throwErrorIfNotOk(int code);

    const SSHSession *session;
};