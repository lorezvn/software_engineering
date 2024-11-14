from .constants import RANDOM_SEED
from datetime import datetime, timedelta

import random

random.seed(RANDOM_SEED)


class DateGenerator:
    def __init__(self, start_date="2000-01-01", end_date="2024-12-31"):

        self.start_date = datetime.strptime(start_date, "%Y-%m-%d")
        self.end_date = datetime.strptime(end_date, "%Y-%m-%d")
        self.date_range = (self.end_date - self.start_date).days

    def generate(self):
        random_days = random.randint(0, self.date_range)
        random_date = self.start_date + timedelta(days=random_days)
        return random_date.strftime("%Y-%m-%d") 

d = DateGenerator()
print(d.generate())

