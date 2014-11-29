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
				d.constraintsByVariable[buff].push_back(constraintIndex);
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

int getSolutionValue(data &d, solution &s) {
	int value = 0;
	for (int i = 0; i < s.varsValues.size(); ++i) {
		value += d.costs[i]*s.varsValues[i];
	}
	return value;
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
	double bestUtility = 0;
	int bestVar = 0;
	for (int var = 0; var < d.variableCount; ++var) {
		int rowCount = 0;
		//TODO
		for(int line = 0; line < d.constraintCount; ++line) {
			if(s.satisfiedConstraints[line] == 0) {
				rowCount++;
			}
		}
		double utility = (double) rowCount / d.costs[var];
		if(utility > bestUtility) {
			utility = bestUtility;
			bestVar = var;
		}
	}
	return bestVar;
}

bool isFeasibleSolution(solution &s) {
	for(int i = 0; i < s.satisfiedConstraints.size(); ++i) {
		if(s.satisfiedConstraints[i] == 0) {
			return false;
		}
	}
	return true;
}
solution createInitialSolution(data &d) {
	solution s;
	for(int i = 0; i < d.constraintCount; ++i) {
		s.satisfiedConstraints.push_back(0);
	}

	while(!isFeasibleSolution(s)) {
		int bestVariable = getBestUtilityVariable(d, s);
		s.varsValues[bestVariable] = 1;
		for(int i = 0; i < d.constraintsByVariable[bestVariable].size(); ++i) {
			int constraints = d.constraintsByVariable[bestVariable][i];
			s.satisfiedConstraints[constraints] += 1;
		}
	}
	return s;
}

vector<solution> createNeighborhood(data &d, solution &s, int k) {
	vector<solution> neighborhood;
	if(k == 1) {
		// DROP
		for(int var = 0; var < d.constraintCount; ++var) {
			if(s.varsValues[var] == 1) {
				//DROP possibility
				for(int line = 0; line < d.constraintsLength; ++line) {
					if()
				}
			}

		}
	}
	return neighborhood;
}

solution localSearch(solution &s, int k) {

}
int random_range(int min, int max) {
    return rand() % (max + 1 - min) + min;
}

solution pickRandomSolution(vector<solution> neighborhood) {
	int solutionIndex = random_range(0, neighborhood.size()-1);
	return neighborhood[solutionIndex];
}


int main(int argc, char **argv) {
	srand(time(0));
	string dataFile("scp41.txt");
	data d = readDataFile(dataFile.c_str());
	cout << d.variableCount << " variables, " << d.constraintCount
			<< " constraints" << endl;

	int maxIterations = 100;
	int iterations = 0;
	int k;
	solution currentSolution = createInitialSolution(d);
	while (iterations < maxIterations) {
		iterations++;
		k = 1;
		while (k < 3) {
			vector<solution> neighborhood = createNeighborhood(d,
					currentSolution, k);
			solution randomNeighbor = pickRandomSolution(neighborhood);
			solution localOptimum = localSearch(randomNeighbor, k);
			if (localOptimum.cost < currentSolution.cost) {
				currentSolution = localOptimum;
				k = 1;
			} else {
				k += 1;
			}
		}
	}
}

