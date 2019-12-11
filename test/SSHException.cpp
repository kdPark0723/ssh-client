#include "SSHException.h"


SSHException::SSHException(const std::string &message)
    : std::runtime_error(message) {
}


SSHException::~SSHException() noexcept {
}
