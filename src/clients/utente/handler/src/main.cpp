#include "main.h"

int main() {
    int num_requests = 5;
    std::string requests[num_requests] = {"add-utente", "add-richiesta-prestito", "ricerca-libri", 
                                          "update-username", "visualizza-sanzioni"};

    Server server(SERVER_ID, 42071, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}