from .constants import RANDOM_SEED

import random
import string

random.seed(RANDOM_SEED)


class UsernameGenerator():
    def __init__(self):
        self.allowed_chars = string.ascii_letters + string.digits
        self.max_len = 16

    def generate(self):
        return "".join(random.choices(self.allowed_chars, k=random.randint(1, self.max_len)))