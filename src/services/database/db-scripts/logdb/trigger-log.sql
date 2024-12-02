-- 1. Trigger sulla tabella Client: Impedisce che un file descriptor venga riutilizzato mentre è ancora associato a una connessione attiva.
CREATE OR REPLACE FUNCTION check_connessione_chiusa() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM Client client
        WHERE NEW.serverName = client.serverName
        AND NEW.fileDescriptor = client.fileDescriptor
        AND (client.disconnTime IS NULL OR NEW.connTime <= client.disconnTime)
    ) IS TRUE THEN
        RAISE EXCEPTION 'Connessione non chiusa';
END IF;

RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_check_connessione_chiusa
BEFORE INSERT ON Client 
FOR EACH ROW EXECUTE FUNCTION check_connessione_chiusa();