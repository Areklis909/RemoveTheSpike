#ifndef MODEL_STABILITY_CPP
#define MODEL_STABILITY_CPP

#include <ModelStability/ModelStability.hpp>

using namespace arma;

namespace NsModelStability {

ModelStability::ModelStability(const int r) : r(r), stabilityMatrix(r, r, fill::zeros) {}

bool ModelStability::isModelStable(const vec & model) {
	stabilityMatrix.zeros();
	// std::cout << "update MS" << '\n';
	for(int i = 0; i < r; ++i) {
		stabilityMatrix(r-1, i) = model(i) * (-1);
	}

	for(int k = r-1; k >= 1; --k) {
		for(int i = 1; i <= k; ++i) {
			stabilityMatrix(k - 1,i - 1) = (stabilityMatrix(k, i-1) - stabilityMatrix(k, k) * stabilityMatrix(k, k-i))/(1-pow(stabilityMatrix(k, k), 2));
		}
	}
	arma::vec mainDiagonal = stabilityMatrix.diag(0);
	for(const auto & elem : mainDiagonal) {
		if(abs(elem) >= 1) return false;
	}
	return true;
}

}

#endif