import math

dictionary = {"100, 0.5, 0.1\n": 0, "100, 0.5, 0.3\n": 1, "100, 0.5, 0.5\n": 2,
              "100, 0.7, 0.1\n": 3, "100, 0.7, 0.3\n": 4, "100, 0.7, 0.5\n": 5,
              "100, 0.9, 0.1\n": 6, "100, 0.9, 0.3\n": 7, "100, 0.9, 0.5\n": 8,

              "300, 0.5, 0.1\n": 9, "300, 0.5, 0.3\n": 10, "300, 0.5, 0.5\n": 11,
              "300, 0.7, 0.1\n": 12, "300, 0.7, 0.3\n": 13, "300, 0.7, 0.5\n": 14,
              "300, 0.9, 0.1\n": 15, "300, 0.9, 0.3\n": 16, "300, 0.9, 0.5\n": 17,

              "500, 0.5, 0.1\n": 18, "500, 0.5, 0.3\n": 19,"500, 0.5, 0.5\n": 20,
              "500, 0.7, 0.1\n": 21, "500, 0.7, 0.3\n": 22, "500, 0.7, 0.5\n": 23,
              "500, 0.9, 0.1\n": 24, "500, 0.9, 0.3\n": 25, "500, 0.9, 0.5\n": 26
              }

File=open("results.1.csv", "r")
all_lines_w_logs = File.readlines()
File.close()

full_file_wo_logs = []
for line in all_lines_w_logs:
    single_line = line[(line.find("[info]") + 7):]
    full_file_wo_logs.append(single_line)

sorted_data = []
for i in range(len(dictionary)):
    sorted_data.append([])

for line in full_file_wo_logs[1:]:
    iterator_to_key = line.find(", ") + 2
    value = dictionary[line[iterator_to_key:]]
    sorted_data[value].append(line)

write_file=open("processed_data.csv", "w")
full_file_wo_logs[0] = full_file_wo_logs[0].rstrip() + ", Average, Variance, standard deviation\n"
write_file.write(full_file_wo_logs[0])

for one_type in sorted_data:
    average = 0
    for line in one_type:
        average += float(line[0:line.find(",") - 1])
    average= average / len(one_type)
    variance = 0
    for line in one_type:
        variance += (float(line[0:line.find(",") - 1]) - average) * (float(line[0:line.find(",") - 1]) - average)
    variance = variance / len(one_type)
    std_deviation = math.sqrt(variance)
    one_type[len(one_type) - 1] += ", , , , " + str(round(average, 4)) + ", " + str(round(variance, 4)) + ", " + \
                                                                                str(round(std_deviation, 4)) + "\n"

for one_type in sorted_data:
    write_file.writelines(one_type)
    write_file.write("\n")

write_file.close()