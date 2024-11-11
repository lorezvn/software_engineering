-- Crea il database
DROP DATABASE IF EXISTS :dbname;
CREATE DATABASE :dbname;

\c :dbname postgres


-- Crea l'utente admin
REASSIGN OWNED BY :admin TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :admin;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :admin;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :admin;

DROP OWNED BY :admin;
DROP USER IF EXISTS :admin;

CREATE USER :admin WITH ENCRYPTED PASSWORD 'admin';


-- Crea l'utente bibliotecario
REASSIGN OWNED BY :bibliotecario TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :bibliotecario;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :bibliotecario;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :bibliotecario;

DROP OWNED BY :bibliotecario;
DROP USER IF EXISTS :bibliotecario;

CREATE USER :bibliotecario WITH ENCRYPTED PASSWORD 'bibliotecario';


-- Crea l'utente utente
REASSIGN OWNED BY :utente TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :utente;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :utente;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :utente;

DROP OWNED BY :utente;
DROP USER IF EXISTS :utente;

CREATE USER :utente WITH ENCRYPTED PASSWORD 'utente';


-- Crea l'utente fornitore
REASSIGN OWNED BY :fornitore TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :fornitore;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :fornitore;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :fornitore;

DROP OWNED BY :fornitore;
DROP USER IF EXISTS :fornitore;

CREATE USER :fornitore WITH ENCRYPTED PASSWORD 'fornitore';

