\c :dbname

-- Inserisci alcuni utenti 
INSERT INTO Utente (nome, cognome, email, username)
VALUES
    ( 'Mario', 'Rossi', 'mario.rossi@gmail.com', 'marionz'),
    ( 'Lucia', 'Bianchi', 'lucia.bianchi@gmail.com', 'l.bianchi'),
    ( 'Franco', 'Neri', 'franco.neri@gmail.com', 'f_neri');

-- Inserisci alcuni bibliotecari 
INSERT INTO Bibliotecario (nome, cognome, email, dataAssunzione)
VALUES
    ( 'Giovanni', 'Verdi', 'giovanni.verdi@gmail.com', '2023-01-10'),
    ( 'Francesca', 'Blu', 'francesca.blu@gmail.com', '2022-06-15'), 
    ( 'Sara', 'Azzurri', 'sara.azzurri@gmail.com', '2020-09-13');

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
    ('9784515712938', 'Il Pianeta di CiccioGamer e Jokerinho', 789000, 'Sgruda'),
    ('9774515712039', 'Il Mondo di Sgrognolox', 300, 'Sgruda');

-- Inserisci alcuni libri fisici
INSERT INTO LibroFisico (edizione)
VALUES
    ('9781234567897'),
    ('9782345678901'),
    ('9784515712938'),
    ('9784515712938'), 
    ('9774515712039');
    

-- Inserimento di richieste di rifornimento 
INSERT INTO RichiestaRestock (quantita, bibliotecario, istante, fornitore, edizione, stato)
VALUES
    (1, 1, '2024-09-13 10:30:00', 'Distribuzione Libri S.p.A.', '9781234567897', 'IN ATTESA'),
    (1, 2, '2024-09-05 14:45:00', 'Libro Express SRL', '9782345678901', 'IN ATTESA'),
    (2, 3, '2024-09-05 14:45:00', 'Libro Express SRL', '9784515712938', 'IN ATTESA'),
    (12, 3, '2024-10-07 17:50:00', 'Distribuzione Libri S.p.A.', '9784515712938', 'IN ATTESA'),
    (15, 1, '2024-11-10 10:32:50', 'Libro Express SRL', '9774515712039', 'IN ATTESA');

-- Inserimento di rifornimenti completati
INSERT INTO Restock (richiesta, quantita, istante, fornitore, edizione)
VALUES
    (1, 1, '2024-09-15 09:00:00', 'Distribuzione Libri S.p.A.', '9781234567897'),
    (2, 1, '2024-09-07 11:20:00', 'Libro Express SRL', '9782345678901'),
    (3, 2, '2024-10-05 15:00:00', 'Libro Express SRL', '9784515712938');

-- Inserimento di richieste di prestito
INSERT INTO RichiestaPrestito (istante, stato, edizione, utente)
VALUES
    ('2024-11-10 09:50:00', 'IN ATTESA', '9781234567897', 1),
    ('2024-11-06 10:30:30', 'IN ATTESA', '9782345678901', 2),
    ('2024-11-06 10:30:30', 'IN ATTESA', '9774515712039', 1);

-- Inserisci prestiti di esempio
INSERT INTO Prestito (richiesta, dataInizio, dataFine, utente, bibliotecario)
VALUES
    (1, '2024-11-10', '2024-12-10', 1, 1),
    (2, '2024-11-10', '2024-12-10', 2, 1),
    (3, '2024-11-10', '2024-12-10', 1, 1);

-- Inserimento di sanzioni
INSERT INTO Sanzione (costo, stato, motivazione, data, bibliotecario, utente)
VALUES
    (15.0, 'NON PAGATA', 'Ritardo nella restituzione del libro', '2024-10-15', 2, 1),
    (100.20, 'NON PAGATA', 'Libro perso', '2024-10-15', 2, 1),
    (180.50, 'NON PAGATA', 'Libro bruciato', '2024-10-15', 2, 1),
    (30.0, 'NON PAGATA', 'Ritardo nella restituzione del libro', '2023-11-24', 3, 2);