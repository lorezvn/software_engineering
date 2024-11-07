#include "server.h"

bool run_server = true;

void termination_handler(int signum) {
    run_server = false;
    cout << "\n[INFO] Spegnimento server..." << endl;
}

Server::Server(const char* server_id, int server_port, const char* redis_ip, int redis_port, string requests[], int num_requests) {

    sockaddr_in address;

    server = server_id;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        sprintf(error_msg, "[%s]: Errore nella creazione del socket", server);
        throw runtime_error(error_msg);
    };
    socket_port = server_port;

    const int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("setsockopt(SO_REUSEADDR) fallita");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        sprintf(error_msg, "[%s]: Errore nel settare il socket a NON_BLOCKING", server);
        throw runtime_error(error_msg);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(socket_port);

    if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        sprintf(error_msg, "[%s]: Errore binding del socket", server);
        throw runtime_error(error_msg);
    }
    if (listen(socket_fd, MAX_CONNECTIONS) < 0) {
        sprintf(error_msg, "[%s]: Errore listening nel socket", server);
        throw runtime_error(error_msg);
    }

    handler = new Handler(redis_ip, redis_port, requests, num_requests);
}

Server::~Server() {
    closeConnections();
    logdb.endDBConnection();
}

void Server::run(){

    struct sigaction signal_handler;

    fd_set working_set;
    int rc, i, new_client, ready_requests;
    struct timeval timeout;

    int client_id;
    bool response;
    char query[QUERY_SIZE];
    string out_str;

    signal_handler.sa_handler = termination_handler;
    sigemptyset(&signal_handler.sa_mask);
    signal_handler.sa_flags = 0;

    sigaction(SIGINT, &signal_handler, NULL);
    sigaction(SIGTERM, &signal_handler, NULL);

    ready_requests = 0;
    FD_ZERO(&current_set);
    max_fd = socket_fd;
    FD_SET(socket_fd, &current_set);

    timeout.tv_sec  = 0;
    timeout.tv_usec = 5000;

    printf("[%s]: Server in esecuzione...\n", server);

    while(run_server) {

        memcpy(&working_set, &current_set, sizeof(current_set));

        rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0) {
            perror("Errore socket selection\n");
            break;
        }

        ready_requests = rc;

        for (i = 0; i <= max_fd && ready_requests > 0; ++i) {
            
            if ((i != socket_fd) && FD_ISSET(i, &working_set)) {
                ready_requests -= 1;
                receiveData(i);
            }
        }

        if (FD_ISSET(socket_fd, &working_set)) {
            addNewClients();
        }

        response = true;
        while(response) {
            out_str = "";
            client_id = -1;

            response = handler->readFromFunctions(&out_str, &client_id);

            if(response) {
                sendResponse(client_id, out_str);
            }
        }

        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;
    }

    closeConnections();
}

void Server::addNewClients() {

    int new_client;
    char query[QUERY_SIZE];

    do {
        new_client = accept(socket_fd, NULL, NULL);
        
        if (new_client < 0) {

            // Non ci sono altre connessioni in coda -> stop server
            if (errno != EWOULDBLOCK) {
                cout << "\nErrore accept client" << endl;
                run_server = false;
            }
            break;
        }

        sprintf(query, "INSERT INTO Client(serverName, fileDescriptor, connTime) VALUES (\'%s\', %d, CURRENT_TIMESTAMP)", server, new_client);
        query_res = logdb.execQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send(new_client, "SERVER_ERROR", 12, 0);
            close(new_client);
            return;
        }

        FD_SET(new_client, &current_set);
        if (new_client > max_fd) {
            max_fd = new_client;
        }
        if (fcntl(new_client, F_SETFL, O_NONBLOCK) < 0) {
            cout << "\nErrore accept client" << endl;
        }
    } while (new_client != -1);
}

void Server::sendResponse(int client_id, string out_str) {

    char query[QUERY_SIZE];

    size_t eol = out_str.find('\n');
    string first_line = out_str.substr(0, eol);


    const char* query_max_client_conn = 
        "WITH MaxClientConn AS ( "
        "   SELECT max(connTime) AS instant "
        "   FROM Client "
        "   WHERE serverName = \'%s\' " 
        "   AND fileDescriptor = %d), ";

    const char* query_last_request =
        "LastRequest AS ( "
        "   SELECT MAX(r.requestTime) AS instant "
        "   FROM Requests AS r, MaxClientConn AS m "
        "   WHERE r.clientServerName = \'%s\' " 
        "   AND r.clientFileDescriptor = %d "
        "   AND r.clientConnTime = m.instant) ";

    const char* query_update = 
        "UPDATE Requests "
        "SET responseStatus = \'%s\', responseTime = CURRENT_TIMESTAMP "
        "WHERE clientServerName = \'%s\' "
        "  AND clientFileDescriptor = %d "
        "  AND clientConnTime = (SELECT instant FROM MaxClientConn) "
        "  AND requestTime = (SELECT instant FROM LastRequest);";

    sprintf(query, 
           "%s%s%s", 
           query_max_client_conn, query_last_request, query_update,
           server, client_id, server, client_id, first_line.c_str(), server, client_id);

    query_res = logdb.execQuery(query, false);

    if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        send(client_id, "SERVER_ERROR", 12, 0);
        return;
    }

    cout << "\nClient id: " << client_id << " - Response: " << out_str << endl;
    send(client_id, out_str.c_str(), out_str.length(), 0);
}


void Server::receiveData(int i) {

    string	msg;
    char buffer[100];
    int rc;
    bool close_conn = false;
    char query[QUERY_SIZE];

    do {
        bzero(buffer, sizeof(buffer));
        rc = recv(i, buffer, sizeof(buffer) -1, 0);

        if (rc < 0) {
            if (errno != EWOULDBLOCK) {
                close_conn = true;
            }
            break;
        }
        if (rc == 0) {
            close_conn = true;
            break;
        }
        msg.append(buffer);
    } while (true);

    if (close_conn) {
        sprintf(query, "UPDATE Client SET disconnTime = CURRENT_TIMESTAMP WHERE serverName = \'%s\' AND fileDescriptor = %d AND disconnTime IS NULL", server, i);
        query_res = logdb.execQuery(query, false);

        close(i);
        FD_CLR(i, &current_set);
        if (i == max_fd) {
            while (FD_ISSET(max_fd, &current_set) == false)
                max_fd -= 1;
        }

        return;
    }

    sprintf(query, "INSERT INTO Requests(request, requestTime, clientServerName, clientFileDescriptor, clientConnTime)"
                   "VALUES (\'%s\', CURRENT_TIMESTAMP, \'%s\', %d, (SELECT connTime FROM Client WHERE disconnTime IS NULL and serverName = \'%s\' and fileDescriptor = %d))", msg.c_str(), server, i, server, i);

    query_res = logdb.execQuery(query, false);
    if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        send(i, "SERVER_ERROR", 12, 0);
        return;
    }

    if(!handler->sendToFunctions(i, msg)) {
        sendResponse(i, "BAD_REQUEST");
    }
}

void Server::closeConnections() {

    int i;
    char query[QUERY_SIZE];

    for (i=0; i <= max_fd; ++i) {
        if (FD_ISSET(i, &current_set)) {
            sprintf(query, "UPDATE Client SET disconnTime = CURRENT_TIMESTAMP WHERE serverName = \'%s\' AND fileDescriptor = %d AND disconnTime IS NULL", server, i);
            query_res = logdb.execQuery(query, false);
            close(i);
        }
    }
}



