import time


class Timer:
    def __init__(self):
        self.start = time.time()
        self.stop = 0

    def __del__(self):
        self.stop = time.time()
        print("Elapsed time: " + str(self.stop - self.start))
