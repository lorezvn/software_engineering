#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <cassert>
#include <cerrno>
#include <list>

#include "../../../../../services/database/src/con2db.h"
#include "../../../../../services/redis/src/con2redis.h"
#include "../../../../../classes/src/libro_edizione.h"
#include "../../../../../utilities/src/functions.h"
#include "../../../../../utilities/src/constants.h"

#define READ_STREAM "ricerca-libri-in"
#define WRITE_STREAM "ricerca-libri-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "utente"
#define POSTGRESQL_PASSWORD "utente"
#define POSTGRESQL_DBNAME "biblioteca"

#endif
