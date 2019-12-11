#pragma once

#include "SSHObject.h"
#include "SSHSession.h"

class SSHInternalSession : public SSHObject {
public:
    SSHInternalSession(ssh_session intenal);
    ~SSHInternalSession() noexcept;

    const ssh_session intenal;
};

