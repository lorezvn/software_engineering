#include "main.h"

int main() {
    redisContext* c2r;
    redisReply* reply;

    PGresult* query_res;

    char query[QUERY_SIZE];
    char response[RESPONSE_SIZE];
    char msg_id[MSG_ID_SIZE];
    char first_key[KEY_SIZE];
    char client_id[VALUE_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while (true) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP main bibliotecario BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        }

        // stream_num = 0
        // msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check per la prima coppia chiave-valore
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);

        if (strcmp(first_key, "client_id") != 0) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Creazione query per visualizzare le copie disponibili (non attualmente in prestito)
        // per ogni edizione
        sprintf(query, 
                "SELECT le.ISBN, "
                "       le.titolo, "
                "       COALESCE(COUNT(lf.id) - COUNT(p.libro)) AS copie_disponibili "
                "FROM LibroEdizione le "
                "LEFT JOIN LibroFisico lf ON lf.edizione = le.ISBN "
                "LEFT JOIN Prestito p ON p.libro = lf.id AND p.isTerminato = FALSE "
                "GROUP BY le.ISBN, le.titolo"
        );

        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for (int row = 0; row < PQntuples(query_res); row++) {

            const char* isbn = PQgetvalue(query_res, row, PQfnumber(query_res, "ISBN"));
            const char* titolo = PQgetvalue(query_res, row, PQfnumber(query_res, "titolo"));
            int copie_disponibili = atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "copie_disponibili")));

            reply = RedisCommand(
                c2r,
                "XADD %s * row %d ISBN %s titolo %s copie_disponibili %d",
                WRITE_STREAM, row,
                isbn, titolo, copie_disponibili
            );

            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }

    db.endDBConnection();
    return 0;
}
