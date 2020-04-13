import numpy
from math import cos, pi, sqrt, exp
POPULATION = {'names': ('a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c'),
              'formats': ('float64','float64','float64','float64','float64','float64')}

class Population:
    def __init__(self, size, file_data):
        self.parent_size = size
        self.offspring_size = size * 6
        self.file_data = file_data
        self.population_vector = numpy.empty(self.parent_size, dtype=POPULATION)
        self.offspring_population = numpy.empty([self.offspring_size],dtype=POPULATION)
        self.aggregated_function_values = numpy.empty((self.parent_size, len(self.file_data)))
        self.error_list = numpy.empty([len(self.aggregated_function_values), 3])

    def create_new_population(self, min_value, max_value, std_dev):
        col_names = ['a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c']
        for i in range(len(col_names[0:3])):
            self.population_vector[:][col_names[i]] = numpy.random.uniform(min_value, max_value, self.parent_size).transpose()
            self.population_vector[:][col_names[i + 3]] = numpy.random.uniform(0, std_dev, self.parent_size).transpose()

    def modeling_function(self):
        for j in range(self.parent_size):
            a = len(self.file_data)
            function_values = numpy.empty(len(self.file_data))
            for i in range(len(self.file_data)):
                function_values[i] = self.population_vector[j]["a"] * (self.file_data[i][0] ** 2 - self.population_vector[j]["b"] *
                                                                    cos(self.population_vector[j]["c"] * pi * self.file_data[i][0]))
            self.aggregated_function_values[j][:] = function_values

    def evaluate_population(self):
        index = 0
        for value_vector in self.aggregated_function_values:
            mean_square_error = 0
            for i in range(len(self.file_data)):
                mean_square_error += (self.file_data[i][1] - value_vector[i]) ** 2
            mean_square_error /= len(self.file_data)
            self.error_list[index][0] = mean_square_error
            self.error_list[index][1] = index
            index += 1

    def create_offspring_population(self):
        random_numbers = numpy.random.randint(0, self.parent_size - 1, self.offspring_size)
        word_map = {"a", "b", "c", "std_dev_a", "std_dev_b", "std_dev_c"}
        index = 0
        for number in random_numbers:
            for word in word_map:
                self.offspring_population[index][word] = self.population_vector[number][word]
            index += 1

    def mutate_population(self, n):
        word_map = ["a", "b", "c", "std_dev_a", "std_dev_b", "std_dev_c"]
        for i in range(self.offspring_size):
            for j in range(len(word_map)-3):
                r = numpy.random.normal(0, self.offspring_population[i][word_map[j+3]], 1)
                self.offspring_population[i][word_map[j]] += r

        tau_1 = 1/sqrt(2*n)
        tau_2 = 1/sqrt(2*sqrt(n))

        for i in range(self.offspring_size):
            for word in {"std_dev_a", "std_dev_b", "std_dev_c"}:
                ro_1 = tau_1 * numpy.random.normal(0, 1, 1)
                ro_2 = tau_2 * numpy.random.normal(0, 1, 1)
                self.offspring_population[i][word] = self.offspring_population[i][word] * exp(ro_1) * exp(ro_2)

def main():
    file = numpy.loadtxt("model9.txt", dtype={'names': ('we', 'wy'), 'formats': ('float64', 'float64')})
    a = Population(100, file)
    a.create_new_population(-10,10,10)
    a.modeling_function()
    a.evaluate_population()
    a.create_offspring_population()
    a.mutate_population(3)

if __name__ == '__main__':
    main()