/*
 * solution.hpp
 *
 *  Created on: 25 nov. 2014
 *      Author: antoinemarendet
 */

#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

typedef struct {
	std::vector<int> varsValues;
	int cost;
	std::vector<int> satisfiedConstraints;
} solution;

solution copySolution(solution s) {
	solution n;
	n.cost = s.cost;
	n.varsValues = std::vector<int>(s.varsValues);
	n.satisfiedConstraints = std::vector<int>(s.satisfiedConstraints);
	return n;
}

#endif /* SOLUTION_HPP_ */
