/*
 * data.hpp
 *
 *  Created on: 25 nov. 2014
 *      Author: antoinemarendet
 */

#ifndef DATA_HPP_
#define DATA_HPP_

#include <vector>
#include <map>

typedef struct {
	int constraintCount;
	int variableCount;
	std::vector<int> costs;
	std::vector<int> constraintsLength;
	std::vector<std::vector<int> > constraints;
	std::vector<std::vector<int> > constraintsByVariable;
} data;

#endif /* DATA_HPP_ */
