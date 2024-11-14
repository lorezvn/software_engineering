from params.email import EmailGenerator
from params.int_gz import IntGZGenerator
from params.real_gez import RealGEZGenerator
from params.string_L import StringLGenerator
from params.string_M import StringMGenerator
from params.string_S import StringSGenerator
from params.username import UsernameGenerator
from params.id import IdGenerator
from params.date import DateGenerator


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
    "update-username": [
        [("user_id", IdGenerator)],
        [("new_username", UsernameGenerator)]
    ]
}


api_methods = {
    "bibliotecario": ["add-bibliotecario"],
    "fornitore": ["add-fornitore"],
    "utente": ["add-utente", "update-username"]
}
