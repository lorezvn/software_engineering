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
    char second_key[KEY_SIZE];
    char richiesta_id[PARAMETERS_SIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PASSWORD, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while (true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main fornitore BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        }

        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Controllo per la prima coppia chiave-valore
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);

        if (strcmp(first_key, "client_id") != 0) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Controllo per la seconda coppia chiave-valore
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);
        ReadStreamMsgVal(reply, 0, 0, 3, richiesta_id);

        int max_elements = ReadStreamMsgNumVal(reply, 0, 0);
        if (strcmp(second_key, "richiesta_id") != 0 || max_elements > 4) {
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }


        // Esecuzione della query di aggiornamento
        sprintf(query,
                "UPDATE RichiestaRestock SET stato = 'RIFIUTATA' "
                "WHERE id = %d ",
                atoi(richiesta_id));

        query_res = db.execQuery(query, false);

        // Controllo del risultato della query
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        // Invio della risposta di successo
        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
    }

    db.endDBConnection();
    return 0;
}
