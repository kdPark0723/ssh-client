#pragma once

#include <memory>

#include "SSHObject.h"
#include "SSHKeyContent.h"
#include "SSHHashKey.h"

class SSHKey : public SSHObject {
public:
    SSHKey(ssh_key key);
    ~SSHKey() noexcept;

    bool isPublic() const;

    SSHHashKey hash();
private:
    std::shared_ptr<SSHKeyContent> content;
};