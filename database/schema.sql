-- Mancano vincoli: inclusione (trigger)

CREATE DOMAIN IntGZ AS INTEGER CHECK (VALUE > 0);
CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringM AS VARCHAR(100);

CREATE DOMAIN Via as StringS
	CHECK (VALUE IS NOT NULL);

CREATE DOMAIN Cap as char(5)
	CHECK (VALUE IS NOT NULL);

CREATE TYPE Indirizzo (
	via Via,
	civico IntGZ,
	cap Cap
);

CREATE TABLE IF NOT EXISTS Utente (
    cf VARCHAR(16) PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    email StringM NOT NULL,
    indirizzo Indirizzo NOT NULL,
    UNIQUE(email)
);

CREATE TABLE IF NOT EXISTS Prestito (
    id SERIAL PRIMARY KEY,
    inizio DATE NOT NULL,
    restituzione DATE,
    fine DATE NOT NULL,
    isTerminato BOOLEAN DEFAULT FALSE,
    libro INTEGER NOT NULL,
    utente VARCHAR(16) NOT NULL,
    CHECK (inizio < fine),
    -- is_terminato = true (A) <--> restituzione is not null (B)
    CHECK (
        -- A --> B = NOT A OR B
        (isTerminato = FALSE OR restituzione IS NOT NULL)
        AND
        -- B --> A = NOT B OR A
        (restituzione IS NULL OR isTerminato = TRUE)
    ),
    -- is_terminato = true --> inizio <= restituzione <= fine
    CHECK (isTerminato = FALSE OR ((inizio <= restituzione) AND (restituzione <= fine))),
    FOREIGN KEY (libro) REFERENCES LibroFisico(id),
    FOREIGN KEY (utente) REFERENCES Utente(cf)
);

CREATE TABLE IF NOT EXISTS LibroFisico (
    id SERIAL PRIMARY KEY,
    edizione VARCHAR(16) NOT NULL,
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN)
);

CREATE TABLE IF NOT EXISTS LibroEdizione (
    ISBN VARCHAR(13) PRIMARY KEY,
    titolo StringS NOT NULL,
    pagine IntGZ NOT NULL,
    casaEditrice StringM NOT NULL,
    FOREIGN KEY (casaEditrice) REFERENCES CasaEditrice(nome)
);

CREATE TABLE IF NOT EXISTS Autore (
    id SERIAL PRIMARY KEY,
    nome StringS NOT NULL,
    cognome StringS NOT NULL,
    nascita DATE NOT NULL
);

CREATE TABLE IF NOT EXISTS CasaEditrice (
    nome StringM PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS Genere (
    nome StringS PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS GenereEdizione (
    edizione VARCHAR(13) NOT NULL,
    genere StringS NOT NULL,
    PRIMARY KEY (edizione, genere),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN),
    FOREIGN KEY (genere) references Genere(nome)
);

CREATE TABLE IF NOT EXISTS EdizioneAutore (
    edizione VARCHAR(13) NOT NULL,
    autore INTEGER NOT NULL,
    PRIMARY KEY (edizione, autore),
    FOREIGN KEY (edizione) REFERENCES LibroEdizione(ISBN),
    FOREIGN KEY (autore) references Autore(id)
);