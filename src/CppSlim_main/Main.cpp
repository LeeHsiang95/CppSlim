#include "Slim.hpp"
#include "SocketServer.hpp"
#include "SlimConnectionHandler.hpp"
#include <stdlib.h>
#include <string.h>

int main(int ac, char** av)
{
    auto server = std::make_shared<SocketServer>();
    server->Run(av[1]);
    return 0;
}

