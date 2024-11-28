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

-- Permessi generali sui SEQUENCES
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :bibliotecario;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :fornitore;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :utente;

-- Privilegi del bibliotecario
GRANT CONNECT ON DATABASE :dbname TO :bibliotecario;
GRANT USAGE ON SCHEMA public TO :bibliotecario;
GRANT INSERT ON Bibliotecario TO :bibliotecario;
GRANT SELECT ON Utente TO :bibliotecario;                           
GRANT SELECT, INSERT, UPDATE, DELETE ON Prestito TO :bibliotecario; 
GRANT SELECT, DELETE ON LibroFisico TO :bibliotecario;                      
GRANT SELECT, INSERT ON LibroEdizione TO :bibliotecario;                    
GRANT SELECT ON Autore TO :bibliotecario;                           
GRANT SELECT ON CasaEditrice TO :bibliotecario;                     
GRANT SELECT, INSERT, UPDATE, DELETE ON RichiestaRestock TO :bibliotecario;
GRANT SELECT, INSERT, UPDATE, DELETE ON Sanzione TO :bibliotecario;
GRANT SELECT ON Restock TO :bibliotecario;
GRANT SELECT, INSERT, UPDATE, DELETE ON RichiestaPrestito TO :bibliotecario;

-- Privilegi dell'utente
GRANT CONNECT ON DATABASE :dbname TO :utente;
GRANT USAGE ON SCHEMA public TO :utente;
GRANT SELECT, INSERT, UPDATE ON Utente TO :utente;     
GRANT SELECT ON Prestito TO :utente;           
GRANT SELECT ON LibroFisico TO :utente;                
GRANT SELECT ON LibroEdizione TO :utente;              
GRANT SELECT ON Autore TO :utente;                    
GRANT SELECT ON CasaEditrice TO :utente;               
GRANT SELECT ON Sanzione TO :utente;
GRANT SELECT, INSERT, UPDATE, DELETE ON RichiestaPrestito TO :utente;

-- Privilegi del fornitore 
GRANT CONNECT ON DATABASE :dbname TO :fornitore;
GRANT USAGE ON SCHEMA public TO :fornitore;
GRANT INSERT ON Fornitore TO :fornitore;
GRANT INSERT, SELECT, UPDATE ON LibroEdizione TO :fornitore;       
GRANT INSERT, SELECT, UPDATE, DELETE ON LibroFisico TO :fornitore; 
GRANT INSERT, SELECT, UPDATE ON Autore TO :fornitore;              
GRANT INSERT, SELECT, UPDATE ON CasaEditrice TO :fornitore;        
GRANT SELECT, UPDATE ON RichiestaRestock TO :fornitore;
GRANT SELECT, INSERT, UPDATE, DELETE ON Restock TO :fornitore;
