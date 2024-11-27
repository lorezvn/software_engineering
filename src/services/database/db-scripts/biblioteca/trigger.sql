\c :dbname

-- 1. Trigger sulla tabella LibroFisico: Impedisci la cancellazione di un libro se è in prestito
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


-- 2. Trigger sulla tabella LibroEdizione: Impedisci la cancellazione di un' edizione se ci sono libri associati in prestiti attivi
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


-- 3. Trigger sulla tabella Prestito: Impedisci la restituzione di un libro che non è stato preso in prestito dall'utente
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


-- 4. Trigger sulla tabella Utente: Impedisci la cancellazione di un utente se ha prestiti attivi
CREATE OR REPLACE FUNCTION prevent_delete_if_user_has_prestiti() RETURNS TRIGGER AS $$
BEGIN
    -- Impedisci la cancellazione dell'utente se ha prestiti attivi
    IF EXISTS (SELECT 1 FROM Prestito WHERE utente = OLD.id AND isTerminato = FALSE) THEN
        RAISE EXCEPTION 'Impossibile cancellare l''utente, ha ancora prestiti attivi';
    END IF;
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_delete_utente
BEFORE DELETE ON Utente
FOR EACH ROW
EXECUTE FUNCTION prevent_delete_if_user_has_prestiti();


-- 5. Trigger sulla tabella Prestito: Completa la richiesta associata al prestito inserito
CREATE OR REPLACE FUNCTION update_richiesta_prestito_status()
RETURNS TRIGGER AS $$
BEGIN
    -- Aggiorna lo stato della richiesta in 'COMPLETATA' quando viene inserito un nuovo prestito
    UPDATE RichiestaPrestito
    SET stato = 'COMPLETATA'
    WHERE id = NEW.richiesta;  
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER after_prestito_insert
AFTER INSERT ON Prestito
FOR EACH ROW
EXECUTE FUNCTION update_richiesta_prestito_status();


-- 6. Trigger sulla tabella Restock: Completa la richiesta associata al restock inserito
CREATE OR REPLACE FUNCTION update_richiesta_restock_status()
RETURNS TRIGGER AS $$
BEGIN
    -- Aggiorna lo stato della richiesta in 'COMPLETATA' quando viene inserito un nuovo restock
    UPDATE RichiestaRestock
    SET stato = 'COMPLETATA'
    WHERE id = NEW.richiesta;  
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER after_restock_insert
AFTER INSERT ON Restock
FOR EACH ROW
EXECUTE FUNCTION update_richiesta_restock_status();


-- 7. Trigger sulla tabella Prestito: Inserimento solo se la richiesta associata è "IN ATTESA"
CREATE OR REPLACE FUNCTION check_richiesta_prestito_in_attesa()
RETURNS TRIGGER AS $$
BEGIN
    -- Controlla se la richiesta associata è in stato "IN ATTESA"
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaPrestito
        WHERE id = NEW.richiesta AND stato = 'IN ATTESA'
    ) THEN
        RAISE EXCEPTION 'Non è possibile creare un prestito per una richiesta non in stato "IN ATTESA".';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_prestito_insert
BEFORE INSERT ON Prestito
FOR EACH ROW
EXECUTE FUNCTION check_richiesta_prestito_in_attesa();


-- 8. Trigger sulla tabella Restock: Inserimento solo se la richiesta associata è "IN ATTESA"
CREATE OR REPLACE FUNCTION check_richiesta_restock_in_attesa()
RETURNS TRIGGER AS $$
BEGIN
    -- Controlla se la richiesta associata è in stato "IN ATTESA"
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaRestock
        WHERE id = NEW.richiesta AND stato = 'IN ATTESA'
    ) THEN
        RAISE EXCEPTION 'Non è possibile creare un restock per una richiesta non in stato "IN ATTESA".';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER before_restock_insert
BEFORE INSERT ON Restock
FOR EACH ROW
EXECUTE FUNCTION check_richiesta_restock_in_attesa();


-- 9. Trigger sulla tabella Prestito: Verifica che i dati del prestito corrispondano a quelli della richiesta
CREATE OR REPLACE FUNCTION verifica_coerenza_prestito()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaPrestito AS rp
        JOIN LibroFisico AS lf ON rp.edizione = lf.edizione
        WHERE rp.id = NEW.richiesta
          AND rp.utente = NEW.utente
          AND lf.id = NEW.libro
    ) THEN
        RAISE EXCEPTION 'I dati del prestito non corrispondono alla richiesta associata.';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_verifica_prestito
BEFORE INSERT OR UPDATE ON Prestito
FOR EACH ROW
EXECUTE FUNCTION verifica_coerenza_prestito();


-- 10. Trigger sulla tabella Prestito: Verifica che l'istante della richiesta sia minore della dataInizio del prestito
CREATE OR REPLACE FUNCTION verifica_istante_richiesta_prestito()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaPrestito
        WHERE id = NEW.richiesta
          AND istante::DATE <= NEW.dataInizio
    ) THEN
        RAISE EXCEPTION 'La data di inizio del prestito deve essere successiva all''istante della richiesta.';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_verifica_istante_richiesta_prestito
BEFORE INSERT OR UPDATE ON Prestito
FOR EACH ROW
EXECUTE FUNCTION verifica_istante_richiesta_prestito();


-- 11. Trigger sulla tabella Restock: Verifica che i dati del restock corrispondano a quelli della richiesta
CREATE OR REPLACE FUNCTION verifica_coerenza_restock()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaRestock
        WHERE id = NEW.richiesta
          AND quantita = NEW.quantita
          AND fornitore = NEW.fornitore
          AND edizione = NEW.edizione
    ) THEN
        RAISE EXCEPTION 'I dati del restock non corrispondono alla richiesta associata.';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_verifica_restock
BEFORE INSERT OR UPDATE ON Restock
FOR EACH ROW
EXECUTE FUNCTION verifica_coerenza_restock();


-- 12. Trigger sulla tabella Restock: Verifica che l'istante della richiesta sia minore dell'istante del restock
CREATE OR REPLACE FUNCTION verifica_istante_richiesta_restock()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM RichiestaRestock
        WHERE id = NEW.richiesta
          AND istante < NEW.istante
    ) THEN
        RAISE EXCEPTION 'L''istante del restock deve essere successivo all''istante della richiesta.';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_verifica_istante_richiesta_restock
BEFORE INSERT OR UPDATE ON Restock
FOR EACH ROW
EXECUTE FUNCTION verifica_istante_richiesta_restock();


-- 13. Trigger sulla tabella Prestito: Controlla se esiste un altro prestito attivo per lo stesso libro nello stesso periodo
CREATE OR REPLACE FUNCTION verifica_prestito_unico()
RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM Prestito
        WHERE libro = NEW.libro
          AND isTerminato = FALSE
          AND (
              (NEW.dataInizio, NEW.dataFine) OVERLAPS (dataInizio, dataFine)
          )
    ) THEN
        RAISE EXCEPTION 'Il libro è già in prestito in questo periodo.';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_verifica_prestito_unico
BEFORE INSERT OR UPDATE ON Prestito
FOR EACH ROW
EXECUTE FUNCTION verifica_prestito_unico();


-- 14. Trigger sulla tabella Prestito: Trova un libro fisico disponibile per l'edizione richiesta
CREATE OR REPLACE FUNCTION assegna_libro_disponibile()
RETURNS TRIGGER AS $$
DECLARE
    libro_disponibile INTEGER;
BEGIN

    SELECT lf.id
    INTO libro_disponibile
    FROM LibroFisico lf
    LEFT JOIN Prestito p ON lf.id = p.libro AND p.isTerminato = FALSE
    WHERE lf.edizione = (SELECT edizione FROM RichiestaPrestito WHERE id = NEW.richiesta)
    AND p.id IS NULL
    LIMIT 1;

    -- Se non ci sono libri disponibili, solleva un'eccezione
    IF libro_disponibile IS NULL THEN
        RAISE EXCEPTION 'Non ci sono copie disponibili per questa edizione.';
    END IF;

    -- Assegna il libro trovato al prestito
    NEW.libro = libro_disponibile;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_assegna_libro
BEFORE INSERT ON Prestito
FOR EACH ROW
EXECUTE FUNCTION assegna_libro_disponibile();



