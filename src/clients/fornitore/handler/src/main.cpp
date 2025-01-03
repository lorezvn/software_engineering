#include "main.h"

int main() {
    int num_requests = 4;
    std::string requests[num_requests] = {"add-fornitore", "add-restock", "visualizza-richieste-restock",
                                          "rifiuta-richiesta-restock"};

    Server server(SERVER_ID, 42070, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}