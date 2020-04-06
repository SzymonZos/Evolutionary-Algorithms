import math
import re

dictionary = {"100, 0.5, 0.1\n": 0, "100, 0.5, 0.3\n": 1, "100, 0.5, 0.5\n": 2,
              "100, 0.7, 0.1\n": 3, "100, 0.7, 0.3\n": 4, "100, 0.7, 0.5\n": 5,
              "100, 0.9, 0.1\n": 6, "100, 0.9, 0.3\n": 7, "100, 0.9, 0.5\n": 8,

              "300, 0.5, 0.1\n": 9, "300, 0.5, 0.3\n": 10, "300, 0.5, 0.5\n": 11,
              "300, 0.7, 0.1\n": 12, "300, 0.7, 0.3\n": 13, "300, 0.7, 0.5\n": 14,
              "300, 0.9, 0.1\n": 15, "300, 0.9, 0.3\n": 16, "300, 0.9, 0.5\n": 17,

              "500, 0.5, 0.1\n": 18, "500, 0.5, 0.3\n": 19, "500, 0.5, 0.5\n": 20,
              "500, 0.7, 0.1\n": 21, "500, 0.7, 0.3\n": 22, "500, 0.7, 0.5\n": 23,
              "500, 0.9, 0.1\n": 24, "500, 0.9, 0.3\n": 25, "500, 0.9, 0.5\n": 26
              }

with open("bin/logs/results.csv", "r") as file:
    logs = file.readlines()

for i, log in enumerate(logs):
    logs[i] = re.sub(r'\[.+info\]', '', log)

sorted_data = []
for i in range(len(dictionary)):
    sorted_data.append([])

for line in logs[1:]:
    iterator_to_key = line.find(", ") + 2
    value = dictionary[line[iterator_to_key:]]
    sorted_data[value].append(line)

with open("processed_data.csv", "w") as write_file:
    logs[0] = logs[0].rstrip() + ", Average, " \
                                                           "Variance, " \
                                                           "standard " \
                                                           "deviation\n "
    write_file.write(logs[0])

    for one_type in sorted_data:
        average = 0
        for line in one_type:
            average += float(line[0:line.find(",") - 1])
        average /= len(one_type)
        variance = 0
        for line in one_type:
            variance += (float(line[0:line.find(",") - 1]) - average) * \
                        (float(line[0:line.find(",") - 1]) - average)
        variance = variance / len(one_type)
        std_deviation = math.sqrt(variance)
        one_type[len(one_type) - 1] += ", , , , " + str(round(average, 4)) + \
                                       ", " + str(round(variance, 4)) + \
                                       ", " + str(round(std_deviation, 4)) + \
                                       "\n"

    for one_type in sorted_data:
        write_file.writelines(one_type)
        write_file.write("\n")
