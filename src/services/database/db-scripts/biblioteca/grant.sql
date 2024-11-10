\c :dbname postgres

-- Concedi privilegi completi all'admin
GRANT ALL PRIVILEGES ON DATABASE :dbname TO :admin;
GRANT ALL ON SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public TO :admin;

-- Imposta admin come proprietario delle tabelle
ALTER TABLE Utente OWNER TO :admin;
ALTER TABLE CasaEditrice OWNER TO :admin;
ALTER TABLE LibroEdizione OWNER TO :admin;
ALTER TABLE LibroFisico OWNER TO :admin;
ALTER TABLE Prestito OWNER TO :admin;
ALTER TABLE Autore OWNER TO :admin;
ALTER TABLE Genere OWNER TO :admin;
ALTER TABLE GenereEdizione OWNER TO :admin;
ALTER TABLE EdizioneAutore OWNER TO :admin;

-- Permessi generali sui SEQUENCES
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :bibliotecario;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :fornitore;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :utente;

-- Privilegi del bibliotecario
GRANT CONNECT ON DATABASE :dbname TO :bibliotecario;
GRANT USAGE ON SCHEMA public TO :bibliotecario;
GRANT SELECT ON Utente TO :bibliotecario;                           -- Solo visualizzare dati utenti
GRANT SELECT, INSERT, UPDATE, DELETE ON Prestito TO :bibliotecario; -- Gestione completa dei prestiti
GRANT SELECT ON LibroFisico TO :bibliotecario;                      -- Visualizzare copie fisiche dei libri
GRANT SELECT ON LibroEdizione TO :bibliotecario;                    -- Visualizzare edizioni dei libri
GRANT SELECT ON Autore TO :bibliotecario;                           -- Visualizzare autori
GRANT SELECT ON CasaEditrice TO :bibliotecario;                     -- Visualizzare case editrici
GRANT SELECT ON GenereEdizione TO :bibliotecario;                   -- Visualizzare associazioni tra generi ed edizioni
GRANT SELECT ON EdizioneAutore TO :bibliotecario;                   -- Visualizzare associazioni tra edizioni e autori
GRANT SELECT, INSERT, UPDATE, DELETE ON PendingRestock TO :bibliotecario;
GRANT SELECT, INSERT, UPDATE, DELETE ON Sanzione TO :bibliotecario;
GRANT SELECT ON Restock TO :bibliotecario;
GRANT SELECT, INSERT, UPDATE, DELETE ON PendingPrestito TO :bibliotecario;
-- Privilegi dell'utente
GRANT CONNECT ON DATABASE :dbname TO :utente;
GRANT USAGE ON SCHEMA public TO :utente;
GRANT SELECT, INSERT, UPDATE ON Utente TO :utente;     -- Solo creare nuovi utenti o aggiornare dati personali
GRANT SELECT ON Prestito TO :utente;           -- Permessi sui prestiti personali
GRANT SELECT ON LibroFisico TO :utente;                -- Visualizzare copie fisiche dei libri
GRANT SELECT ON LibroEdizione TO :utente;              -- Visualizzare edizioni dei libri
GRANT SELECT ON Autore TO :utente;                     -- Visualizzare autori
GRANT SELECT ON CasaEditrice TO :utente;               -- Visualizzare case editrici
GRANT SELECT ON GenereEdizione TO :utente;             -- Visualizzare associazioni tra generi ed edizioni
GRANT SELECT ON EdizioneAutore TO :utente;             -- Visualizzare associazioni tra edizioni e autori
GRANT SELECT ON Sanzione TO :utente;
GRANT SELECT, INSERT, UPDATE, DELETE ON PendingPrestito TO :utente;
-- Privilegi del fornitore 
GRANT CONNECT ON DATABASE :dbname TO :fornitore;
GRANT USAGE ON SCHEMA public TO :fornitore;
GRANT INSERT, SELECT, UPDATE ON LibroEdizione TO :fornitore;       -- Gestione delle edizioni dei libri
GRANT INSERT, SELECT, UPDATE, DELETE ON LibroFisico TO :fornitore; -- Gestione completa delle copie fisiche dei libri
GRANT INSERT, SELECT, UPDATE ON Autore TO :fornitore;              -- Gestione degli autori
GRANT INSERT, SELECT, UPDATE ON CasaEditrice TO :fornitore;        -- Gestione delle case editrici
GRANT INSERT, SELECT, UPDATE ON GenereEdizione TO :fornitore;      -- Gestione associazioni tra generi ed edizioni dei libri
GRANT INSERT, SELECT, UPDATE ON EdizioneAutore TO :fornitore;      -- Gestione associazioni tra edizioni e autori dei libri
GRANT SELECT ON PendingRestock TO :fornitore;
GRANT SELECT, INSERT, UPDATE, DELETE ON Restock TO :fornitore;
