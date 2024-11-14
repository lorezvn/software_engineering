#include "main.h"

int main() {
    int num_requests = 1;
    std::string requests[num_requests] = {"add-bibliotecario"};

    Server server(SERVER_ID, 42069, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}