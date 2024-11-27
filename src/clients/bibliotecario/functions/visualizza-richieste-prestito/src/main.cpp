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

        // Creazione query per richieste di prestito in attesa
        sprintf(query, "SELECT * FROM RichiestaPrestito WHERE stato = 'IN ATTESA'");

        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<RichiestaPrestito*> richieste;

        for (int row = 0; row < PQntuples(query_res); row++) {
            RichiestaPrestito* richiesta = new RichiestaPrestito(
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "utente"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "edizione"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "istante"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "stato")))
            );

            richieste.push_back(richiesta);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for (int row = 0; row < PQntuples(query_res); row++) {
            RichiestaPrestito* richiesta = richieste.front();
            richieste.pop_front();

            reply = RedisCommand(
                c2r,
                "XADD %s * row %d utente_id %d edizione %d data_inizio %s data_fine %s istante %s stato %s",
                WRITE_STREAM, row,
                richiesta->utente_id, richiesta->edizione.c_str(),
                richiesta->istante.c_str(), richiesta->stato.c_str()
            );
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }

    db.endDBConnection();
    return 0;
}
