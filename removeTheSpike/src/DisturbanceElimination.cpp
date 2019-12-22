#ifndef DISTURBANCE_CPP
#define DISTURBANCE_CPP

#include "../include/DisturbanceElimination.hpp"
#include <functional>

namespace NsDisturbanceElimination {

void DisturbanceElimination::initializeConfiguration() {

}

std::shared_ptr<double[]> DisturbanceElimination::getSamplesToProcess(const std::string & filename) {
    using NsFileHandler::FileHandler;
    
    FileHandler fileHandler(filename, configuration.numberOfChannels, configuration.sampleRate);
    return fileHandler.getSignalHandler(configuration.N);
}

void DisturbanceElimination::processSamples(std::shared_ptr<double[]> samples) {

    using NsSignalparameters::SignalParameters;
    using NsVariadicKalmanFilter::VariadicKalmanFilter;
    using NsCounter::Counter;

    SignalParameters signalParameters(samples, configuration.r, configuration.N, configuration.ro, configuration.lambda, configuration.mi);
    const int startPointOfProcessing = 0;
    bool relaxAfterAlarm = false;
    auto counterCallback =  [&relaxAfterAlarm](){
        relaxAfterAlarm = false;
    };
    auto counter = std::make_unique<Counter<decltype(counterCallback)> >(configuration.r, counterCallback);
    /*
        Train the AR model for the first r samples
    */
    counter->enable();
    for(int t = startPointOfProcessing; t < configuration.N; ++t) {
        signalParameters.computeEwlsAndVariance(t);
        counter->tick();
        if(signalParameters.isAlarm() == true && relaxAfterAlarm == false) {
            VariadicKalmanFilter kalman(configuration.r, configuration.maxAlarmLength, signalParameters.getTeta(), configuration.mi, samples);
            int alarmLength = kalman.fixDamagedSamples(t);
            if(alarmLength >= configuration.maxAlarmLength) {
                relaxAfterAlarm = true;
                counter->enable();
            }
        }
    }
}


DisturbanceElimination::DisturbanceElimination(const DisturbanceEliminationConfiguration & config) : configuration(config) {
}

}


#endif
