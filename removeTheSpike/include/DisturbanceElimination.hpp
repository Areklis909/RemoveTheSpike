#ifndef DISTURBANCE_HPP
#define DISTURBANCE_HPP

#include "SignalParameters.hpp"
#include "VariadicKalmanFilter.hpp"
#include "FileHandler.hpp"

namespace NsDisturbanceElimination {

class DisturbanceElimination {
public:

    struct DisturbanceEliminationConfiguration {
        const int r;
        const int N;
        const int ro;
        const int maxAlarmLength;
        const int mi;
        const int sampleRate;
        const int numberOfChannels;
        const double lambda;
        const double lambdaZero;

        DisturbanceEliminationConfiguration(const int r, const int N, const int ro, const int maxAlarmLength, const int mi,
        const int sampleRate, const int numberOfChannels, const double lambda, const double lambdaZero) : r(r), N(N), ro(ro),
        maxAlarmLength(maxAlarmLength), mi(mi), sampleRate(sampleRate), numberOfChannels(numberOfChannels), lambda(lambda), lambdaZero(lambda) {};
    };

    DisturbanceElimination(const DisturbanceEliminationConfiguration & configuration);

private:
    DisturbanceEliminationConfiguration configuration;

    void initializeConfiguration(); // confiuguration file parsing to do
    std::shared_ptr<double[]> getSamplesToProcess(const std::string & filename);
    void warmUp(std::shared_ptr<double[]> samples, NsSignalparameters::SignalParameters & signalParameters);
    void processSamples(std::shared_ptr<double[]> samples);
    void saveSamples(std::shared_ptr<double[]> samples);

};


}


#endif
