#include "main.h"

int main() {
    int num_requests = 5;
    std::string requests[num_requests] = {"add-bibliotecario", "add-prestito", "add-sanzione", "estingui-sanzione", "revoca-sanzione"};

    Server server(SERVER_ID, 42069, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}