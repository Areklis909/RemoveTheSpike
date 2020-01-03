#ifndef DISTURBANCE_CPP
#define DISTURBANCE_CPP

#include <functional>
#include <DisturbanceElimination/DisturbanceElimination.hpp>

using namespace NsConfigProcessor;
using NsSignal::Signal;

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
    configuration.startPointOfProcessing = processor.readStartPointOfProcessing();
}

 Signal<double> DisturbanceElimination::getSamplesToProcess() {
    
    using NsFileHandler::FileHandler;
    
    fileHandler = std::make_unique<FileHandler>(configuration.soundFileName);
    return fileHandler->getSignalHandler(configuration.N);
}

void DisturbanceElimination::processSamples(NsSignal::Signal<double> & signal) {

    using NsSignalparameters::SignalParameters;
    using NsVariadicKalmanFilter::VariadicKalmanFilter;
    using NsCounter::Counter;

    SignalParameters signalParameters(signal.getSignal(), configuration.r, signal.getLength(), configuration.ro, configuration.lambda, configuration.lambdaZero, configuration.mi);
    bool relaxAfterAlarm = false;
    auto counterCallback =  [&relaxAfterAlarm](){
        relaxAfterAlarm = false;
    };
    const int delayAfterLongAlarmInSamples = 2 * configuration.r;
    auto counter = std::make_unique<Counter<decltype(counterCallback)>>(delayAfterLongAlarmInSamples, counterCallback);
    /*
        Train the AR model for the first r samples - do not accept any alarm
    */
    relaxAfterAlarm = true;
    counter->enable();
    
    for(int t = configuration.startPointOfProcessing; t < signal.getLength(); ++t) {
        signalParameters.computeEwlsAndVariance(t);
        counter->tick();

        if(relaxAfterAlarm == true) continue;

        if(signalParameters.isAlarm() == true) {
            // std::cout << "alarm t: " << t << '\n';
            signalParameters.loadPreviousParameters();
            VariadicKalmanFilter kalman(configuration.r, configuration.maxAlarmLength, signalParameters.getTeta(t), configuration.mi, signal.getSignal(), signalParameters.getWariancjaSzumu());
            int alarmLength = kalman.fixDamagedSamples(t);
            if(alarmLength >= configuration.maxAlarmLength) {
                relaxAfterAlarm = true;
                counter->enable();
            }
            t += alarmLength - 1;
        }
    }
}

void DisturbanceElimination::saveSamples(NsSignal::Signal<double> & signal) {
    fileHandler->writeSamples(signal, configuration.outputFilePath);
}


DisturbanceElimination::DisturbanceElimination() {}

}


#endif
