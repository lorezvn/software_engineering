#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>


#include "../../../../../services/database/src/con2db.h"
#include "../../../../../services/redis/src/con2redis.h"
#include "../../../../../classes/src/libro_edizione.h"
#include "../../../../../utilities/src/functions.h"
#include "../../../../../utilities/src/constants.h"


#define READ_STREAM "add-edizione-in"
#define WRITE_STREAM "add-edizione-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "bibliotecario"
#define POSTGRESQL_PASSWORD "bibliotecario"
#define POSTGRESQL_DBNAME "biblioteca"

#endif