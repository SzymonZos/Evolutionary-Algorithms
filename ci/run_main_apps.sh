#!/usr/bin/env bash
set -Eeuo pipefail

echo "AntSystems"
./build/AntSystems/bin/AntSystems

echo "EvolutionStrategies"
./build/EvolutionStrategies/bin/EvolutionStrategies

echo "TravellingSalesmanProblem"
./build/TravellingSalesmanProblem/bin/TravellingSalesmanProblem
