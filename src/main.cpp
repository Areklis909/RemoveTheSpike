#include <DisturbanceElimination/DisturbanceElimination.hpp>
#include <CommandLineParser/CommandLineParser.hpp>

int main(int argc, char ** argv) {
    using NsDisturbanceElimination::DisturbanceElimination;
    using NsCommandLineParser::CommandLineParser;

    try {
        CommandLineParser parser;
        parser.parseCommandLine(argc, argv);

        const std::string configName = "./Configuration.cfg";
        DisturbanceElimination disturbanceElimination;
        disturbanceElimination.initializeConfiguration(configName);
        disturbanceElimination.adjustConfiguration(parser);
        // disturbanceElimination.printConfiguration();

        auto samples = disturbanceElimination.getSamplesToProcess();
        disturbanceElimination.processSamples(samples);
        disturbanceElimination.saveSamples(samples);
    } catch(std::exception & e) {
        std::cout << e.what();
    }
}
