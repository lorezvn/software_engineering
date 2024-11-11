\c :log_dbname

-- Definizione dei domini
CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringL AS VARCHAR(500);
CREATE DOMAIN RealGEZ AS REAL CHECK (VALUE >= 0);

-- Definizione dei tipi
--CREATE TYPE ClientStatus AS ENUM ('ACTIVE', 'CLOSED', 'ERROR');
CREATE TYPE SessionStatsType AS ENUM ('SESSION', 'RESPONSE', 'CONNECTION_SUCCESS_RATE');
CREATE TYPE ResponseStatus AS ENUM ('SUCCESS', 'ERROR');

-- Tabella Client
CREATE TABLE IF NOT EXISTS Client (
    serverName StringS NOT NULL,
    fileDescriptor INTEGER NOT NULL,
    connTime TIMESTAMP NOT NULL,
    disconnTime TIMESTAMP,
    --connStatus ClientStatus NOT NULL,
    PRIMARY KEY (serverName, fileDescriptor, connTime),
    CHECK (disconnTime IS NULL OR connTime <= disconnTime)
);

-- Tabella Requests
CREATE TABLE IF NOT EXISTS Requests (
    request StringL NOT NULL,
    requestTime TIMESTAMP NOT NULL,
    responseStatus StringS,
    responseTime TIMESTAMP,
    clientServerName StringS NOT NULL,
    clientFileDescriptor INTEGER NOT NULL,
    clientConnTime TIMESTAMP NOT NULL,
    PRIMARY KEY (clientServerName, clientFileDescriptor, clientConnTime, requestTime),
    FOREIGN KEY (clientServerName, clientFileDescriptor, clientConnTime) REFERENCES Client(serverName, fileDescriptor, connTime),
    CHECK (responseTime IS NULL OR requestTime <= responseTime),
    CHECK (clientConnTime <= requestTime)
);

-- Tabella SessionStats
CREATE TABLE IF NOT EXISTS SessionStats (
    sessionID SERIAL PRIMARY KEY,
    sessionType SessionStatsType NOT NULL,
    endTime TIMESTAMP NOT NULL,
    value RealGEZ NOT NULL,
    responseStatus ResponseStatus NOT NULL,
    UNIQUE (sessionType, endTime)
);