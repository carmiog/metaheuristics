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

#endif /* SOLUTION_HPP_ */
