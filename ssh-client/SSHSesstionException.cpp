#include "stdafx.h"
#include "SSHSesstionException.h"

SSHSesstionException::SSHSesstionException(const std::string &message)
    : SSHException(message) {
}


SSHSesstionException::~SSHSesstionException() noexcept {
}
