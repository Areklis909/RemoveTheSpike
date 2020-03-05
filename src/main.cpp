#include <DisturbanceElimination/DisturbanceElimination.hpp>
#include <CommandLineParser/CommandLineParser.hpp>

int main(int argc, char ** argv) {
    using NsDisturbanceElimination::DisturbanceElimination;
    using NsCommandLineParser::CommandLineParser;

    try {
        CommandLineParser parser;
        parser.parseCommandLine(argc, argv);

        std::string configName;
        if(parser.isConfigurationFilePathSet() == true) {
            configName = parser.getConfigurationFilePath();
        } else {
            configName = "Configuration.cfg";
        }
        DisturbanceElimination disturbanceElimination;
        disturbanceElimination.initializeConfiguration(configName);
        disturbanceElimination.adjustConfiguration(parser);

        auto samples = disturbanceElimination.getSamplesToProcess();
        disturbanceElimination.processSamples(samples);
        disturbanceElimination.saveSamples(samples);
    } catch(std::exception & e) {
        std::cout << e.what();
    }
}
