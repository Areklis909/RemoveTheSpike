#ifndef LEVINSON_DURBIN
#define LEVINSON_DURBIN

#include<memory>
#include<armadillo>
#include "utils/SwapBuffer.hpp"

namespace NsLevinsonDurbin {

class LevinsonDurbin
{
	const int r;
	const double equivalentWindowLength;
	const int maxBufferSize;
	arma::mat R;
	arma::vec p;
	NsSwapBuffer::SwapBuffer<arma::vec> pk;
	std::shared_ptr<double[]> y;
	
	void updateLevinsonDurbinCoefficientsClassic(const int t);
	void updateLevinsonDurbinCoefficientsRecursive(const int t);

public:
	LevinsonDurbin(const int r, const double equivalentWindowLength, std::shared_ptr<double[]> signal);
	~LevinsonDurbin(void);
	void updateLevinsonDurbinCoefficients(const int t);
	arma::vec getStableModel();
};

}

#endif
