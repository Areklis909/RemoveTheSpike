#include <armadillo>
#include <iostream>

int main() {
	const int sz = 5;
	arma::vec v(sz);
	for(int i = 0; i < sz; ++i) {
		v(i) = i;
	}
	

	double * space = new double[sz];
	double * p = v.memptr();

	memcpy(space, p, sz * sizeof(double));

	for(int i = 0;i < sz; ++i) {
		std::cout << space[i] << " ";
	}
}
