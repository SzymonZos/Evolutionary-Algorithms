# Evolutionary Algorithms

## Genetic Algorithm for Traveling Salesman Problem

### Description

### Installation

#### Windows specific black magic

1. Follow [this guide](https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2) to install desired toolchain (MinGW).

2. To successfully run MinGW toolchain:
   * One **cannot** have included *sh.exe* in the path. Usually it comes with git modules to support git bash.
   * It has to be run from *cmd* or *PowerShell*.

3. Using **MSYS2** shell execute following commands in desired directory
   ```
   git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
   cd Evolutionary-Algorithms/
   git submodule update --init --recursive
   ```

4. Then run following commands using **cmd** from Evolutionary-Algorithms/ directory
   ```
   mkdir TravellingSalesmanProblem/build/
   cd TravellingSalesmanProblem/build/
   cmake ..
   mingw32-make
   ../bin/TravellingSalesmanProblem.exe
   ```

#### Normal operating systems

```
git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
cd Evolutionary-Algorithms/
git submodule update --init --recursive
mkdir TravellingSalesmanProblem/build/
cd TravellingSalesmanProblem/build/
cmake ..
make
../bin/TravellingSalesmanProblem
```
