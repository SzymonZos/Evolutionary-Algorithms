from Population import Population, open_file_and_process_data
from Timer import Timer


def main():

    my_timer = Timer()
    file = open_file_and_process_data("model9.txt")
    stop_criteria = 0.3
    population_size = 100
    min_param_value = -10
    max_param_value = 10
    std_var_value = 10
    population = Population(population_size, file, stop_criteria)
    population.initialize(min_param_value, max_param_value, std_var_value)
    population.run()
    population.print(False)


if __name__ == '__main__':
    main()
