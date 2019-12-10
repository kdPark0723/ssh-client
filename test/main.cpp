#include <iostream>
#include <string>
#include "SSHSession.h"

int main() {
    try {
        SSHSession session{ SSHInfo{"localhost"} };

        session.connect();
        session.verifyKnownhost();
    } catch (const std::runtime_error &error) {
        std::cerr << error.what();
    }

    return 0;
}