# Evolutionary Algorithms

## Genetic Algorithm for Traveling Salesman Problem

### Description

### Installation

#### Windows specific black magic

1. [Download](https://www.python.org/downloads/release/python-2717/) desired Python2.7 version.
   ```shell script
   pip2 install numpy matplotlib
   ```

2. Follow [this guide](https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2) to install desired toolchain (MinGW).

3. To successfully run MinGW toolchain:
   * One **cannot** have included *sh.exe* in the path. Usually it comes with git modules to support git bash.
   * It has to be run from *cmd* or *PowerShell*.

4. Using **MSYS2** shell execute following commands in desired directory
   ```shell script
   git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
   cd Evolutionary-Algorithms
   git submodule update --init --recursive
   ```

5. Then run following commands using **cmd** from Evolutionary-Algorithms/ directory
   ```shell script
   mkdir TravellingSalesmanProblem\build
   cd TravellingSalesmanProblem\build
   cmake ..
   mingw32-make
   ..\bin\TravellingSalesmanProblem.exe
   ```

#### Normal operating systems

1. Install Python 2.7 using your package manager. Unfortunately Python 3 does not cooperate well.

```
pip2 install numpy matplotlib
git clone https://github.com/SzymonZos/Evolutionary-Algorithms.git
cd Evolutionary-Algorithms/
git submodule update --init --recursive
mkdir TravellingSalesmanProblem/build/
cd TravellingSalesmanProblem/build/
cmake ..
make
../bin/TravellingSalesmanProblem
```
