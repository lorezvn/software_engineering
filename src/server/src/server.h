#ifndef server_h
#define server_h

#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h> 
#include <cerrno>
#include <vector>
#include <sstream>
#include <signal.h>

#include "handler.h"
#include "../../services/database/src/con2db.h"
#include "../../utilities/src/constants.h"

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "handler"
#define POSTGRESQL_PASSWORD "handler"
#define POSTGRESQL_DBNAME "logdb_biblioteca"

#define MAX_CONNECTIONS 100

class Server {
    public:
        Server(const char* server_id, int server_port, const char* redis_ip, int redis_port, std::string requests[], int num_requests);
        ~Server();
        void run();

    private:
        void addNewClients();
        void receiveData(int i);
        void sendResponse(int client_id, std::string out_str);
        void closeConnections();

        Con2DB logdb = Con2DB(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
        PGresult *query_res;

        const char* server;
        int socket_fd;
        int socket_port;
        fd_set current_set;
        int max_fd;
        char error_msg[ERROR_MSG_SIZE];

        Handler* handler;
};

#endif