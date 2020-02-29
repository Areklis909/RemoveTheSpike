#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

namespace NsCommandLineParser {

class CommandLineParser {
    po::options_description optionsDescription;
    po::variables_map variablesMap;

    const char * helpStr = "help";
    const char * modelStr = "model";
    const char * roStr = "ro";
    const char * miStr = "mi";
    const char * maxAlarmLengthStr = "maxAlarmLength";
    const char * lambdaStr = "lambda";
    const char * lambdaZeroStr = "lambdaZero";
    const char * signalLengthStr = "signalLength";
    const char * filenameStr = "filename";
    const char * outputFileStr = "outputFile";
    const char * offsetStr = "offset";
    const char * configStr = "config";

    template<typename T>
    T getOption(const std::string & optionName) {
        return variablesMap.at(optionName.data()).as<T>();
    }

    template<typename T>
    bool isOptionSet(const std::string & optionName) {
        if(variablesMap.count(optionName)) {
            return true;
        }
        return false;
    }

public:
    CommandLineParser();
    void parseCommandLine(int argc, char ** argv);

    int getModelOrder();
    int getRo();
    int getMi();
    int getMaxAlarmLength();
    double getLambda();
    double getLambdaZero();
    int getSignalLength();
    int getOffset();
    std::string getFileNameToProcess();
    std::string getOutputFile();
    std::string getConfigurationFilePath();

    bool isModelOrderSet();
    bool isRoSet();
    bool isMiSet();
    bool isMaxAlarmLengthSet();
    bool isLambdaSet();
    bool isLambdaZeroSet();
    bool isSignalLengthSet();
    bool isOffsetSet();
    bool isFileNameToProcessSet();
    bool isOutputFileSet();
    bool isConfigurationFilePathSet();

};

}

#endif
