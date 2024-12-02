#include "main.h"

// Requisiti non funzionali
    // 1. Il tempo medio di sessione di un client deve essere inferiore o uguale al tempo massimo di sessione.
    // 2. Il tempo medio di risposta del server ad una richiesta da parte di un client deve essere inferiore o uguale al tempo massimo di risposta.
int main() {

    Con2DB log_db = Con2DB(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);

    PGresult *query_res;
    char query[QUERY_SIZE];
    char response_status[8];

    while(true) {

        // Tempo medio di sessione
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(disconnTime - connTime)) * 1000 as avg FROM Client WHERE disconnTime IS NOT NULL");
        query_res = log_db.execQuery(query, true);

        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("Errore database\n");
            continue;
        }

        char *avg = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        if(strlen(avg) == 0) sprintf(avg, "0");

        sprintf(response_status, atof(avg) <= MAX_CONNECTION_TIME_AVG ? "SUCCESS" : "ERROR");

        sprintf(query, "INSERT INTO SessionStats(sessionType, endTime, value, responseStatus) VALUES ('SESSION', CURRENT_TIMESTAMP, %s, \'%s\')", avg, response_status);

        query_res = log_db.execQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("Errore database\n");
            continue;
        }


        // Tempo medio di risposta
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(responseTime - requestTime)) * 1000 as avg FROM Requests WHERE responseTime IS NOT NULL");
        query_res = log_db.execQuery(query, true);

        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("Errore database\n");
            continue;
        }

        avg = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        if(strlen(avg) == 0) sprintf(avg, "0");
        
        sprintf(response_status, atof(avg) <= MAX_RESPONSE_TIME_AVG ? "SUCCESS" : "ERROR");

        sprintf(query, "INSERT INTO SessionStats(sessionType, endTime, value, responseStatus) VALUES ('RESPONSE', CURRENT_TIMESTAMP, %s, \'%s\')", avg, response_status);

        query_res = log_db.execQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("Errore database\n");
            continue;
        }

        micro_sleep(60000000);
    }

    log_db.endDBConnection();  
}