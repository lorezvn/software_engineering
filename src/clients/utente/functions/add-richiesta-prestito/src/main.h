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
#include "../../../../../classes/src/richiesta_prestito.h"
#include "../../../../../utilities/src/functions.h"
#include "../../../../../utilities/src/constants.h"

#define READ_STREAM "add-richiesta-prestito-in"
#define WRITE_STREAM "add-richiesta-prestito-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "utente"
#define POSTGRESQL_PASSWORD "utente"
#define POSTGRESQL_DBNAME "biblioteca"

#endif 
