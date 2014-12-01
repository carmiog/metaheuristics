/*
 * main.cpp
 *
 *  Created on: 25 nov. 2014
 *      Author: antoinemarendet
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cfloat>
#include <vector>
#include "data.hpp"
#include "solution.hpp"
using namespace std;

void printData(data &d) {
	cout << d.variableCount << " variables, " << d.constraintCount << " constraints" << endl;

	for (int i = 0; i < d.variableCount; ++i) {
		cout << "var " << i << ": ";
		for (int j = 0; j < d.constraintsByVariable[i].size(); ++j) {
			cout << d.constraintsByVariable[i][j] << " ";
		}
		cout << endl;
	}
}


void printSolution(solution &s) {
	cout << s.cost << endl;
	for (int i = 0; i < s.varsValues.size(); ++i) {
		cout << s.varsValues[i];
	}
	cout << endl;
	/*for(int i = 0; i < s.satisfiedConstraints.size(); ++i) {
		cout << s.satisfiedConstraints[i] << " ";
	}
	cout << endl;*/
}

data readDataFile(const char *dataFile) {
	FILE *f = fopen(dataFile, "r");
	data d;
	if (f) {
		int buff;
		fscanf(f, "%d %d", &d.constraintCount, &d.variableCount);
		for (int i = 0; i < d.variableCount; ++i) {
			fscanf(f, "%d", &buff);
			d.costs.push_back(buff);
			d.constraintsByVariable.push_back(vector<int>());
		}
		for (int i = 0; i < d.constraintCount; ++i) {
			fscanf(f, "%d", &buff);
			d.constraintsLength.push_back(buff);
			vector<int> constraintLine;
			for (int constraintIndex = 0; constraintIndex < d.constraintsLength[i]; ++constraintIndex) {
				fscanf(f, "%d", &buff);
				d.constraintsByVariable[buff - 1].push_back(i);
				constraintLine.push_back(buff);
			}
			d.constraints.push_back(constraintLine);
		}
		fclose(f);

	} else {
		printf("Error while reading file %s.\n", dataFile);
	}
	return d;
}

void computeSolutionValue(data &d, solution &s) {
	int value = 0;
	for (int i = 0; i < s.varsValues.size(); ++i) {
		value += d.costs[i] * s.varsValues[i];
	}
	s.cost = value;
}

bool contains(vector<int> vect, int value) {
	for (vector<int>::iterator it = vect.begin(); it != vect.end(); ++it) {
		if (*it == value) {
			return true;
		}
	}
	return false;
}

int getBestUtilityVariable(data &d, solution &s) {
	double bestUtility = 0.;
	int bestVar = 0;
	for (int var = 0; var < d.variableCount; ++var) {
		int rowCount = 0;
		for (int line = 0; line < d.constraintsByVariable[var].size(); ++line) {
			int constraint = d.constraintsByVariable[var][line];
			if (s.satisfiedConstraints[constraint] == 0) {
				rowCount++;
			}
		}
		double utility = double(rowCount) / double(d.costs[var]);
		if (utility > bestUtility) {
			bestUtility = utility;
			bestVar = var;
		}
	}
	return bestVar;
}

bool isFeasibleSolution(solution &s) {

	for (int i = 0; i < s.satisfiedConstraints.size(); ++i) {
		if (s.satisfiedConstraints[i] == 0) {
			return false;
		}
	}
	return true;
}
solution createInitialSolution(data &d) {
	solution s;
	for (int i = 0; i < d.constraintCount; ++i) {
		s.satisfiedConstraints.push_back(0);
	}
	for (int i = 0; i < d.variableCount; ++i) {
		s.varsValues.push_back(0);
	}

	while (!isFeasibleSolution(s)) {
		int bestVariable = getBestUtilityVariable(d, s);
		s.varsValues[bestVariable] = 1;
		for (int i = 0; i < d.constraintsByVariable[bestVariable].size(); ++i) {
			int constraints = d.constraintsByVariable[bestVariable][i];
			s.satisfiedConstraints[constraints] += 1;
		}
	}
	computeSolutionValue(d, s);
	return s;
}

vector<solution> createNeighborhood(data &d, solution &s, int k) {
	vector<solution> neighborhood;
	if (k >= 1) {
		// DROP
		for (int var = 0; var < d.variableCount; ++var) {
			if (s.varsValues[var] == 1) {
				//DROP possibility
				bool canDrop = true;
				for (int line = 0; line < d.constraintsByVariable[var].size(); ++line) {
					int constraint = d.constraintsByVariable[var][line];
					if (s.satisfiedConstraints[constraint] == 1) {
						canDrop = false;
						break;
					}
				}
				if (canDrop) {
					solution newSol = copySolution(s);
					for (int line = 0; line < d.constraintsByVariable[var].size(); ++line) {
						int constraint = d.constraintsByVariable[var][line];
						newSol.satisfiedConstraints[constraint] -= 1;
					}
					newSol.varsValues[var] = 0;
					computeSolutionValue(d, newSol);
					neighborhood.push_back(newSol);
				}
			}
		}
	}
	if (k >= 2) {
		// ADD-DROP
		for (int var = 0; var < d.variableCount; ++var) {
			if (s.varsValues[var] == 1) {
				for (int var2 = 0; var2 < d.variableCount; ++var2) {
					if (s.varsValues[var2] == 0) {
						bool canExchange = true;
						for (int line = 0; line < d.constraintsByVariable[var].size(); ++line) {
							int constraint = d.constraintsByVariable[var][line];
							if (s.satisfiedConstraints[constraint] == 1
									&& !contains(d.constraintsByVariable[var2], constraint)) {
								canExchange = false;
								break;
							}
						}
						if (canExchange) {
							solution newSol = copySolution(s);
							for (int line = 0; line < d.constraintsByVariable[var].size(); ++line) {
								int constraint = d.constraintsByVariable[var][line];

								newSol.satisfiedConstraints[constraint] -= 1;
							}
							for (int line = 0; line < d.constraintsByVariable[var2].size(); ++line) {
								int constraint = d.constraintsByVariable[var2][line];

								newSol.satisfiedConstraints[constraint] += 1;
							}
							newSol.varsValues[var] = 0;
							newSol.varsValues[var2] = 1;
							computeSolutionValue(d, newSol);
							neighborhood.push_back(newSol);
						}
					}
				}
			}
		}
	}
	return neighborhood;
}

solution localSearch(data &d, solution &s, int k) {

	vector<solution> neighborhood = createNeighborhood(d, s, k);

	solution best;
	best.cost = INT_MAX;
	for (vector<solution>::iterator it = neighborhood.begin(); it != neighborhood.end(); ++it) {
		if (it->cost < best.cost) {
			best = *it;
		}
	}
	return best;
}

int random_range(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

solution pickRandomSolution(vector<solution> neighborhood) {
	int solutionIndex = random_range(0, neighborhood.size() - 1);
	return neighborhood[solutionIndex];
}


int main(int argc, char **argv) {
	srand(time(0));
	string dataFile("scp41.txt");
	data d = readDataFile(dataFile.c_str());

	int maxIterations = 100;
	int iterations = 0;
	int k;
	solution currentSolution = createInitialSolution(d);
	printSolution(currentSolution);
	while (iterations < maxIterations) {
		iterations++;
		k = 1;
		while (k < 3) {

			vector<solution> neighborhood = createNeighborhood(d, currentSolution, k);

			solution localOptimum = currentSolution;
			if (!neighborhood.empty()) {
				solution randomNeighbor = pickRandomSolution(neighborhood);
				localOptimum = localSearch(d, randomNeighbor, k);

			}
			if (localOptimum.cost < currentSolution.cost) {
				currentSolution = localOptimum;

				k = 1;
			} else {
				k += 1;
			}
		}
	}
	printSolution(currentSolution);
}

