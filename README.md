# Evolutionary Algorithms

## Introduction

This repository consists of three implementations of evolutionary algorithms:

* Genetic Algorithm for Traveling Salesman Problem
* Evolution Strategies
* Ant Systems

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

The structure of chromosome representing an individual is a pair of float
vectors **v** = (**x**, σ). The first vector, **x** ∈ R<sup>n</sup>, represents
here a point in the n-dimensional search space and the second vector σ ∈
R<sup>n</sup><sub>+</sub> is the vector of standard deviations. The main idea
behind ES was to introduce a mechanism for a self-adaptation of the control
parameters. This is achieved by applying the genetic operators to the object
variable **x**, and to the strategy parameter σ. The mutation operator works by
adding a normally distributed random vector **r** = **N**(0, σ) with a mean of
zero and covariance matrix diag(σ<sup>2</sup>) to the object variable
**x**<sup>(t+1)</sup> = **x**<sup>(t)</sup> + r and the strategy parameter is
updated as follows σ<sup>(t+1)</sup> = σ<sup>(t)</sup> * exp(r<sub>σ1</sub>) ·
exp(**r**<sub>σ2</sub>), where superscript (t) denotes iteration t,
r<sub>σ1</sub> is a single realization of zero-mean gaussian variable with
variance τ<sub>1</sub><sup>2</sup>. The r<sub>σ1</sub> = N(0, τ<sub>1</sub>) =
τ<sub>1</sub> · N(0, 1) and **r**<sub>σ2</sub> = **N**(**0**,
τ<sub>2</sub><sup>2</sup>I) = τ<sub>2</sub> · **N**(**0**, **1**) is a single
realization of zero-mean gaussian variable with covariance matrix
τ<sub>2</sub><sup>2</sup>I, where I denotes identity matrix. Parameters
τ<sub>1</sub> and τ<sub>2</sub> usually are chosen as follows:
τ<sub>1</sub> = 1 / sqrt(2n), τ<sub>2</sub> = 1 / sqrt(2 * sqrt(n)).

The acceptance of offspring **x**<sup>(t+1)</sup> is determined by the level
of its fitness f(**x**<sup>(t+1)</sup>). The evolution strategies evolved into
two mature approaches denoted as (µ + λ) - ESs and (µ, λ) - ESs. The (µ + λ)
strategy selects the µ best solutions from the union of parents and offsprings.
In contrast, in the (µ, λ) strategy the best µ offsprings a selected from λ
(λ > µ) descendants to replace the parents (the life of each individual is
limited to only one generation).

Denoting the population in iteration t as P<sub>µ</sub><sup>(t)</sup> =
{v<sub>1</sub>, v<sub>2</sub>, ... , v<sub>µ</sub>} the evolution
strategies method can be summarized in the following steps:

1. Set iteration index, t = 0. Initialize P<sub>µ</sub><sup>(0)</sup>.
2. Evaluate P<sub>µ</sub><sup>(0)</sup>.
3. Perform reproduction of P<sub>µ</sub><sup>(t)</sup> to
R<sub>λ</sub><sup>(t)</sup>.
4. Perform mutation of R<sub>λ</sub><sup>(t)</sup> to
O<sub>λ</sub><sup>(t)</sup>.
5. Evaluate O<sub>λ</sub><sup>(t)</sup>.
6. Select (O<sub>λ</sub><sup>(t)</sup> ∪ Q) to P<sub>µ</sub><sup>(t+1)</sup>.
7. Update iteration index, t = t + 1.
8. If termination condition is not satisfied go to the Step 3.

In the (µ, λ) strategy Q = ∅ and in the (µ + λ) strategy Q =
P<sub>µ</sub><sup>(t)</sup>. In the third step, the individuals from µ parents
are reproduced to λ individuals in the set R<sub>λ</sub><sup>(t)</sup> by
independently random drawn individuals from P<sub>µ</sub><sup>(t)</sup>. As the
termination condition usually pre-set number of iteration or achieving of
pre-set value of the cost function is used.


#### Task

Write a computer program to solve the optimization problem provided by a tutor
using the Evolution Strategy. Implement both, (µ, λ) and (µ + λ) approaches.
As the population varying operator only mutation should be used. Evaluate the
influence of the ES parameters (number of individuals, number of offspring,
selection and crossover method, etc.) on the performance and the time of
computations.

A given dataset contains a series of N = 101 measurements of input
*i<sub>n</sub>* and output *o<sub>n</sub>* of a certain system *f* which can be
modeled using the following function: **o** = a(i<sup>2</sup> - b * cos(cπi)).
Find values of the parameters a, b and c using the Evolution Strategies method,
minimizing the mean square error between o and **o**. Generate the initial
population according to uniform distribution −10 ≤ a, b, c ≤ 10,
0 ≤ σ<sub>a</sub>, σ<sub>b</sub>, σ<sub>c</sub> ≤ 10.

### Ant Systems

#### Algorithm

Ant System is one of many variants of ant colony optimization algorithms. In
this approach solutions are constructed by moving m artificial ants on the
problem graph. The ants build a tour visiting n = N cities using a stochastic
decision rule presented later. After a complete tour is constructed the
artificial ants deposit the pheromone trail (update a variable associated with
each arc). The amount of pheromone τ<sub>ij</sub>(t) deposed in iteration t on
arc (i, j) represents the desirability to move from city i to city j. The
quantity of pheromone deposed by every ant is proportional to the quality of
the solution found. The internal memory of each ant contains the list of already
visited cities and is used to identify the details of the chosen path. By
exploiting its memory an ant k can build a set of feasible solutions.

The ant decision in node *i* is based on a decision table **A**<sub>i</sub> =
\[a<sub>ij</sub>(t)\]<sub>\[N<sub>i</sub>\]</sub> obtained using the following
rule: a<sub>ij</sub>(t) = (τ<sub>ij</sub>(t))<sup>α</sup> * 
(η<sub>ij</sub>)<sup>β</sup> / sum<sub>l∈N<sub>i</sub><sup>k</sup></sub>
((τ<sub>ij</sub>(t))<sup>α</sup> * (η<sub>ij</sub>)<sup>β</sup>), ∀j ∈ 
N<sub>i</sub>, where τ<sub>ij</sub> is the amount of pheromone on arc (i, j)
at time t, η<sub>ij</sub> = 1/d<sub>ij</sub>, N<sub>i</sub> is the set of
neighbors cities of city i, N<sub>i</sub><sup>k</sup> ⊆ N<sub>i</sub> is a set
of neighbor cities of city i that ant k has not visited yet, and α and β are
control parameters.

The ant *k* chooses to go from city *i* to city *j* ∈ N<sub>i</sub><sup>k</sup>
with probability p<sub>ij</sub><sup>k</sup> = a<sub>ij</sub>(t) /
sum<sub>l∈N<sub>i</sub><sup>k</sup></sub>a<sub>ij</sub>(t). After all m ants
have completed their tour the deposition and evaporation of pheromone mechanism
is started. This mechanism can be described by the formula τ<sub>ij</sub>(t + 1)
= (1 − ρ)τ<sub>ij</sub>(t) + ∆τ<sub>ij</sub>(t), where ρ ∈ [0, 1] is the
pheromone evaporation coefficient, and the total quantity of pheromone 
∆τ<sub>ij</sub>(t) deposed on arc (i, j) is specified by ∆τ<sub>ij</sub>(t) =
sum<sub>k=1</sub><sup>m</sup>∆τ<sub>ij</sub><sup>k</sup>(t). The quantity of
pheromone ∆τ<sub>ij</sub><sup>k</sup>(t) deposed by ant *k* on arc (i, j) (that
it has used) is equal to 1 / L<sup>k</sup>(t) if (i, j) ∈ T<sup>k</sup>(t) else
0, where L<sup>k</sup>(t) is the length of the tour T<sup>k</sup>(t) made by
the k-th ant.

Before the start of the optimization algorithm a small amount of pheromone
τ<sup>0</sup> > 0 is deposed on all arcs τ<sub>ij</sub><sup>k</sup>(0) =
τ<sub>0</sub>. A good choice for parameter values is m = n, α = 1, β = 5 and
ρ = 0.5 but the optimal set depends on the investigated problem.

#### Tasks

1. Write a computer program simulating the ants colony behavior in an environment
where the nest and the food are separated by a double, asymmetric bridge.
2. Implement the Ant System to solve the traveling salesman problem. Use a map
of cities provided by a tutor, m = N = 10, the maximum number of tours 
Tmax = 200. What was the minimal total distance traveled? What is the sequence
of cities to be visited ensuring the minimal total distance traveled? Show the
results in a graphic form (as a network of connections).

## Installation

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
