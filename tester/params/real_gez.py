from .constants import RANDOM_SEED

import random

random.seed(RANDOM_SEED)


class RealGEZGenerator():
    def __init__(self):
        pass

    def generate(self):
        return round(random.uniform(0, 1000), 2)