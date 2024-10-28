#!/bin/sh

# Definisce i parametri
# parameters.sql 

# Crea il db e gli utenti
# create-db-users.sql

# Imposta i trigger
# trigger.sql

# Crea lo schema del db
# schema.sql

# Privilegi
# grant.sql

# Popola il db
# data.sql

sudo -u postgres psql postgres -f parameters.sql -f create-db-users.sql -f schema.sql -f grant.sql -f trigger.sql -f data.sql

echo "Database creato con successo."
