\c :dbname

-- Inserisci alcuni utenti 
INSERT INTO Utente (nome, cognome, email, username)
VALUES
    ( 'Mario', 'Rossi', 'mario.rossi@gmail.com', 'marionz'),
    ( 'Lucia', 'Bianchi', 'lucia.bianchi@gmail.com', 'l.bianchi');

-- Inserisci alcuni bibliotecari 
INSERT INTO Bibliotecario (nome, cognome, email, dataAssunzione)
VALUES
    ( 'Giovanni', 'Verdi', 'giovanni.verdi@gmail.com', '2023-01-10'),
    ( 'Francesca', 'Blu', 'francesca.blu@gmail.com', '2022-06-15');

-- Inserisci alcuni fornitori
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
    ('9782345678901'),
    ('9784515712938');

-- Inserimento di richieste di rifornimento 
INSERT INTO RichiestaRestock (quantita, bibliotecario, istante, fornitore, edizione, stato)
VALUES
    (5, 1, '2024-09-13 10:30:00', 'Distribuzione Libri S.p.A.', '9781234567897', 'IN ATTESA'),
    (10, 2, '2024-09-05 14:45:00', 'Libro Express SRL', '9782345678901', 'IN ATTESA');

-- Inserimento di rifornimenti completati
INSERT INTO Restock (richiesta, quantita, istante, fornitore, edizione)
VALUES
    (1, 5, '2024-09-15 09:00:00', 'Distribuzione Libri S.p.A.', '9781234567897'),
    (2, 10, '2024-09-07 11:20:00', 'Libro Express SRL', '9782345678901');

-- Inserimento di richieste di prestito
INSERT INTO RichiestaPrestito (dataInizio, dataFine, istante, stato, libro, utente)
VALUES
    ('2024-11-10', '2024-12-10', '2024-11-05 09:50:00', 'IN ATTESA', 1, 1),
    ('2024-11-15', '2024-12-15', '2024-11-06 10:30:30', 'IN ATTESA', 2, 2),
    ('2024-11-20', '2024-12-20', '2024-11-06 10:30:30', 'IN ATTESA', 3, 1);

-- Inserisci prestiti di esempio
INSERT INTO Prestito (richiesta, dataInizio, dataFine, libro, utente)
VALUES
    (1, '2024-01-01', '2024-01-31', 1, 1),
    (2, '2024-02-01', '2024-02-28', 2, 2);

-- Inserimento di sanzioni
INSERT INTO Sanzione (costo, stato, motivazione, data, bibliotecario, utente)
VALUES
    (15.0, 'NON PAGATA', 'Ritardo nella restituzione del libro', '2024-10-15', 2, 1);