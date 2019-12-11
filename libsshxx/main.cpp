#include <iostream>
#include <string>

#include "SSHSession.h"
#include "SSHChanner.h"
#include "SSHException.h"

int main() {
    try {
        std::cout << "Input host: ";
        std::string host{};
        std::cin >> host;

        SSHSession session{ SSHInfo{host} };

        session.connect();
        session.verifyKnownhost();

        std::cout << "Input password: ";
        std::string password{};
        std::cin >> password;

        session.userauthPassword(password);

        SSHChanner channer{ session };

        std::cout << "Input request: ";
        std::string request{};
        std::cin >> request;

        auto result{ channer.reuestAndGetResult(request) };
        std::cout << result;
    } catch (const SSHException &error) {
        std::cerr << error.what();
    }

    return 0;
}