#ifndef SIGNAL_PARAMS_CPP
#define SIGNAL_PARAMS_CPP

#include "../include/SignalParameters.hpp"

using namespace arma;
using NsSwapBuffer::SwapBuffer;
using NsBufferMoment::BufferMoment;


namespace NsSignalparameters {

SignalParameters::SignalParameters(std::shared_ptr<double[]> data, const int order, const int len, const int roZero, const double lbda, const int miTmp) : r(order), N(len),
	ro(roZero), lambda(lbda), lambdaZero(0.98), equivalentWindowLength((1+lambda)/(1-lambda)), y(data), mi(miTmp), commonBufferSize(2),
	wektorWzmocnien(order, fill::zeros),
	bledyEstymacji(0.0),
	teta(commonBufferSize, r, fill::zeros),
	fi(r, fill::zeros),
	kowBledow(r, r, fill::ones),
	wariancjaSzumu(commonBufferSize, 0.0),
	modelStability(r),
	levinsonDurbin(r, equivalentWindowLength, y)
{
	kowBledow *= ro;
}


SignalParameters::~SignalParameters(void)
{
}

void SignalParameters::computeEwlsAndVariance(const int t) {

	updateFi(t);
	auto x = (y)[t] - (fi.t()) * (teta.get(BufferMoment::PREVIOUS));
	bledyEstymacji = x.at(0,0);
	auto num = (kowBledow) * (fi);
	auto den = lambda + (fi.t() * (kowBledow) * (fi));
	wektorWzmocnien = num/den.at(0,0);
	mat Ir = eye<mat>(r, r);	
	kowBledow =  (1/lambda) * (Ir - (wektorWzmocnien) * (fi.t())) * (kowBledow);
	teta.get(BufferMoment::CURRENT) = teta.get(BufferMoment::PREVIOUS) + ((wektorWzmocnien) * bledyEstymacji);
	levinsonDurbin.updateLevinsonDurbinCoefficients(t);
	if(modelStability.isModelStable(teta.get(BufferMoment::CURRENT)) == false) {
		teta.get(BufferMoment::CURRENT) = levinsonDurbin.getStableModel();
	}

	teta.swap();
	updateWariancjaSzumuRecursive(t);
}

double SignalParameters::getWariancjaSzumu() {
	return (wariancjaSzumu).get(BufferMoment::CURRENT);
}

void SignalParameters::updateWariancjaSzumuRecursive(const int t) {
	wariancjaSzumu.get(BufferMoment::CURRENT) = wariancjaSzumu.get(BufferMoment::PREVIOUS) * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji, 2));
	wariancjaSzumu.swap();
}

double SignalParameters::getErrorThreshold() {
	return mi * sqrt(getWariancjaSzumu());
}

bool SignalParameters::isAlarm() {
	return abs(bledyEstymacji) > getErrorThreshold(); 
}

double SignalParameters::getEquivalentWindowLength() const {
	return equivalentWindowLength;
}

vec & SignalParameters::getTeta(void) {
	return teta.get(BufferMoment::CURRENT);
}

void SignalParameters::updateFi(int t) {
	for(int i = 1; i <= r; i++) {
		if(t-i < 0) {
			(fi)(i-1) = 0;
		} else {
			(fi)(i-1) =  (y)[t-i];
		}
	}
}

}

#endif