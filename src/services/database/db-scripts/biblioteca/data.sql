\c :dbname

-- Inserisci alcuni utenti di esempio
INSERT INTO Utente (cf, nome, cognome, email)
VALUES
    ('RSSMRA85M01H501Z', 'Mario', 'Rossi', 'mario.rossi@gmail.com'),
    ('BNCLRD70M01H501Z', 'Lucia', 'Bianchi', 'lucia.bianchi@gmail.com');

-- Inserisci alcune case editrici
INSERT INTO CasaEditrice (nome)
VALUES
    ('Bompiani'),
    ('Salani'),
    ('Sgruda');

-- Inserisci alcune edizioni di libri
INSERT INTO LibroEdizione (ISBN, titolo, pagine, casaEditrice)
VALUES
    ('9781234567897', 'Il Signore degli Anelli', 1000, 'Bompiani'),
    ('9782345678901', 'Harry Potter e la Pietra Filosofale', 340, 'Salani'),
    ('9784515712938', 'Il Pianeta di CiccioGamer e Jokerinho',789000, 'Sgruda'),
    ('9774515712039', 'Il Mondo di Sgrognolox', 300, 'Sgruda');

-- Inserisci alcuni libri fisici
INSERT INTO LibroFisico (edizione)
VALUES
    ('9781234567897'),
    ('9782345678901');

-- Inserisci prestiti di esempio
INSERT INTO Prestito (dataInizio, dataFine, libro, utente)
VALUES
    ('2024-01-01', '2024-01-31', 1, 'RSSMRA85M01H501Z'),
    ('2024-02-01', '2024-02-28', 2, 'BNCLRD70M01H501Z');
INSERT INTO Prestito (dataInizio, dataFine, libro, utente)
VALUES
    ('2024-01-01', '2024-01-31', 1, 'RSSMRA85M01H501Z'),
    ('2024-02-01', '2024-02-28', 2, 'BNCLRD70M01H501Z');
