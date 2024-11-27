from .constants import RANDOM_SEED

import random
import string

random.seed(RANDOM_SEED)


class ISBNGenerator():
    def __init__(self):
        self.prefissi = ["978", "979"]
        self.list = ['9781234567897', '9782345678901', '9784515712938', '9774515712039']


    def generate(self):
        prefisso = random.choice(self.prefissi)
        sequenza = "".join(random.choices(string.digits, k=10))
        return random.choice(self.list)
        return f"{prefisso}{sequenza}"