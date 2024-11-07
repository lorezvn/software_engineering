#ifndef main_h
#define main_h

#include <cstring> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h> 

#include "../../services/database/src/con2db.h"
#include "../../utilities/constants.h"

#define DEBUG 1000

#define MAX_CONNECTION_TIME_AVG 30000.0
#define MAX_RESPONSE_TIME_AVG 30000.0
#define MIN_SUCCESS_RATE 95.0

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "admin"
#define POSTGRESQL_PASSWORD "admin"
#define POSTGRESQL_DBNAME "logdb_biblioteca"

int micro_sleep(long usec);

#endif