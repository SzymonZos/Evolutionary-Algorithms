import numpy
from math import cos, pi, sqrt, exp
from matplotlib import pyplot

POPULATION = {'names': ('a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c'),
              'formats': ('float64', 'float64', 'float64', 'float64', 'float64', 'float64')}


def open_file_and_process_data(file_name):
    file = numpy.loadtxt(file_name, dtype={'names': ('we', 'wy'), 'formats': ('float64', 'float64')})
    return file


class Population:
    def __init__(self, size, file_data, stop_criteria):
        self.parent_size = size
        self.offspring_size = size * 6
        self.file_data = file_data
        self.stop_criteria = stop_criteria
        self.iterations = 0
        self.print_index = 0
        self.population_vector = numpy.empty(self.parent_size, dtype=POPULATION)
        self.offspring_population = numpy.empty([self.offspring_size], dtype=POPULATION)
        self.col_names = ['a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c']

    def initialize(self, min_value, max_value, std_dev):
        for i in range(len(self.col_names[0:3])):
            self.population_vector[:][self.col_names[i]] = numpy.random.uniform(min_value, max_value,
                                                                                self.parent_size).transpose()
            self.population_vector[:][self.col_names[i + 3]] = numpy.random.uniform(0, std_dev,
                                                                                    self.parent_size).transpose()

    def modeling_function(self, population_name):
        if population_name == "parents":
            size = self.parent_size
            population = self.population_vector
        elif population_name == "offspring":
            size = self.offspring_size
            population = self.offspring_population
        else:
            return "error"
        aggregated_function_values = numpy.empty((size, len(self.file_data)))
        for j in range(size):
            function_values = numpy.empty(len(self.file_data))
            for i in range(len(self.file_data)):
                function_values[i] = population[j]["a"] * (self.file_data[i][0] ** 2 - population[j]["b"] *
                                                           cos(population[j]["c"] * pi * self.file_data[i][0]))
            aggregated_function_values[j][:] = function_values
        return aggregated_function_values

    def evaluate(self, aggregated_function_values):
        error_list = numpy.empty([len(aggregated_function_values), 3])
        index = 0
        for value_vector in aggregated_function_values:
            mean_square_error = 0
            for i in range(len(self.file_data)):
                mean_square_error += (self.file_data[i][1] - value_vector[i]) ** 2
            mean_square_error /= len(self.file_data)
            error_list[index][0] = mean_square_error
            error_list[index][1] = index
            index += 1
        return error_list

    def create_offspring(self):
        random_numbers = numpy.random.randint(0, self.parent_size - 1, self.offspring_size)
        index = 0
        for number in random_numbers:
            for word in self.col_names:
                self.offspring_population[index][word] = self.population_vector[number][word]
            index += 1

    def mutate_population(self, n):
        for i in range(self.offspring_size):
            for j in range(len(self.col_names)-3):
                r = numpy.random.normal(0, self.offspring_population[i][self.col_names[j+3]], 1)
                self.offspring_population[i][self.col_names[j]] += r

        tau_1 = 1/sqrt(2*n)
        tau_2 = 1/sqrt(2*sqrt(n))

        for i in range(self.offspring_size):
            for word in self.col_names[3:6]:
                ro_1 = tau_1 * numpy.random.normal(0, 1, 1)
                ro_2 = tau_2 * numpy.random.normal(0, 1, 1)
                self.offspring_population[i][word] = self.offspring_population[i][word] * exp(ro_1) * exp(ro_2)

    def create_new(self, parent_error, offspring_error):
        parent_error[:, 2] = 1
        offspring_error[:, 2] = 0
        concatenated_array = numpy.concatenate((parent_error, offspring_error), axis=0)
        concatenated_array = concatenated_array[concatenated_array[:, 0].argsort()]
        new_population = numpy.empty([self.parent_size], dtype=POPULATION)

        for i in range(self.parent_size):
            if concatenated_array[i][2] == 1.0:
                new_population[i] = self.population_vector[int(concatenated_array[i][1])]
            elif concatenated_array[i][2] == 0:
                new_population[i] = self.offspring_population[int(concatenated_array[i][1])]
        self.population_vector = new_population

    def is_stop_criteria_fulfilled(self, offspring_error):
        statement = False
        for error in offspring_error:
            if error[0] < self.stop_criteria:
                statement = True
                self.print_index = error[1]
                break
        return statement

    def print(self, plot_graph):
        function_values = numpy.empty(len(self.file_data))
        for i in range(len(self.file_data)):
            function_values[i] = self.offspring_population[int(self.print_index)]["a"] * (self.file_data[i][0] ** 2 -
                                 self.offspring_population[int(self.print_index)]["b"] *
                                 cos(self.offspring_population[int(self.print_index)]["c"] * pi * self.file_data[i][0]))
        if plot_graph:
            pyplot.plot(self.file_data['we'], self.file_data['wy'], "r--", self.file_data['we'], function_values, "g^")
            pyplot.show()
        print("Iterations: " + str(self.iterations))
        print("vector x: " + str(self.offspring_population[int(self.print_index)]))

    def run(self):
        while True:
            self.iterations += 1
            parents_function_values = self.modeling_function("parents")
            parents_error = self.evaluate(parents_function_values)
            self.create_offspring()
            self.mutate_population(3)
            offspring_function_values = self.modeling_function("offspring")
            offspring_error = self.evaluate(offspring_function_values)
            self.create_new(parents_error, offspring_error)
            if self.is_stop_criteria_fulfilled(offspring_error):
                break
