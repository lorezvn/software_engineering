from .constants import RANDOM_SEED

import random
import string

random.seed(RANDOM_SEED)


class TitoloGenerator():
    def __init__(self):
        self.titoli = ["The Hunger Games", "Don Quixote", "The Grass is Always Greener", 
                       "The Higgler", "The Open Boat", "The Speckled Band", "The Signalman", 
                       "The Zombie Room", "The Adventures of Huckleberry Finn", 
                       "Dracula", "The Three Musketeers", "The Secret Garden", "Animal Farm"]

    def generate(self):
        return "Harry"
        #return random.choice(self.titoli)