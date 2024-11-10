\c :dbname

-- Inserisci alcuni utenti di esempio
INSERT INTO Utente (cf, nome, cognome, email)
VALUES
    ('RSSMRA85M01H501Z', 'Mario', 'Rossi', 'mario.rossi@gmail.com'),
    ('BNCLRD70M01H501Z', 'Lucia', 'Bianchi', 'lucia.bianchi@gmail.com');
INSERT INTO Bibliotecario (cf, nome, cognome, email, dataAssunzione)
VALUES
    ('VRDLGI70M01H501Z', 'Giovanni', 'Verdi', 'giovanni.verdi@gmail.com', '2023-01-10'),
    ('BLUFBA60M01H501Z', 'Francesca', 'Blu', 'francesca.blu@gmail.com', '2022-06-15');
INSERT INTO Fornitore (nome, email)
VALUES
    ('Distribuzione Libri S.p.A.', 'info@distribuzionelibri.com'),
    ('Libro Express SRL', 'contatti@libroexpress.com');
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
-- Inserimento di richieste di rifornimento pendenti
INSERT INTO PendingRestock (quantita, bibliotecario, istante, fornitore, edizione, stato)
VALUES
    (5, 'VRDLGI70M01H501Z', '2024-09-13 10:30:00', 'Distribuzione Libri S.p.A.', '9781234567897', 'IN ATTESA'),
    (10, 'BLUFBA60M01H501Z', '2024-09-05 14:45:00', 'Libro Express SRL', '9782345678901', 'IN ATTESA');
INSERT INTO PendingPrestito (dataInizio, dataFine, dataRichiesta, stato, libro, utente)
VALUES
    ('2024-11-10', '2024-12-10', '2024-11-05', 'IN ATTESA', 1, 'RSSMRA85M01H501Z'),
    ('2024-11-15', '2024-12-15', '2024-11-06', 'IN ATTESA', 2, 'BNCLRD70M01H501Z');
-- Inserimento di rifornimenti completati
INSERT INTO Restock (quantita, istante, fornitore, edizione)
VALUES
    (5, '2024-09-15 09:00:00', 'Distribuzione Libri S.p.A.', '9781234567897'),
    (10, '2024-09-07 11:20:00', 'Libro Express SRL', '9782345678901');

-- Inserimento di sanzioni
INSERT INTO Sanzione (costo, stato, motivazione, dataSanzione, bibliotecario, utente)
VALUES
    (ROW(15.0, 'EUR')::Denaro, 'NON PAGATA', 'Ritardo nella restituzione del libro', '2024-10-15', 'VRDLGI70M01H501Z', 'RSSMRA85M01H501Z');