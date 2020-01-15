#ifndef LEVINSON_DURBIN_CPP
#define LEVINSON_DURBIN_CPP

#include <LevinsonDurbin/LevinsonDurbin.hpp>

using std::vector;
using namespace arma;

namespace NsLevinsonDurbin {


LevinsonDurbin::LevinsonDurbin(const int r, const double windowLength, std::shared_ptr<double[]> signal) : 
	r(r),
	equivalentWindowLength(windowLength),
	R(r, r, fill::zeros),
	p(r, fill::zeros),
	pk(r+1, fill::zeros),
	y(signal)
{}


LevinsonDurbin::~LevinsonDurbin(void)
{}

vec LevinsonDurbin::getStableModel(const int t) {

	R.zeros();
	p.zeros();

	updateLevinsonDurbinCoefficientsClassic(t);

	vec rMatrix = pk/equivalentWindowLength;

	for(int i = 0; i < r; ++i) {
		R.diag(i).fill(rMatrix(i));
		R.diag(-i).fill(rMatrix(i));
	}

	for(int i = 1; i <= r; ++i) {
		p(i-1) = rMatrix(i);
	}

	return inv(R)*p;
}

void LevinsonDurbin::updateLevinsonDurbinCoefficientsClassic(const int t) {
	for(int k = 0; k <= r; ++k) {
		double accumulator = 0;
		for(int i = 0; i < equivalentWindowLength - k - 1; ++i) {
			double arg1 = 0;
			double arg2 = 0;
			if(t - i >= 0) {
				arg1 = y[t - i];
			}

			if(t - i - k >= 0) {
				arg2 = y[t - i - k];
			}

			accumulator += arg1 * arg2;
		}
		pk(k) = accumulator;
	}
}

void LevinsonDurbin::updateLevinsonDurbinCoefficientsRecursive(const int t) {

	for(int k = 0; k <= r; ++k) {
		double arg1 = 0;
		double arg2 = 0;
		if(t - k >= 0) {
			arg1 = y[t - k];
		}
		double pkTmp = pk(k) + (y[t] * arg1);

		arg1 = 0;
		if(t - equivalentWindowLength >= 0) {
			arg1 = y[t - equivalentWindowLength];
		}

		if(t - equivalentWindowLength + k >= 0) {
			arg2 =  y[t - equivalentWindowLength + k];
		}
		pk(k) = pkTmp - (arg1 * arg2);
	}
}

void LevinsonDurbin::updateLevinsonDurbinCoefficients(const int t) {
		updateLevinsonDurbinCoefficientsRecursive(t);
}

}

#endif