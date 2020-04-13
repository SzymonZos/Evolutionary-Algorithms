import numpy
from matplotlib import pyplot
from math import cos, pi, sqrt, exp
from collections import defaultdict

POPULATION = {'names': ('a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c'),
              'formats': ('float64','float64','float64','float64','float64','float64')}
def open_file_and_process_data():
    file = numpy.loadtxt("model9.txt", dtype={'names': ('we','wy'), 'formats': ('float64', 'float64')})
    return file


def modeling_function(no_population, file_data, population_vector):
    aggregated_function_values = numpy.empty((no_population, len(file_data)))
    for j in range(no_population):
        a = len(file_data)
        function_values = numpy.empty(len(file_data))
        for i in range(len(file_data)):
            function_values[i] = population_vector[j]["a"] * (file_data[i][0] ** 2 - population_vector[j]["b"] *
                                                                cos(population_vector[j]["c"] * pi * file_data[i][0]))
        aggregated_function_values[j][:] = function_values
    return aggregated_function_values


def gererate_chromosomes(low, high, number_of_elements):
    col_names = ['a', 'b', 'c', 'std_dev_a', 'std_dev_b', 'std_dev_c']
    population_vector = numpy.empty([number_of_elements], dtype=POPULATION)
    for i in range(len(col_names[0:3])):
        #abc_std_dev_vec.append(numpy.random.uniform(low, high, number_of_elements))
        population_vector[:][col_names[i]] = numpy.random.uniform(low, high, number_of_elements).transpose()
        population_vector[:][col_names[i+3]] = numpy.random.uniform(0, high, number_of_elements).transpose()

    return population_vector


def evaluate_population(aggregated_function_values, file_data):
    error_list = numpy.empty([len(aggregated_function_values), 3])
    index = 0
    for value_vector in aggregated_function_values:
        mean_square_error = 0
        for i in range(len(file_data)):
            mean_square_error += (file_data[i][1] - value_vector[i]) ** 2
        mean_square_error /= len(file_data)
        error_list[index][0] = mean_square_error
        error_list[index][1] = index
        index += 1

    return error_list


def create_offspring_population(no_parents, no_offspring, chromosome_vector):
    random_numbers = numpy.random.randint(0, no_parents - 1, no_offspring)
    # offspring_population = defaultdict(list)
    offspring_population = numpy.empty([no_offspring],
                                    dtype=POPULATION)

    word_map = {"a", "b", "c", "std_dev_a", "std_dev_b", "std_dev_c"}
    index = 0
    for number in random_numbers:
        for word in word_map:
            offspring_population[index][word] = chromosome_vector[number][word]
        index += 1
    return offspring_population


def mutate_population(n, no_offspring, offspring_population):
    word_map = ["a", "b", "c", "std_dev_a", "std_dev_b", "std_dev_c"]
    for i in range(no_offspring):
        for j in range(len(word_map)-3):
            r = numpy.random.normal(0, offspring_population[i][word_map[j+3]], 1)
            offspring_population[i][word_map[j]] += r

    tau_1 = 1/sqrt(2*n)
    tau_2 = 1/sqrt(2*sqrt(n))

    for i in range(no_offspring):
        for word in {"std_dev_a", "std_dev_b", "std_dev_c"}:
            ro_1 = tau_1 * numpy.random.normal(0, 1, 1)
            ro_2 = tau_2 * numpy.random.normal(0, 1, 1)
            offspring_population[i][word] = offspring_population[i][word] * exp(ro_1) * exp(ro_2)
    return offspring_population


def create_new_population(offsprin_error, parent_error, no_parents, parent_population, offspring_population):
    concatenated_array = numpy.concatenate((parent_error, offsprin_error), axis=0)
    concatenated_array = concatenated_array[concatenated_array[:, 0].argsort()]
    #print(concatenated_array[1][:])
    new_population = numpy.empty([no_parents], dtype=POPULATION)

    word_map = ["a", "b", "c", "std_dev_a", "std_dev_b", "std_dev_c"]
    for i in range(no_parents):
        if concatenated_array[i][2] == 1.0:
            new_population[i] = parent_population[int(concatenated_array[i][1])]
        elif concatenated_array[i][2] == 0:
            new_population[i] = offspring_population[int(concatenated_array[i][1])]
    return new_population

def main():
    iterations = 0
    low, high, no_parents = -10, 10, 200  # number_of_elements = population
    no_offspring = no_parents*6
    data = open_file_and_process_data()
    chromosome_vector = gererate_chromosomes(low, high, no_parents)
    STOP_CRIT = 0.3
    values_index = 0
    stop = False
    while True:
        iterations += 1
        aggregated_values = modeling_function(no_parents, data, chromosome_vector)

        error_parent_list = evaluate_population(aggregated_values, data)
        error_parent_list[:, 2] = 1
        offspring_population = create_offspring_population(no_parents, no_offspring, chromosome_vector)


        n = 3 #bog wie co to n
        offspring_population = mutate_population(n, no_offspring, offspring_population)

        aggregated_values_offspring = modeling_function(no_offspring, data, offspring_population)
        error_offspring_list = evaluate_population(aggregated_values_offspring, data)
        error_offspring_list[:, 2] = 0

        new_population = create_new_population(error_offspring_list, error_parent_list, no_parents, chromosome_vector,
                                               offspring_population)

        chromosome_vector = new_population

        for error in error_offspring_list:
            if error[0] < STOP_CRIT:
                stop = True
                print(error)
                values_index = error[1]
                break
        if stop:
            break

    #print_values=  modeling_function(1, data, offspring_population[int(values_index)])



    function_values = numpy.empty(len(data))
    for i in range(len(data)):
        function_values[i] = offspring_population[int(values_index)]["a"] * (data[i][0] ** 2 -
                                                  offspring_population[int(values_index)]["b"] *
                                                  cos(offspring_population[int(values_index)]["c"] * pi * data[i][0]))
    pyplot.plot(data['we'], data['wy'], "r--", data['we'], function_values, "g^")
    pyplot.show()
    print(iterations)
    print("vector x: " + str(offspring_population[int(values_index)]))
    a=1
    #In the (µ, λ) strategy Q = ∅ and in the (µ + λ) strategy Q = P(t)µ
if __name__ == "__main__":
    main()