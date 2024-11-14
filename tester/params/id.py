from .constants import RANDOM_SEED

import random

random.seed(RANDOM_SEED)


class IdGenerator():
    def __init__(self):
        pass

    def generate(self):
        return random.randint(1, 10)