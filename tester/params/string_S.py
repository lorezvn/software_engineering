from .constants import RANDOM_SEED
from .constants import PUNCTUATION

import random
import string

random.seed(RANDOM_SEED)


class StringSGenerator():
    def __init__(self):
        self.allowed_chars = string.ascii_letters + string.digits + PUNCTUATION
        self.max_len = 50

    def generate(self):
        return "".join(random.choices(self.allowed_chars, k=random.randint(1, self.max_len)))