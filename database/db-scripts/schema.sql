-- Definizione dei domini
CREATE DOMAIN IntGZ AS INTEGER CHECK (VALUE > 0);
CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringM AS VARCHAR(100);
CREATE DOMAIN Via AS StringS CHECK (VALUE IS NOT NULL);
CREATE DOMAIN Cap AS char(5) CHECK (VALUE IS NOT NULL);

CREATE TYPE Indirizzo AS (
    via Via,
    civico IntGZ,
    cap Cap
);

-- Tabella Utente
CREATE TABLE IF NOT EXISTS Utente (
    cf VARCHAR(16) PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email StringM NOT NULL,
    indirizzo Indirizzo NOT NULL,
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
    utente VARCHAR(16) NOT NULL,
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

