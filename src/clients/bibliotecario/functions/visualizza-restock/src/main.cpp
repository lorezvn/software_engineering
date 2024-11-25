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
        sprintf(query, "SELECT * FROM Restock");

        query_res = db.execQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Restock*> restocks;

        for (int row = 0; row < PQntuples(query_res); row++) {
            Restock* restock = new Restock(
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "richiesta"))),
                atoi(PQgetvalue(query_res, row, PQfnumber(query_res, "quantita"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "istante"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "fornitore"))),
                std::string(PQgetvalue(query_res, row, PQfnumber(query_res, "edizione")))
            );

            restocks.push_back(restock);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for (int row = 0; row < PQntuples(query_res); row++) {
            Restock* restock = restocks.front();
            restocks.pop_front();

            reply = RedisCommand(
                c2r,
                "XADD %s * row %d richiesta_id %d quantita %d istante %s forn_nome %s edizione %s",
                WRITE_STREAM, row,
                restock->richiesta_id, restock->quantita,
                restock->istante.c_str(), restock->forn_nome.c_str(),
                restock->edizione.c_str()
            );
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
        }
    }

    db.endDBConnection();
    return 0;
}
