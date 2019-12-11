#include <iostream>
#include <string>

#include "SSHSession.h"
#include "SSHException.h"

int main() {
    try {
        SSHSession session{ SSHInfo{"localhost"} };

        session.connect();
        session.verifyKnownhost();

        std::string password{};
        std::cin >> password;

        session.userauthPassword(password);
    } catch (const SSHException &error) {
        std::cerr << error.what();
    }

    return 0;
}