from .constants import RANDOM_SEED
from .constants import PUNCTUATION

import random
import string

random.seed(RANDOM_SEED)


class StringLGenerator():
    def __init__(self):
        self.allowed_chars = string.ascii_letters + string.digits + " "
        self.max_len = 250

    def generate(self):
        return "".join(random.choices(self.allowed_chars, k=random.randint(1, self.max_len)))