#!/bin/sh

# Definisce i parametri
    # parameters.sql 

# Setup per il db e gli utenti
    # biblioteca/setup.sql
    # logdb/setup-log.sql

# Imposta i trigger
    # biblioteca/trigger.sql
    # logdb/trigger-log.sql

# Crea lo schema del db
    # biblioteca/schema.sql
    # logdb/schema-log.sql

# Privilegi
    # biblioteca/grant.sql
    # logdb/grant-log.sql

# Popola il db
    # biblioteca/data.sql


SQL_COMMAND="sudo -u postgres psql postgres"


SQL_FILES="parameters.sql \
           biblioteca/setup.sql \
           biblioteca/schema.sql \
           biblioteca/grant.sql \
           biblioteca/trigger.sql \
           biblioteca/data.sql \
           logdb/setup-log.sql \
           logdb/schema-log.sql \
           logdb/grant-log.sql \
           logdb/trigger-log.sql"


for file in $SQL_FILES; do
    SQL_COMMAND="$SQL_COMMAND -f $file"
done

eval $SQL_COMMAND

echo "Database creato con successo."
