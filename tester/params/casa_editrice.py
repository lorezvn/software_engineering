from .constants import RANDOM_SEED

import random

random.seed(RANDOM_SEED)


class CasaEditriceGenerator():
    def __init__(self):
        self.case = ['Bompiani', 'Salani', 'Sgruda']

    def generate(self):
        return random.choice(self.case)
