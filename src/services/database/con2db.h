#ifndef con2db_h
#define con2db_h

#include <postgresql/libpq-fe.h>

class Con2DB {

private:
    PGconn *conn;

public:
    // Costruttore
    Con2DB(const char *hostname, const char *port, const char *username, const char *password, const char *dbname);

    // Per comandi che non ritornano dati e.g. INSERT, UPDATE, DELETE
    PGresult* execActionQuery(char *query);

    // Per comandi che ritornano dati e.g. SELECT
    PGresult* execDataQuery(char *query);

    // Funzione principale per eseguire query, chiama RunActionQuery o RunDataQuery a seconda del valore di moreValue
    PGresult* execQuery(char* query, bool moreValue);

    // Disconnessione dal database
    void endDBConnection();
};

#endif 
