from params.email import EmailGenerator
from params.int_gz import IntGZGenerator
from params.real_gez import RealGEZGenerator
from params.string_L import StringLGenerator
from params.string_M import StringMGenerator
from params.string_S import StringSGenerator

requests = {
    "add-utente": [
        [("nome", StringSGenerator)],
        [("cognome", StringSGenerator)],
        [("email", EmailGenerator)]
    ]
}

api_methods = {
    "utente": ["add-utente"]
}