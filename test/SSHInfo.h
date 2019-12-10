#pragma once

#include <string>

struct SSHInfo
{
    std::string host{ "" };
    unsigned int port{ 22 };
    std::string user{ "" };
};

