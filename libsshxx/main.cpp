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

        std::cout << "Input username: ";
        std::string username{};
        std::cin >> username;

        SSHSession session{ SSHInfo{host, 22, username} };

        // ���� �߻� ����
        session.setSSHDIrOptionDefault();

        session.connect();
        session.verifyKnownhost();

        std::cout << "Input password: ";
        std::string password{};
        std::cin >> password;

        session.userauthPassword(password);

        SSHChanner channer{ session };
        channer.open();

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