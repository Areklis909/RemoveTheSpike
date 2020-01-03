#ifndef SIGNAL_PARAMS_CPP
#define SIGNAL_PARAMS_CPP

#include <SignalParameters/SignalParameters.hpp>

using namespace arma;
using NsSwapBuffer::SwapBuffer;
using NsBufferMoment::BufferMoment;


namespace NsSignalparameters {

SignalParameters::SignalParameters(std::shared_ptr<double[]> data, const int order, const int len, const int roZero, const double lbda, const double lbdaZero, const int miTmp) : r(order), N(len),
	ro(roZero), lambda(lbda), lambdaZero(lbdaZero), equivalentWindowLength((1+lambda)/(1-lambda)), y(data), mi(miTmp),
	wektorWzmocnien(order, fill::zeros),
	bledyEstymacji(0.0),
	teta(r, fill::zeros),
	fi(r, fill::zeros),
	kowBledow(r, r, fill::eye),
	wariancjaSzumu(0.0),
	modelStability(r),
	previousTeta(r, fill::zeros),
	previousWariancjaSzumu(0.0),
	levinsonDurbin(r, equivalentWindowLength, y)
{
	kowBledow *= ro;
}


SignalParameters::~SignalParameters(void)
{
}

void SignalParameters::savePreviousParameters() {
	previousTeta = teta;
	previousWariancjaSzumu = wariancjaSzumu;
}

void SignalParameters::loadPreviousParameters() {
	teta = previousTeta;
	wariancjaSzumu = previousWariancjaSzumu;
}

void SignalParameters::computeEwlsAndVariance(const int t) {

	savePreviousParameters();
	updateFi(t);
	auto x = (y)[t] - (fi.t()) * (teta);
	bledyEstymacji = x.at(0,0);
	auto num = (kowBledow) * (fi);
	auto den = lambda + (fi.t() * (kowBledow) * (fi));
	wektorWzmocnien = num/den.at(0,0);
	mat Ir = eye<mat>(r, r);
	kowBledow =  (1.0/lambda) * (Ir - (wektorWzmocnien) * (fi.t())) * (kowBledow);
	teta = teta + ((wektorWzmocnien) * bledyEstymacji);
	// levinsonDurbin.updateLevinsonDurbinCoefficients(t);
	updateWariancjaSzumuRecursive(t);
}

double SignalParameters::getWariancjaSzumu() {
	return wariancjaSzumu;
}

void SignalParameters::updateWariancjaSzumuRecursive(const int t) {
	wariancjaSzumu = wariancjaSzumu * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji, 2));
}

double SignalParameters::getErrorThreshold() {
	return mi * sqrt(getWariancjaSzumu());
}

bool SignalParameters::isAlarm() {
	return fabs(bledyEstymacji) > getErrorThreshold(); 
}

double SignalParameters::getEquivalentWindowLength() const {
	return equivalentWindowLength;
}

vec & SignalParameters::getTeta(const int t) {
	if(modelStability.isModelStable(teta) == false) {
		teta = levinsonDurbin.getStableModel(t);
	}
	return teta;
}

void SignalParameters::updateFi(int t) {
	for(int i = 1; i <= r; ++i) {
		if(t-i < 0) {
			(fi)(i-1) = 0;
		} else {
			(fi)(i-1) =  (y)[t-i];
		}
	}
}

}

#endif