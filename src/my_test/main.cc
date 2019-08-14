


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <coin/ClpSimplex.hpp>

int main(int argc, char ** argv) {

	ClpSimplex simplex;
	simplex.readMps(argv[1]);
	simplex.initialSolve();


	std::vector<double> obj(simplex.getNumCols());
	std::vector<char> sense(simplex.getNumRows());
	std::vector<double> row_lower(simplex.getNumRows());
	std::vector<double> row_upper(simplex.getNumRows());

	std::ifstream file("problem_3");
	std::string line;
	int index;
	char c;
	double v;
	for(int i(0); i<obj.size(); ++i){
		file >> index;
		file >> obj[i];
		if (i != index) {
			std::cout << "ERROR i != index, " << i << ", " << index << std::endl;
		}
	}
	for (int i(0); i < sense.size(); ++i) {
		file >> index;
		file >> c;
		file >> v;
		if (c == '=') {
			row_lower[index] = v;
			row_upper[index] = v;
		}
		else if (c == '<') {
			row_lower[index] = -DBL_MAX;
			row_upper[index] = v;
		}
		else if (c == '>') {
			row_lower[index] = v;
			row_upper[index] = +DBL_MAX;
		}
	}
	file.close();

	simplex.chgObjCoefficients(obj.data());
	simplex.chgRowLower(row_lower.data());
	simplex.chgRowUpper(row_upper.data());
	std::cout << "Resolving" << std::endl;
	//simplex.primal();
	simplex.initialSolve();

	return 0;
}