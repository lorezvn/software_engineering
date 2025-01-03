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
from params.genere import GenereGenerator
from params.casa_editrice import CasaEditriceGenerator
from params.nome_fornitore import NomeFornitoreGenerator


requests = {
    "add-bibliotecario": [
        [("nome", StringSGenerator)],
        [("cognome", StringSGenerator)],
        [("email", EmailGenerator)],
        [("data_ass", DateGenerator)]
    ],
    "add-fornitore": [
        [("nome", NomeFornitoreGenerator)],
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
        [("edizione", ISBNGenerator)],
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
    ], 
    "add-prestito": [
        [("richiesta_id", IdGenerator)], 
        [("bibl_id", IdGenerator)]
    ],
    "visualizza-prestiti": [
        [("utente_id", IdGenerator)]
    ], 
    "add-richiesta-restock": [
        [("bibl_id", IdGenerator)],
        [("forn_nome", NomeFornitoreGenerator)],
        [("quantita", IntGZGenerator)],
        [("edizione", ISBNGenerator)]
    ], 
    "visualizza-richieste-prestito": [],
    "visualizza-richieste-restock": [
        [("forn_nome", NomeFornitoreGenerator)]
    ], 
    "add-restock": [
        [("richiesta_id", IdGenerator)]
    ],
    "termina-prestito":[
        [("prestito_id", IdGenerator)]
    ], 
    "rifiuta-richiesta-prestito": [
        [("richiesta_id", IdGenerator)]
    ],
    "rifiuta-richiesta-restock": [
        [("richiesta_id", IdGenerator)]
    ],
    "visualizza-copie": [],
    "visualizza-restock": [],
    "add-edizione": [
        [("ISBN", ISBNGenerator)],
        [("titolo", StringSGenerator)],
        [("pagine", IntGZGenerator)],
        [("casa_editrice", CasaEditriceGenerator)],
        [("autore_id", IdGenerator)],
        [("genere", GenereGenerator)],
    ], 
    "delete-copia": [
        [("libro_id", IdGenerator)]
    ]
}


api_methods = {
    "utente": ["add-utente", "add-richiesta-prestito", "ricerca-libri", 
               "update-username", "visualizza-sanzioni", "visualizza-prestiti"],

    "fornitore": ["add-fornitore", "add-restock", "visualizza-richieste-restock",
                  "rifiuta-richiesta-restock"],

    "bibliotecario": ["add-bibliotecario", "add-sanzione", "add-richiesta-restock", 
                      "add-edizione", "add-prestito", "revoca-sanzione", "estingui-sanzione", 
                      "visualizza-richieste-prestito", "termina-prestito", 
                      "rifiuta-richiesta-prestito", "visualizza-copie", 
                      "visualizza-restock", "delete-copia"]
}

ports = {"bibliotecario": 42069, "fornitore": 42070, "utente": 42071}
