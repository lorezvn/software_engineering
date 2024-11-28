from .constants import RANDOM_SEED

import random

random.seed(RANDOM_SEED)

class GenereGenerator():
    def __init__(self):
        self.generi = ['Fantasy', 'Fantascienza', 'Distopico', 'Classico', 
                       'Horror', 'Umoristico', 'Avventura', 'Thriller']

    def generate(self):
        return random.choice(self.generi)
