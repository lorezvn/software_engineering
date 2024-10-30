-- Crea il database 
DROP DATABASE IF EXISTS :log_dbname;
CREATE DATABASE :log_dbname;

\c :log_dbname postgres


-- Crea l'utente admin
REASSIGN OWNED BY :admin TO postgres;


-- Crea l'utente handler
REASSIGN OWNED BY :handler TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :handler;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :handler;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :handler;

DROP OWNED BY :handler;
DROP USER IF EXISTS :handler;

CREATE USER :handler WITH ENCRYPTED PASSWORD 'handler';