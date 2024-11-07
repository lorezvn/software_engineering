#ifndef handler_h
#define handler_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>
#include <iostream>

#include "../../services/redis/src/con2redis.h"
#include "../../services/database/src/con2db.h"
#include "../../utilities/constants.h"

using namespace std;

class Handler {
    public:
        Handler(const char* redis_ip, int redis_port, string requests[], int num_requests);
        bool sendToFunctions(int client_id, string msg);
        bool readFromFunctions(string* out_str_ptr, int* client_id_ptr);

    private:
        redisContext *c2r;
        string* requests;
        int num_requests;
        void initHandlerStreams();
};

#endif