#ifndef LEVINSON_DURBIN_CPP
#define LEVINSON_DURBIN_CPP

#include <LevinsonDurbin/LevinsonDurbin.hpp>

using std::vector;
using NsSwapBuffer::SwapBuffer;
using NsBufferMoment::BufferMoment;
using namespace arma;

namespace NsLevinsonDurbin {


LevinsonDurbin::LevinsonDurbin(const int r, const double windowLength, std::shared_ptr<double[]> signal) : r(r), maxBufferSize(2), R(r, r, fill::zeros), p(r, fill::zeros),
	equivalentWindowLength(windowLength),
	y(signal),
	pk(maxBufferSize, r+1, fill::zeros)
{}


LevinsonDurbin::~LevinsonDurbin(void)
{}

vec LevinsonDurbin::getStableModel() {
	R.zeros();
	p.zeros();

	vec rMatrix = pk.get(BufferMoment::CURRENT)/equivalentWindowLength;

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
	for(int i = 0; i <= r; ++i) {
		double accumulator = 0;
		for(int j = 0; j <= equivalentWindowLength - i - 1; ++j) {
			if(i + j <= t) {
				accumulator += (y)[t - j] * (y)[t - j - i];
			}
		}
		pk.get(BufferMoment::CURRENT)(i) = accumulator;
	}
}

void LevinsonDurbin::updateLevinsonDurbinCoefficientsRecursive(const int t) {
	for(int k = 0; k < r; ++k) {
		double pkTmp = pk.get(BufferMoment::PREVIOUS)(k) + y[t] * y[t - k];
		pk.get(BufferMoment::CURRENT)(k) = pkTmp - (y[t - equivalentWindowLength] * y[t - equivalentWindowLength + k]);
	}
}

void LevinsonDurbin::updateLevinsonDurbinCoefficients(const int t) {
	if(t < equivalentWindowLength) {
		updateLevinsonDurbinCoefficientsClassic(t);
	} else {
		updateLevinsonDurbinCoefficientsRecursive(t);
	}
	pk.swap();
}


}

#endif