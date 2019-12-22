#ifndef SIGNAL_PARAMS_HPP
#define SIGNAL_PARAMS_HPP

#include <armadillo>
#include <iostream>
#include <memory>
#include <sndfile.h>
#include "LevinsonDurbin.hpp"
#include "ModelStability.hpp"
#include "utils/SwapBuffer.hpp"

namespace NsSignalparameters {

class SignalParameters
{
	const int r;
	const int N;
	const int ro;
	const int mi;
	const int commonBufferSize;
	const double lambda;
	const double lambdaZero;
	std::shared_ptr<double[]> y;
	const double equivalentWindowLength;
	arma::vec wektorWzmocnien;
	double bledyEstymacji;
	NsSwapBuffer::SwapBuffer<arma::vec> teta;
	NsSwapBuffer::SwapBuffer<double> wariancjaSzumu;
	arma::vec fi;
	arma::mat kowBledow;
	arma::mat efektSzerOkna;
	NsModelStability::ModelStability modelStability;
	NsLevinsonDurbin::LevinsonDurbin levinsonDurbin;

	void updateFi(int t);
public:
	SignalParameters(std::shared_ptr<double[]> ptr, const int r, const int N, const int ro, const double lambda, const int miTmp);
	~SignalParameters(void);
	void computeEwlsAndVariance(const int t);
	arma::vec & getTeta(void);
	double getWariancjaSzumu();
	void updateWariancjaSzumuRecursive(const int t);
	double getErrorThreshold();
	double getEquivalentWindowLength() const;
	bool isAlarm();
};

}

#endif

