from .constants import RANDOM_SEED

import random
import string
import re

random.seed(RANDOM_SEED)


class EmailGenerator():
    def __init__(self):
        self.allowed_chars = string.ascii_letters + string.digits
        self.regex = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
        self.domain_list = ['gmail.com', 'yahoo.com', 'outlook.com', 'hotmail.com', 'example.com']

    def generate(self):

        while True:
            username = "".join(random.choices(self.allowed_chars, k=random.randint(5, 10)))
            domain = random.choice(self.domain_list)
            email = f'{username}@{domain}'

            if re.match(self.regex, email):
                return email
