#include <DisturbanceElimination/DisturbanceElimination.hpp>

int main() {
    using NsDisturbanceElimination::DisturbanceElimination;

    const std::string configName = "Configuration.cfg";
    DisturbanceElimination disturbanceElimination;
    try {
        disturbanceElimination.initializeConfiguration(configName);
        auto samples = disturbanceElimination.getSamplesToProcess();
        disturbanceElimination.processSamples(samples);
        disturbanceElimination.saveSamples(samples);
    } catch(std::exception & e) {
        std::cout << e.what();
    }
}