#include "con2db.h"
#include <iostream>


Con2DB::Con2DB(const char *hostname, const char *port, const char *username, const char *password, const char *dbname) {

    char info[1000]; 

    sprintf(info, "host=%s port=%s user=%s password=%s dbname=%s", hostname, port, username, password, dbname);

    conn = PQconnectdb(info);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Con2DB(%s): Connessione al database fallita: %s\n", dbname, PQerrorMessage(conn));
        endDBConnection();
    }
}

// Per comandi che non ritornano dati e.g. INSERT, UPDATE, DELETE
PGresult* Con2DB::execActionQuery(char *query) {
    if (conn == NULL) {
        fprintf(stderr, "execActionQuery(): Connessione al database assente\n");
        exit(1);
    }

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "execActionQuery(): Errore durante l'esecuzione della query: %s\n", query);
        fprintf(stderr, "Dettagli dell'errore: %s\n", PQerrorMessage(conn));
        //PQclear(res); 
        //endDBConnection();
    }

    return res;
}

// Per comandi che ritornano dati e.g. SELECT
PGresult* Con2DB::execDataQuery(char *query) {
    if (conn == NULL) {
        fprintf(stderr, "execDataQuery(): Connessione al database assente\n");
        exit(1);
    }

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "execDataQuery(): Errore durante l'esecuzione della query: %s\n", query);
        fprintf(stderr, "Dettagli dell'errore: %s\n", PQerrorMessage(conn));
        //PQclear(res);
        //endDBConnection();
    }

    return res;
}

// Esegue una query e gestisce la transazione (BEGIN, COMMIT)
PGresult* Con2DB::execQuery(char* query, bool moreValue) {
    PGresult *res;     // Risultato della query principale
    PGresult *t_res;   // Risultato della query per la transazione
    char sql_cmd[7];    // Comandi SQL per transazioni (BEGIN, COMMIT)

    // BEGIN 
    sprintf(sql_cmd, "BEGIN");
    t_res = execActionQuery(sql_cmd);
    if (PQresultStatus(t_res) != PGRES_COMMAND_OK) {
        PQclear(t_res);
        return t_res;
    }
    PQclear(t_res);

    // Query principale
    res = !moreValue ? execActionQuery(query) : execDataQuery(query);

    // COMMIT
    sprintf(sql_cmd, "COMMIT");
    t_res = execActionQuery(sql_cmd);


    if(PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK){
        PQclear(t_res);
        PQclear(res);
        return res;
    }
    else if(PQresultStatus(t_res) != PGRES_COMMAND_OK){
        PQclear(res);
        PQclear(t_res);
        return t_res;
    }
    else {
        PQclear(t_res);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) PQclear(res);
        return res;
    }
}

// Disconnessione dal database
void Con2DB::endDBConnection() {
    PQfinish(conn); 
    conn = NULL;
    exit(1);
}
