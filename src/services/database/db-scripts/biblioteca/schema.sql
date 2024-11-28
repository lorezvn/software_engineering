\c :dbname

-- Definizione dei domini
CREATE DOMAIN IntGZ AS INTEGER CHECK (VALUE > 0);
CREATE DOMAIN RealGEZ AS REAL CHECK (VALUE >= 0);
CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringM AS VARCHAR(100);
CREATE DOMAIN StringL AS VARCHAR(250);
CREATE DOMAIN Email AS StringM CHECK (VALUE ~* E'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$');
CREATE DOMAIN ISBN AS VARCHAR(13);

-- Definizione dei tipi
CREATE TYPE StatoRichiesta AS ENUM('IN ATTESA', 'COMPLETATA', 'RIFIUTATA');
CREATE TYPE StatoSanzione AS ENUM('PAGATA', 'NON PAGATA', 'REVOCATA');

-- Tabella Utente
CREATE TABLE IF NOT EXISTS Utente (
    id SERIAL PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email Email NOT NULL,
    username VARCHAR(16) NOT NULL,
    UNIQUE(email)
);

-- Tabella CasaEditrice
CREATE TABLE IF NOT EXISTS CasaEditrice (
    nome StringM PRIMARY KEY
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

-- Tabella LibroEdizione
CREATE TABLE IF NOT EXISTS LibroEdizione (
    ISBN ISBN PRIMARY KEY,
    titolo StringS NOT NULL,
    pagine IntGZ NOT NULL,
    casaEditrice StringM,
    autore INTEGER NOT NULL,
    genere StringS NOT NULL,
    FOREIGN KEY (casaEditrice) REFERENCES CasaEditrice(nome) ON DELETE SET NULL,
    FOREIGN KEY (autore) REFERENCES Autore(id) ON DELETE SET NULL,
    FOREIGN KEY (genere) REFERENCES Genere(nome) ON DELETE SET NULL
);

-- Tabella LibroFisico
CREATE TABLE IF NOT EXISTS LibroFisico (
    id SERIAL PRIMARY KEY,
    edizione ISBN NOT NULL,
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE
);

-- Tabella RichiestaPrestito
CREATE TABLE IF NOT EXISTS RichiestaPrestito (
    id SERIAL PRIMARY KEY,
    utente INTEGER NOT NULL,
    edizione ISBN NOT NULL,
    istante TIMESTAMP NOT NULL,
    stato StatoRichiesta DEFAULT 'IN ATTESA',
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE,
    FOREIGN KEY (utente) REFERENCES Utente(id) ON DELETE CASCADE
);

-- Tabella Prestito
CREATE TABLE IF NOT EXISTS Prestito (
    id SERIAL PRIMARY KEY,
    richiesta INTEGER NOT NULL,
    utente INTEGER NOT NULL,
    bibliotecario INTEGER NOT NULL,
    libro INTEGER NOT NULL,
    dataInizio DATE NOT NULL,
    dataFine DATE NOT NULL,
    dataRest DATE,
    isTerminato BOOLEAN DEFAULT FALSE,
    CHECK (dataInizio < dataFine),
    CHECK (
        (isTerminato = FALSE OR dataRest IS NOT NULL)
        AND (dataRest IS NULL OR isTerminato = TRUE)
    ),
    CHECK (isTerminato = FALSE OR (dataInizio <= dataRest)),
    FOREIGN KEY (richiesta) REFERENCES RichiestaPrestito(id) ON DELETE CASCADE,
    FOREIGN KEY (libro) REFERENCES LibroFisico(id) ON DELETE CASCADE,
    FOREIGN KEY (utente) REFERENCES Utente(id) ON DELETE CASCADE
);

-- Tabella Fornitore
CREATE TABLE IF NOT EXISTS Fornitore (
    nome StringS PRIMARY KEY,
    email Email NOT NULL
);

-- Tabella Bibliotecario
CREATE TABLE IF NOT EXISTS Bibliotecario (
    id SERIAL PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email Email NOT NULL,
    dataAssunzione DATE NOT NULL,
    UNIQUE(email)
);

-- Tabella Sanzione
CREATE TABLE IF NOT EXISTS Sanzione (
    id SERIAL PRIMARY KEY,
    costo RealGEZ NOT NULL,
    stato StatoSanzione NOT NULL DEFAULT 'NON PAGATA', 
    motivazione StringL,
    data DATE NOT NULL,
    bibliotecario INTEGER NOT NULL,
    utente INTEGER NOT NULL,
    FOREIGN KEY (bibliotecario) REFERENCES Bibliotecario(id),
    FOREIGN KEY (utente) REFERENCES Utente(id)
);

-- Tabella RichiestaRestock
CREATE TABLE IF NOT EXISTS RichiestaRestock (
    id SERIAL PRIMARY KEY,
    quantita IntGZ NOT NULL,
    bibliotecario INTEGER NOT NULL,
    fornitore StringS NOT NULL,
    istante TIMESTAMP NOT NULL,
    edizione ISBN NOT NULL,
    stato StatoRichiesta DEFAULT 'IN ATTESA',
    FOREIGN KEY (fornitore) REFERENCES Fornitore(nome),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) ON DELETE CASCADE,
    FOREIGN KEY (bibliotecario) REFERENCES Bibliotecario(id) ON DELETE CASCADE
);

-- Tabella Restock
CREATE TABLE IF NOT EXISTS Restock (
    id SERIAL PRIMARY KEY,
    richiesta INTEGER NOT NULL,
    quantita IntGZ NOT NULL,
    istante TIMESTAMP,
    fornitore StringS NOT NULL,
    edizione ISBN NOT NULL,
    FOREIGN KEY (richiesta) REFERENCES RichiestaRestock(id) ON DELETE CASCADE,
    FOREIGN KEY (fornitore) REFERENCES Fornitore(nome),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN) 
);