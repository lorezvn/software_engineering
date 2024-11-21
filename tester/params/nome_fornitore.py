from .constants import RANDOM_SEED

import random

random.seed(RANDOM_SEED)


class NomeFornitoreGenerator():
    def __init__(self):
        self.nomi = ['Distribuzione Libri S.p.A.', 'Libro Express SRL', 'Laterza', 
                    'De Agostini', 'Adelphi Edizioni', 'Pontedera Libri']

    def generate(self):
        return 'Distribuzione Libri S.p.A.'
        return random.choice(self.nomi)