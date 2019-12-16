#include <iostream>
#include <string>

#include "SSHSession.h"
#include "SSHChanner.h"
#include "SSHException.h"


// $(SolutionDir)..\libssh\include
// $(SolutionDir)..\libssh\lib\x86
// $(SolutionDir)..\libssh\lib\x64
int main() {
    try {
        std::cout << "Input host: ";
        std::string host{};
        std::cin >> host;

        std::cout << "Input username: ";
        std::string username{};
        std::cin >> username;

        SSHSession session{ SSHInfo{host, 22, username} };

        session.setBlocking(true);

        session.setSSHDirOption(".");

        session.connect();
        session.verifyKnownhost();

        std::cout << "Input password: ";
        std::string password{};
        std::cin >> password;

        session.userauthPassword(password);

        std::string request{};
        while (true) {
            SSHChanner channel{ session };
            channel.open();

            std::cout << "Input request: ";
            std::cin >> request;

            if (request == "exit")
                return 0;

            auto result{ channel.reuestAndGetResult(request) };
            std::cout << result << "\n";
        }
    } catch (const SSHException &error) {
        std::cerr << error.what();
    }

    return 0;
}