#include <iostream>
#include <string>
#include "SSHSession.h"

int main() {
    try {
        SSHSession session{ SSHInfo{"localhost"} };

        session.connect();
    } catch (const std::runtime_error &error) {
        std::cout << error.what();
    }

    return 0;
}