from .constants import RANDOM_SEED

import random
import string

random.seed(RANDOM_SEED)


class ISBNGenerator():
    def __init__(self):
        self.prefissi = ["978", "979"]

    def generate(self):
        prefisso = random.choice(self.prefissi)
        sequenza = "".join(random.choices(string.digits, k=10))
        return f"{prefisso}{sequenza}"