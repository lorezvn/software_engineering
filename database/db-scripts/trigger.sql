\c :dbname

-- 1. Trigger sulla tabella Prestito: Imposta automaticamente la data di restituzione quando il prestito è terminato
CREATE OR REPLACE FUNCTION set_restituzione_date() RETURNS TRIGGER AS $$
BEGIN
    -- Se il prestito è terminato e la restituzione non è ancora impostata, imposta la data corrente
    IF NEW.isTerminato = TRUE AND NEW.dataRest IS NULL THEN
        NEW.dataRest = CURRENT_DATE;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_update_prestito_restituzione
BEFORE UPDATE ON Prestito
FOR EACH ROW
WHEN (NEW.isTerminato = TRUE)
EXECUTE FUNCTION set_restituzione_date();


-- 2. Trigger sulla tabella LibroFisico: Impedisci la cancellazione di un libro se è in prestito
CREATE OR REPLACE FUNCTION prevent_delete_if_in_prestito() RETURNS TRIGGER AS $$
BEGIN
    -- Impedisci la cancellazione del libro se è attualmente in prestito
    IF EXISTS (SELECT 1 FROM Prestito WHERE libro = OLD.id AND isTerminato = FALSE) THEN
        RAISE EXCEPTION 'Impossibile cancellare il libro, è ancora in prestito';
    END IF;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_delete_librofisico
BEFORE DELETE ON LibroFisico
FOR EACH ROW
EXECUTE FUNCTION prevent_delete_if_in_prestito();


-- 3. Trigger sulla tabella LibroEdizione: Impedisci la cancellazione di un' edizione se ci sono libri associati in prestiti attivi
CREATE OR REPLACE FUNCTION prevent_delete_if_edizione_in_prestito() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (SELECT 1 FROM Prestito WHERE libro IN (SELECT id FROM LibroFisico WHERE edizione = OLD.ISBN) AND isTerminato = FALSE) THEN
        RAISE EXCEPTION 'Impossibile cancellare l''edizione, ha prestiti attivi';
    END IF;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_delete_libroedizione
BEFORE DELETE ON LibroEdizione
FOR EACH ROW
EXECUTE FUNCTION prevent_delete_if_edizione_in_prestito();


-- 4. Trigger sulla tabella Prestito: Impedisci la restituzione di un libro che non è stato preso in prestito dall'utente
CREATE OR REPLACE FUNCTION check_prestito_validity() RETURNS TRIGGER AS $$
BEGIN
    -- Verifica che l'utente stia restituendo un libro che ha effettivamente preso in prestito
    IF NOT EXISTS (
        SELECT 1
        FROM Prestito
        WHERE NEW.utente = Prestito.utente
        AND NEW.libro = Prestito.libro
        AND Prestito.isTerminato = FALSE
    ) THEN
        RAISE EXCEPTION 'L''utente non ha preso in prestito questo libro';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_prestito_validity_trg
BEFORE UPDATE ON Prestito
FOR EACH ROW
WHEN (NEW.dataRest IS NOT NULL)
EXECUTE FUNCTION check_prestito_validity();


-- 5. Trigger sulla tabella Utente: Impedisci la cancellazione di un utente se ha prestiti attivi
CREATE OR REPLACE FUNCTION prevent_delete_if_user_has_prestiti() RETURNS TRIGGER AS $$
BEGIN
    -- Impedisci la cancellazione dell'utente se ha prestiti attivi
    IF EXISTS (SELECT 1 FROM Prestito WHERE utente = OLD.cf AND isTerminato = FALSE) THEN
        RAISE EXCEPTION 'Impossibile cancellare l''utente, ha ancora prestiti attivi';
    END IF;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_delete_utente
BEFORE DELETE ON Utente
FOR EACH ROW
EXECUTE FUNCTION prevent_delete_if_user_has_prestiti();




