#ifndef SIGNAL_PARAMS_HPP
#define SIGNAL_PARAMS_HPP

#include <armadillo>
#include <iostream>
#include <memory>
#include <sndfile.h>
#include <LevinsonDurbin/LevinsonDurbin.hpp>
#include <ModelStability/ModelStability.hpp>
#include <utils/SwapBuffer.hpp>
#include <fstream>


namespace NsSignalparameters {

class SignalParameters
{
	const int r;
	const int N;
	const int ro;
	const double lambda;
	const double lambdaZero;
	const double equivalentWindowLength;
	std::shared_ptr<double[]> y;
	const int mi;
	arma::vec wektorWzmocnien;
	double bledyEstymacji;
	arma::vec teta;
	arma::vec fi;
	arma::mat kowBledow;
	double wariancjaSzumu;
	arma::vec previousTeta;
	double previousWariancjaSzumu;
	NsModelStability::ModelStability modelStability;
	NsLevinsonDurbin::LevinsonDurbin levinsonDurbin;

	void updateFi(int t);
	void savePreviousParameters();
public:
	SignalParameters(std::shared_ptr<double[]> ptr, const int r, const int N, const int ro, const double lambda, const double lambdaZero, const int miTmp);
	~SignalParameters(void);
	void computeEwlsAndVariance(const int t);
	arma::vec & getTeta(const int t);
	double getWariancjaSzumu();
	void updateWariancjaSzumuRecursive(const int t);
	double getErrorThreshold();
	double getEquivalentWindowLength() const;
	bool isAlarm();
	void loadPreviousParameters();
};

}

#endif

