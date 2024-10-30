\c :log_dbname postgres

GRANT ALL PRIVILEGES ON DATABASE :log_dbname to :admin;

ALTER TABLE Client OWNER TO :admin;
ALTER TABLE Requests OWNER TO :admin;

GRANT ALL ON SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin;

GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;

-- Privilegi handler
GRANT CONNECT ON DATABASE :log_dbname TO :handler;
GRANT SELECT, INSERT, UPDATE ON Client TO :handler;
GRANT SELECT, INSERT, UPDATE ON Requests TO :handler;