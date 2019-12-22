#ifndef DISTURBANCE_CPP
#define DISTURBANCE_CPP

#include "../include/DisturbanceElimination.hpp"

namespace NsDisturbanceElimination {

void DisturbanceElimination::initializeConfiguration() {

}

std::shared_ptr<double[]> DisturbanceElimination::getSamplesToProcess(const std::string & filename) {
    using NsFileHandler::FileHandler;
    
    FileHandler fileHandler(filename, configuration.numberOfChannels, configuration.sampleRate);
    return fileHandler.getSignalHandler(configuration.N);
}

void DisturbanceElimination::warmUp(std::shared_ptr<double[]> samples, NsSignalparameters::SignalParameters & signalParameters) {
    
    using NsSignalparameters::SignalParameters;
    
    const int warmUpRequired = configuration.r * 2;
    const int warmUpLength = (configuration.N > warmUpRequired) ? warmUpRequired : configuration.N/2;
    for(int t = 0; t < warmUpLength; ++t) {
        signalParameters.computeEwlsAndVariance(t);
    }
}

void DisturbanceElimination::processSamples(std::shared_ptr<double[]> samples) {
    using NsSignalparameters::SignalParameters;
    using NsVariadicKalmanFilter::VariadicKalmanFilter;

    SignalParameters signalParameters(samples, configuration.r, configuration.N, configuration.ro, configuration.lambda, configuration.mi);

    warmUp(samples, signalParameters);

    for(int t = 0; t < configuration.N; ++t) {
        signalParameters.computeEwlsAndVariance(t);
        if(signalParameters.isAlarm()) {
            VariadicKalmanFilter kalman(configuration.r, configuration.maxAlarmLength, signalParameters.getTeta(), configuration.mi, samples);
            kalman.fixDamagedSamples(t);
        }
    }
}


DisturbanceElimination::DisturbanceElimination(const DisturbanceEliminationConfiguration & config) : configuration(config) {
}

}


#endif
