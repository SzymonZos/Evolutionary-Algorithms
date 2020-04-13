from Population import Population, open_file_and_process_data


def main():
    file = open_file_and_process_data("model9.txt")
    stop_criteria = 0.3
    population_size = 100
    min_param_value = -10
    max_param_value = 10
    std_var_value = 10
    population = Population(population_size, file, stop_criteria)
    population.initialize_population(min_param_value, max_param_value, std_var_value)

    while True:
        population.iterations += 1
        parents_function_values = population.modeling_function("parents")
        parents_error = population.evaluate_population(parents_function_values)
        population.create_offspring_population()
        population.mutate_population(3)
        offspring_function_values = population.modeling_function("offspring")
        offspring_error = population.evaluate_population(offspring_function_values)
        population.create_new_population(parents_error, offspring_error)
        if population.is_stop_criteria_fulfilled(offspring_error):
            break
    population.print_population()


if __name__ == '__main__':
    main()