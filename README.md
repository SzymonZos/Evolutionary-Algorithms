# Evolutionary Algorithms

## Introduction

TODO

## Description

### Genetic Algorithm for Traveling Salesman Problem

#### Algorithm

1. Create an initial population of P chromosomes (parents population).
2. Evaluate the cost (the total distance to be traveled) of each individual.
3. Choose n * P parents from the current population via proportional selection,
where n - (0,1].
4. Select randomly two parents to create offspring using crossover operator.
5. Repeat the Step 4 until n * P offspring are generated.
6. Apply mutation operators for changes in randomly selected offspring.
7. Replace old parent population with the best (of minimum cost) P individuals
selected from the combined population of parents and offspring.
8. If the maximum number of generations (T<sub>max</sub>) were performed, then
stop the algorithm; otherwise go to the Step 2.

The selection criteria, crossover and mutation are the major operators but
cross-over plays the most important role. In the presented solution the Cycle
Crossover Operator (CX) is applied.

#### Tasks

1. Implement the described GA algorithm to solve the traveling salesman problem.
A map of cities provided by a tutor x = [0, 3, 6, 7, 15, 12, 14, 9, 7, 0],
y = [1, 4, 5, 3, 0, 4, 10, 6, 9, 10]; N = 10,  and the GA parameters: P = 250,
n = 0.8, p<sub>m</sub> = 0.2, T<sub>max</sub> = 1000. What was the minimal total
distance traveled? What is the sequence of cities to be visited ensuring
the minimal total distance traveled? Show the results in a graphic form
(as a network of connections).
2. Investigate the influence of parameters P, n and p<sub>m</sub> on the mean
minimal total distance traveled calculated for 10 trials. Change the values
of P within a set {100, 300, 500}, n - {0.5, 0.7, 0.9}
and p<sub>m</sub> - {0.1, 0.3, 0.5}.

### Evolution Strategies

#### Algorithm

Denoting the population in iteration t as $$P_{µ}^{(t)}$$

#### Task

Write a computer program to solve the optimization problem provided by a tutor
using the Evolution Strategy. Implement both, (µ, λ) and (µ + λ) approaches.
As the population varying operator only mutation should be used. Evaluate the
influence of the ES parameters (number of individuals, number of offspring,
selection and crossover method, etc.) on the performance and the time of
computations.

### Ant Systems

TODO

## Installation

TODO

### Windows specific black magic

1. [Download](https://www.python.org/downloads/release/python-2717/)
desired ```Python 2.7``` version and install these modules.
   ```shell script
   pip2 install numpy matplotlib
   ```

2. Follow [this 
guide](https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2)
to install desired toolchain ```MinGW```. Note that CMake 3.16 comes with great
opportunity to lose developer's mind under Windows. A nice feature that prevents
CMake from working was added there. One **cannot** include *sh.exe* in the path.
To solve this issue simply use one of newer versions (3.17 was tested and
worked quite nicely).

3. Using ```MSYS2``` shell execute following commands in the desired directory.
   ```shell script
   git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
   cd Evolutionary-Algorithms
   git submodule update --init --recursive
   ```

4. Then run following commands using ```cmd``` from Evolutionary-Algorithms
 directory.
   ```shell script
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   cmake --build .
   ```

5. Choose desired built binary and run it.

### Normal operating systems

1. Install Python ```2.7``` using your package manager. Unfortunately 
```Python 3``` does not cooperate well.

2. Run following commands. Note that default system generator, compiler and
build type are chosen in this way. To change this behavior, proceed with
standard CMake command ```-D```.
   ```shell script
   pip2 install numpy matplotlib
   git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
   cd Evolutionary-Algorithms
   git submodule update --init --recursive
   mkdir build
   cd build
   cmake ..
   cmake --build . 
   ```
