from .constants import RANDOM_SEED

import random
import string

random.seed(RANDOM_SEED)


class StringLGenerator():
    def __init__(self):
        self.allowed_chars = string.ascii_letters + string.digits + string.punctuation
        self.max_len = 500

    def generate(self):
        return "".join(random.choices(self.allowed_chars, k=random.randint(1, self.max_len)))