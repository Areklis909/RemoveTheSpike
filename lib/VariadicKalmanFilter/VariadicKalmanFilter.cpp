#ifndef VARIADIC_KALMAN_CPP
#define VARIADIC_KALMAN_CPP

#include <VariadicKalmanFilter/VariadicKalmanFilter.hpp>

using namespace arma;

namespace NsVariadicKalmanFilter {

VariadicKalmanFilter::VariadicKalmanFilter(const int r, const int maxLengthOfAlarm, const vec & wsp, const int miTmp, std::shared_ptr<double[]> frms,
	 const double & noiseVarianceBeforeAlarm) : 
	r(r),
	maxLengthOfDamagedBlock(maxLengthOfAlarm),
	qMax((2*r)+maxLengthOfDamagedBlock),
	mi(miTmp),
	frames(frms),
	wspAutoregresji(wsp),
	xApriori(qMax, fill::zeros),
	xAposteriori(qMax, fill::zeros),
	PqApriori(qMax, qMax, fill::zeros),
	PqAposteriori(qMax, qMax, fill::zeros),
	hq(qMax, fill::zeros),
	var(1, 1, fill::zeros)
{
	var(0,0) = noiseVarianceBeforeAlarm;
}

VariadicKalmanFilter::~VariadicKalmanFilter() {
}

vec VariadicKalmanFilter::createTheta(const int q) {
	vec theta(q, fill::zeros);
	for(int i = 0; i < r; ++i) {
		(theta)(i) = wspAutoregresji(i);
	}
	return theta;
}
														
void VariadicKalmanFilter::initMatrices(const int t) {
	xAposteriori.zeros();
	PqAposteriori.zeros();
	for(int i = 0; i < r; ++i) {
		(xAposteriori)(i) = (frames)[t - i];
	}
}

VariadicKalmanFilter::VarKalStatus VariadicKalmanFilter::updateStateAndCovarianceMatrices(const int t, const int i, const int q, const int currentIndex) {
	const vec theta = createTheta(q - 1);
	const mat y = (theta).t() * (xAposteriori)(0, 0, size(q - 1, 1));
	const double error = (frames)[t + i] - y(0,0);
	xApriori.zeros();
	(xApriori)(0, 0) = y(0,0);
	(xApriori)(1, 0, size(q - 1, 1)) = (xAposteriori)(0, 0, size(q - 1, 1));
	(hq)(0, 0, size(q - 1, 1)) = (PqAposteriori)(currentIndex + 1, currentIndex + 1, size(q - 1, q - 1)) * (theta);
	const mat roTmp = ((theta).t() * (hq)(0, 0, size(q - 1, 1))) + var;
	const double ro = roTmp.at(0, 0);
	PqApriori.zeros();
	(PqApriori)(currentIndex, currentIndex) = ro;
	(PqApriori)(currentIndex + 1, currentIndex, size(q - 1, 1)) = (hq)(0, 0, size(q - 1, 1));
	(PqApriori)(currentIndex, currentIndex + 1, size(1, q - 1)) = (hq)(0, 0, size(q - 1, 1)).t();
	(PqApriori)(currentIndex + 1, currentIndex + 1, size(q - 1, q - 1)) = (PqAposteriori)(currentIndex + 1, currentIndex + 1, size(q - 1, q - 1));
	VarKalStatus status(ro, error);
	return status;
}

void VariadicKalmanFilter::aposterioriUpdateDamaged() {
	(xAposteriori) = (xApriori);
	(PqAposteriori) = (PqApriori);
}

void VariadicKalmanFilter::aposterioriUpdateNotDamaged(const VarKalStatus & status, const int q, const int currentIndex) {
	const vec lq = PqApriori(currentIndex, currentIndex, size(q, 1))/status.ro;
	xAposteriori(0, 0, size(q, 1)) = xApriori(0, 0, size(q, 1)) + (lq * status.error);
	PqAposteriori(currentIndex, currentIndex, size(q, q)) = PqApriori(currentIndex, currentIndex, size(q, q)) - (status.ro * lq * lq.t());
}

int VariadicKalmanFilter::getAlarmLength(const int t) {
	int length = 1;
	int goodSamples = 0;
	int q = r + 1;
	int currentIndex = qMax - q - 1;

	initMatrices(t - 1);
	for(int i = 0; goodSamples < r; ++i, ++q, ++length, --currentIndex) {
		
		if(length >= maxLengthOfDamagedBlock) break;
		const auto status = updateStateAndCovarianceMatrices(t, i, q, currentIndex);
		if(fabs(status.error) > mi * sqrtf(status.ro)) {
			aposterioriUpdateDamaged();
			goodSamples = 0;
		} else {
			aposterioriUpdateNotDamaged(status, q, currentIndex);
			++goodSamples;
		}
	}

	return length;
}

void VariadicKalmanFilter::interpolate(const AlarmDescriptor & alarm) {
	int t = alarm.startSample;
	int length = alarm.length;
	int q = r + 1;
	int currentIndex = qMax - q - 1;

	initMatrices(t - 1);
	for(int i = 0; i < length; ++i, ++q, --currentIndex) {
		updateStateAndCovarianceMatrices(t, i, q, currentIndex);
		aposterioriUpdateDamaged();
	}

	for(int i = length; i < length + r; ++i, ++q, --currentIndex) {
		const auto status = updateStateAndCovarianceMatrices(t, i, q, currentIndex);
		aposterioriUpdateNotDamaged(status, q, currentIndex);
	}
}

void VariadicKalmanFilter::pasteTheResult(const AlarmDescriptor & alarm) {
	vec result = arma::reverse(xAposteriori(r, 0, size(alarm.length, 1)));
	memcpy(frames.get() + alarm.startSample, result.memptr(), alarm.length * sizeof(double));
}


int VariadicKalmanFilter::fixDamagedSamples(const int t) {
	const auto length = getAlarmLength(t) - r;
	AlarmDescriptor descriptor(t, length);
	interpolate(descriptor);
	pasteTheResult(descriptor);
	return length;
}

}

#endif
