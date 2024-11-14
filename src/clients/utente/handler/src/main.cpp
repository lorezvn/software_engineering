#include "main.h"

int main() {
    int num_requests = 2;
    std::string requests[num_requests] = {"add-utente", "update-username"};

    Server server(SERVER_ID, 42071, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}