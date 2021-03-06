#ifndef DISTURBANCE_HPP
#define DISTURBANCE_HPP

#include <SignalParameters/SignalParameters.hpp>
#include <VariadicKalmanFilter/VariadicKalmanFilter.hpp>
#include <FileHandler/FileHandler.hpp>
#include <ConfigProcessor/ConfigProcessor.hpp>
#include <CommandLineParser/CommandLineParser.hpp>
#include <utils/Counter.hpp>
#include <Signal/Signal.hpp>

namespace NsDisturbanceElimination {

class DisturbanceElimination {
public:

    struct DisturbanceEliminationConfiguration {
        int r;
        int N;
        int ro;
        int startPointOfProcessing;
        int maxAlarmLength;
        int mi;
        // int sampleRate;
        // int numberOfChannels;
        double lambda;
        double lambdaZero;
        std::string soundFileName;
        std::string outputFilePath;

        DisturbanceEliminationConfiguration(const int r, const int N, const int ro, const int maxAlarmLength, const int mi,
         const int startPoint, const double lambda, const double lambdaZero, const std::string & fnm, const std::string & ofnm) : r(r), N(N), ro(ro),
         startPointOfProcessing(startPoint), maxAlarmLength(maxAlarmLength), mi(mi), lambda(lambda), lambdaZero(lambdaZero),
         soundFileName(fnm), outputFilePath(ofnm) {};
        DisturbanceEliminationConfiguration() : r(0), N(0), ro(0), startPointOfProcessing(0),
        maxAlarmLength(50), mi(4), lambda(0.99), lambdaZero(0.98) {};
    };

    DisturbanceElimination();
    DisturbanceElimination(const DisturbanceEliminationConfiguration & dec);
    void initializeConfiguration(const std::string & configName);
    void adjustConfiguration(NsCommandLineParser::CommandLineParser & parser);
    void printConfiguration();
    NsSignal::Signal<double> getSamplesToProcess();
    void processSamples(NsSignal::Signal<double> & signal);
    void saveSamples(NsSignal::Signal<double> & signal);

private:
    std::unique_ptr<NsFileHandler::FileHandler> fileHandler;
    DisturbanceEliminationConfiguration configuration;
};


}


#endif
