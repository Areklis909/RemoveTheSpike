#ifndef DISTURBANCE_CPP
#define DISTURBANCE_CPP

#include <DisturbanceElimination/DisturbanceElimination.hpp>
#include <functional>

using namespace NsConfigProcessor;

namespace NsDisturbanceElimination {

void DisturbanceElimination::initializeConfiguration(const std::string & configName) {
    ConfigProcessor processor(configName);

    configuration.r = processor.readModelOrder();
    configuration.ro = processor.readRo();
    configuration.mi = processor.readMi();
    configuration.maxAlarmLength = processor.readMaxAlarmLength();
    configuration.lambda = processor.readLambda();
    configuration.lambdaZero = processor.readLambdaZero();
    configuration.N = processor.readSignalLength();
    configuration.soundFileName = processor.readSoundFilePath();
    configuration.outputFilePath = processor.readOutputFilePath();
}

std::shared_ptr<double[]> DisturbanceElimination::getSamplesToProcess() {
    
    using NsFileHandler::FileHandler;
    
    fileHandler = std::make_unique<FileHandler>(configuration.soundFileName);
    return fileHandler->getSignalHandler(configuration.N);
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
    auto counter = std::make_unique<Counter<decltype(counterCallback)>>(configuration.r, counterCallback);
    /*
        Train the AR model for the first r samples - do not accept any alarm
    */
    relaxAfterAlarm = true;
    counter->enable();
    
    for(int t = startPointOfProcessing; t < configuration.N; ++t) {
        signalParameters.computeEwlsAndVariance(t);
        counter->tick();

        if(relaxAfterAlarm == true) continue;

        if(signalParameters.isAlarm() == true) {
            // std::cout << "alarm t: " << t << '\n';
            signalParameters.loadPreviousParameters();
            VariadicKalmanFilter kalman(configuration.r, configuration.maxAlarmLength, signalParameters.getTeta(), configuration.mi, samples, signalParameters.getWariancjaSzumu());
            int alarmLength = kalman.fixDamagedSamples(t);
            if(alarmLength >= configuration.maxAlarmLength) {
                relaxAfterAlarm = true;
                counter->enable();
            }
            t += alarmLength - 1;
        }
    }
}

void DisturbanceElimination::saveSamples(std::shared_ptr<double[]> samples) {
    fileHandler->writeSamples(samples, configuration.N, configuration.outputFilePath);
}


DisturbanceElimination::DisturbanceElimination() {}

}


#endif
