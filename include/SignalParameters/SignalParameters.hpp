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
	const int mi;
	const int commonBufferSize;
	const double lambda;
	const double lambdaZero;
	const double equivalentWindowLength;
	std::shared_ptr<double[]> y;
	double wariancjaSzumu;
	double bledyEstymacji;
	arma::vec teta;
	arma::vec previousTeta;
	double previousWariancjaSzumu;
	arma::vec wektorWzmocnien;
	arma::vec fi;
	arma::mat kowBledow;
	arma::mat efektSzerOkna;
	NsModelStability::ModelStability modelStability;
	NsLevinsonDurbin::LevinsonDurbin levinsonDurbin;
	std::fstream f;

	void updateFi(int t);
	void savePreviousParameters();
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
	void loadPreviousParameters();
};

}

#endif

