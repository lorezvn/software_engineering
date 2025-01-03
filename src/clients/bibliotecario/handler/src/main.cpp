#include "main.h"

int main() {
    int num_requests = 13;
    std::string requests[num_requests] = {"add-bibliotecario", "add-prestito", "add-richiesta-restock",
                                          "add-sanzione", "estingui-sanzione", "revoca-sanzione", 
                                          "visualizza-richieste-prestito", "termina-prestito", 
                                          "rifiuta-richiesta-prestito", "visualizza-restock",
                                          "visualizza-copie", "add-edizione", "delete-copia"};

    Server server(SERVER_ID, 42069, REDIS_SERVER, REDIS_PORT, requests, num_requests);

    server.run();
}