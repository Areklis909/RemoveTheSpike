#ifndef VARIADIC_KALMAN_HPP
#define VARIADIC_KALMAN_HPP

#include <memory>
#include <armadillo>
#include <algorithm>
#include <iostream>
#include "AlarmDescriptor.hpp"
#include "SignalParameters.hpp"


namespace NsVariadicKalmanFilter {

class VariadicKalmanFilter
{

	struct VarKalStatus {
		double ro;
		double error;
		VarKalStatus(double roTemp, double errorTemp) : ro(roTemp), error(errorTemp) {};
	};
	
	int q; // aktualny rzad modelu
	const int qMax;
	const int maxLengthOfDamagedBlock;
	const int r;
	const int mi;
	arma::vec wspAutoregresji;
	std::shared_ptr<double[]> frames;
	arma::vec xAposteriori;
	arma::vec xApriori;
	arma::mat PqAposteriori;
	arma::mat PqApriori;
	arma::vec hq;
	arma::mat var;

	VariadicKalmanFilter::VarKalStatus updateStateAndCovarianceMatrices(const int t, const int i, const int q, const int currentIndex);
	arma::vec createTheta(const int t, const int q);
	void initMatrices(const int t);
	void aposterioriUpdateDamaged();
	void aposterioriUpdateNotDamaged(const VariadicKalmanFilter::VarKalStatus & status, const int q, const int currentIndex);
	void interpolate(const AlarmDescriptor & alarm);
	void pasteTheResult(const AlarmDescriptor & alarm);
	int getAlarmLength(const int t);

public:
	VariadicKalmanFilter(const int r, const int maxLengthOfAlarm, const arma::vec & wsp, const int miTmp, std::shared_ptr<double[]> frms);
	~VariadicKalmanFilter(void);
	void fixDamagedSamples(const int t);
};

}


#endif
