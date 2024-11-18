from params.email import EmailGenerator
from params.int_gz import IntGZGenerator
from params.real_gez import RealGEZGenerator
from params.string_L import StringLGenerator
from params.string_M import StringMGenerator
from params.string_S import StringSGenerator
from params.username import UsernameGenerator
from params.id import IdGenerator
from params.date import DateGenerator
from params.isbn import ISBNGenerator


requests = {
    "add-bibliotecario": [
        [("nome", StringSGenerator)],
        [("cognome", StringSGenerator)],
        [("email", EmailGenerator)],
        [("data_ass", DateGenerator)]
    ],
    "add-fornitore": [
        [("nome", StringSGenerator)],
        [("email", EmailGenerator)]
    ],
    "add-utente": [
        [("nome", StringSGenerator)],
        [("cognome", StringSGenerator)],
        [("email", EmailGenerator)],
        [("username", UsernameGenerator)]
    ], 
    "add-richiesta-prestito": [
        [("utente_id", IdGenerator)],
        [("libro_id", IdGenerator)],
        [(("data_inizio", "data_fine"), DateGenerator)]
    ],
    "ricerca-libri": [
        [("titolo", StringSGenerator)]
    ],
    "update-username": [
        [("user_id", IdGenerator)],
        [("new_username", UsernameGenerator)]
    ], 
    "visualizza-sanzioni": [
        [("utente_id", IdGenerator)]
    ],
    "add-sanzione": [
        [("costo", RealGEZGenerator)],
        [("motivazione", StringLGenerator)],
        [("data", DateGenerator)],
        [("bibl_id", IdGenerator)], 
        [("utente_id", IdGenerator)]
    ], 
    "estingui-sanzione": [
        [("sanzione_id", IdGenerator)]
    ], 
    "revoca-sanzione": [
        [("sanzione_id", IdGenerator)]
    ]
}


api_methods = {
    "bibliotecario": ["add-bibliotecario", "add-sanzione", "estingui-sanzione", "revoca-sanzione"],
    "fornitore": ["add-fornitore"],
    "utente": ["add-utente", "add-richiesta-prestito", "ricerca-libri", "update-username", "visualizza-sanzioni"]
}
