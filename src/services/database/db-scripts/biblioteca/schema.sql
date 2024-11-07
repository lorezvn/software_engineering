\c :dbname

-- Definizione dei domini
CREATE DOMAIN IntGZ AS INTEGER CHECK (VALUE > 0);
CREATE DOMAIN RealGEZ AS REAL CHECK (VALUE >= 0);
CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringM AS VARCHAR(100);
CREATE DOMAIN StringL AS VARCHAR(500);
CREATE DOMAIN CF AS VARCHAR(16) CHECK (VALUE ~ '^[A-Z]{6}[0-9]{2}[A-Z][0-9]{2}[A-Z][0-9]{3}[A-Z]$');
CREATE DOMAIN Email AS StringM CHECK (VALUE ~* E'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$');

-- Definizione dei tipi

CREATE TYPE Denaro AS (
    importo RealGEZ,
    valuta CHAR(3)
);
CREATE TYPE StatoRestock AS ENUM('IN ATTESA',  'COMPLETATO', 'ANNULLATA');
CREATE TYPE StatoSanzione AS ENUM('PAGATA', 'NON PAGATA');

-- Tabella Utente
CREATE TABLE IF NOT EXISTS Utente (
    cf CF PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email Email NOT NULL,
    UNIQUE(email)
);

-- Tabella CasaEditrice
CREATE TABLE IF NOT EXISTS CasaEditrice (
    nome StringM PRIMARY KEY
);

-- Tabella LibroEdizione
CREATE TABLE IF NOT EXISTS LibroEdizione (
    ISBN VARCHAR(13) PRIMARY KEY,
    titolo StringS NOT NULL,
    pagine IntGZ NOT NULL,
    casaEditrice StringM,
    FOREIGN KEY (casaEditrice) REFERENCES CasaEditrice(nome) ON DELETE SET NULL
);

-- Tabella LibroFisico
CREATE TABLE IF NOT EXISTS LibroFisico (
    id SERIAL PRIMARY KEY,
    edizione VARCHAR(13) NOT NULL,
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE
);

-- Tabella Prestito
CREATE TABLE IF NOT EXISTS Prestito (
    id SERIAL PRIMARY KEY,
    dataInizio DATE NOT NULL,
    dataFine DATE NOT NULL,
    dataRest DATE,
    isTerminato BOOLEAN DEFAULT FALSE,
    libro INTEGER NOT NULL,
    utente CF NOT NULL,
    CHECK (dataInizio < dataFine),
    CHECK (
        (isTerminato = FALSE OR dataRest IS NOT NULL)
        AND (dataRest IS NULL OR isTerminato = TRUE)
    ),
    CHECK (isTerminato = FALSE OR (dataInizio <= dataRest AND dataRest <= dataFine)),
    FOREIGN KEY (libro) REFERENCES LibroFisico(id) ON DELETE CASCADE,
    FOREIGN KEY (utente) REFERENCES Utente(cf) ON DELETE CASCADE
);

-- Tabella Autore
CREATE TABLE IF NOT EXISTS Autore (
    id SERIAL PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    nascita DATE NOT NULL
);

-- Tabella Genere
CREATE TABLE IF NOT EXISTS Genere (
    nome StringS PRIMARY KEY
);

-- Tabella GenereEdizione
CREATE TABLE IF NOT EXISTS GenereEdizione (
    edizione VARCHAR(13) NOT NULL,
    genere StringS NOT NULL,
    PRIMARY KEY (edizione, genere),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE,
    FOREIGN KEY (genere) REFERENCES Genere(nome) ON DELETE CASCADE
);

-- Tabella EdizioneAutore
CREATE TABLE IF NOT EXISTS EdizioneAutore (
    edizione VARCHAR(13) NOT NULL,
    autore INTEGER NOT NULL,
    PRIMARY KEY (edizione, autore),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE,
    FOREIGN KEY (autore) REFERENCES Autore(id) ON DELETE CASCADE
);

-- Tabella Fornitore
CREATE TABLE IF NOT EXISTS Fornitore (
    nome StringS PRIMARY KEY,
    email Email NOT NULL
);

-- Tabella Restock
CREATE TABLE IF NOT EXISTS Restock (
    id SERIAL PRIMARY KEY,
    quantita IntGZ NOT NULL,
    istante TIMESTAMP,
    fornitore StringS NOT NULL,
    edizione VARCHAR(13) NOT NULL,
    FOREIGN KEY (fornitore) REFERENCES Fornitore(nome),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN)
);

-- Tabella Bibliotecario
CREATE TABLE IF NOT EXISTS Bibliotecario (
    cf CF PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email Email NOT NULL,
    dataAssunzione DATE NOT NULL,
    UNIQUE(email)
);

-- Tabella Sanzione
CREATE TABLE IF NOT EXISTS Sanzione (
    id SERIAL PRIMARY KEY,
    costo Denaro NOT NULL,
    stato StatoSanzione NOT NULL DEFAULT 'NON PAGATA', 
    motivazione StringL,
    dataSanzione DATE NOT NULL,
    bibliotecario CF NOT NULL,
    utente CF NOT NULL,
    FOREIGN KEY (bibliotecario) REFERENCES Bibliotecario(cf),
    FOREIGN KEY (utente) REFERENCES Utente(cf)
);

-- Tabella PendingRestock
CREATE TABLE IF NOT EXISTS PendingRestock (
    id SERIAL PRIMARY KEY,
    quantita IntGZ NOT NULL,
    bibliotecario CF NOT NULL,
    istante TIMESTAMP,
    fornitore StringS NOT NULL,
    edizione VARCHAR(13) NOT NULL,
    stato StatoRestock DEFAULT 'IN ATTESA',
    FOREIGN KEY (fornitore) REFERENCES Fornitore(nome),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN),
    FOREIGN KEY (bibliotecario) REFERENCES Bibliotecario(cf)
);