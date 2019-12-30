#ifndef VARIADIC_KALMAN_CPP
#define VARIADIC_KALMAN_CPP

#include <VariadicKalmanFilter/VariadicKalmanFilter.hpp>

using namespace arma;

namespace NsVariadicKalmanFilter {


VariadicKalmanFilter::VariadicKalmanFilter(const int r, const int maxLengthOfAlarm, const vec & wsp, const int miTmp, std::shared_ptr<double[]> frms) : r(r),
maxLengthOfDamagedBlock(maxLengthOfAlarm),
wspAutoregresji(wsp),
frames(frms),
qMax((2*r)+maxLengthOfDamagedBlock),
mi(miTmp), // 4
xApriori(qMax, fill::zeros),
xAposteriori(qMax, fill::zeros),
PqApriori(qMax, qMax, fill::zeros),
PqAposteriori(qMax, qMax, fill::zeros),
hq(qMax, fill::zeros),
var(1, 1, fill::ones)
{}

VariadicKalmanFilter::~VariadicKalmanFilter() {
}

vec VariadicKalmanFilter::createTheta(const int t, const int q) {
	vec theta(q);
	for(int i = 0; i < q; i++) {
		if(i < r) {
			// (theta)(i) = (wspAutoregresji)(r-i-1); // nie na odwrót?
			(theta)(i) = wspAutoregresji(i);
		} else {
			(theta)(i) = 0;
		}
	}
	return theta;
}
														
void VariadicKalmanFilter::initMatrices(const int t) {
	for(int i = 0; i < r; ++i) {
		(xAposteriori)(i) = (frames)[t - i];
	}
	PqAposteriori.zeros();
}

VariadicKalmanFilter::VarKalStatus VariadicKalmanFilter::updateStateAndCovarianceMatrices(const int t, const int i, const int q, const int currentIndex) {
	auto theta = createTheta(t, q - 1);
	mat y = (theta).t() * (xAposteriori)(currentIndex, 0, size(q - 1, 1));
	double error = (frames)[t + 1 + i] - y(0,0); 
	xApriori.zeros();
	(xApriori)(currentIndex, 0, size(1,1)) = y;
	(xApriori)(currentIndex + 1, 0, size(q - 1, 1)) = (xAposteriori)(currentIndex, 0, size(q - 1, 1));
	(hq)(currentIndex - 1, 0, size(q - 1, 1)) = (PqAposteriori)(currentIndex - 1, currentIndex - 1, size(q - 1, q - 1)) * (theta);
	auto ro = (theta).t() * (hq)(currentIndex - 1, 0, size(q - 1, 1)) + var;
	double roTemp = ro.at(0, 0);
	PqApriori.zeros();
	(PqApriori)(currentIndex, currentIndex) = roTemp;
	(PqApriori)(currentIndex + 1, currentIndex, size(q - 1, 1)) = (hq)(currentIndex - 1, 0, size(q - 1, 1));
	(PqApriori)(currentIndex, currentIndex + 1, size(1, q - 1)) = (hq)(currentIndex - 1, 0, size(q - 1, 1)).t();
	(PqApriori)(currentIndex + 1, currentIndex + 1, size(q - 1, q - 1)) = (PqAposteriori)(currentIndex, currentIndex, size(q - 1, q - 1));
	VarKalStatus status(roTemp, error);
	return status;
}

void VariadicKalmanFilter::aposterioriUpdateDamaged() {
	// std::cout << "Xapos:  "<< xAposteriori.n_cols << " " << xAposteriori.n_rows << '\n';
	// std::cout << "Xaprio:  " << xApriori.n_cols << " " << xApriori.n_rows << '\n';

	// std::cout << "Papos: " << PqAposteriori.n_cols << " " << PqAposteriori.n_rows << '\n';
	// std::cout << "Pprio: " << PqApriori.n_cols << " " << PqApriori.n_rows << '\n';

	(xAposteriori) = (xApriori);
	(PqAposteriori) = (PqApriori);
}

void VariadicKalmanFilter::aposterioriUpdateNotDamaged(const VarKalStatus & status, const int q, const int currentIndex) {
	// auto lq = (1.0/status.ro) * (PqApriori)(currentIndex, currentIndex, size(q, 1));
	// (xAposteriori)(currentIndex, currentIndex, size(q, q)) = (xApriori)(currentIndex, currentIndex, size(q, q)) + lq * status.error;
	// (PqAposteriori)(currentIndex, currentIndex, size(q, q)) = (PqApriori)(currentIndex, currentIndex, size(q, q)) - status.ro * lq * lq.t();
	auto lq = (1.0/status.ro) * PqApriori.col(0);
	xAposteriori = xApriori + lq * status.error;
	PqAposteriori = PqApriori - status.ro * lq * lq.t();
}

int VariadicKalmanFilter::getAlarmLength(const int t) {
	int length = 0;
	int goodSamples = 0;
	int q = r + 1;
	int currentIndex = qMax - q - 1;

	initMatrices(t);

	for(int i = 0; goodSamples < r; ++i, ++q, ++length, --currentIndex) {

		if(length >= maxLengthOfDamagedBlock) break;

		auto status = updateStateAndCovarianceMatrices(t, i, q, currentIndex);
		if(fabs(status.error) > mi * sqrt(status.ro)) {
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

	initMatrices(t);
	for(int i = 0; i < length; ++i, ++q, --currentIndex) {
		updateStateAndCovarianceMatrices(t, i, q, currentIndex);
		aposterioriUpdateDamaged();
	}
}

void VariadicKalmanFilter::pasteTheResult(const AlarmDescriptor & alarm) {
	vec result = arma::reverse(xAposteriori(r - 1, 0, size(alarm.length, 1)));
	memcpy(frames.get() + alarm.startSample, result.memptr(), alarm.length * sizeof(double));
	// std::cout << "arek";
}

int VariadicKalmanFilter::fixDamagedSamples(const int t) {
	// std::cout << "Start fixing" << '\n';
	auto length = getAlarmLength(t);
	// std::cout << "Alarm length " << length <<  '\n';
	AlarmDescriptor descriptor(t, length);
	interpolate(descriptor);
	pasteTheResult(descriptor);

	return length;
}

}

#endif
