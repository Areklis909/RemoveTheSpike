#ifndef COMMAND_LINE_PARSER_CPP
#define COMMAND_LINE_PARSER_CPP

#include <CommandLineParser/CommandLineParser.hpp>

namespace NsCommandLineParser {


CommandLineParser::CommandLineParser() : optionsDescription("Allowed options") {
    // produces proxy object with overloaded operator()
    optionsDescription.add_options()
        (helpStr, "Show help message")
        (modelStr, po::value<int>(), "AR model order")
        (roStr, po::value<int>(), "ro")
        (miStr, po::value<int>(), "mi")
        (maxAlarmLengthStr, po::value<int>(), "Max alarm length")
        (lambdaStr, po::value<double>(), "lambda")
        (lambdaZeroStr, po::value<double>(), "lambdaZero")
        (signalLengthStr, po::value<int>(), "signal length")
        (filenameStr, po::value<std::string>(), "File to process")
        (outputFileStr, po::value<std::string>(), "Processed file name")
        (offsetStr, po::value<int>(), "offset in frames");
}

void CommandLineParser::parseCommandLine(int argc, char ** argv) {
    po::store(po::parse_command_line(argc, argv, optionsDescription), variablesMap);
    po::notify(variablesMap);
}


int CommandLineParser::getModelOrder() {
    return getOption<int>(modelStr);
}

int CommandLineParser::getRo() {
    return getOption<int>(roStr);

}

int CommandLineParser::getMi() {
    return getOption<int>(miStr);

}

int CommandLineParser::getMaxAlarmLength() {
    return getOption<int>(maxAlarmLengthStr);

}

double CommandLineParser::getLambda() {
    return getOption<double>(lambdaStr);

}

double CommandLineParser::getLambdaZero() {
    return getOption<double>(lambdaZeroStr);

}

int CommandLineParser::getSignalLength() {
    return getOption<int>(signalLengthStr);

}

int CommandLineParser::getOffset() {
    return getOption<int>(offsetStr);

}

std::string CommandLineParser::getFileNameToProcess() {
    return getOption<std::string>(filenameStr);

}

std::string CommandLineParser::getOutputFile() {
    return getOption<std::string>(outputFileStr);
}

    
bool CommandLineParser::isModelOrderSet() {
    return isOptionSet<int>(modelStr);
}
    
bool CommandLineParser::isRoSet() {
    return isOptionSet<int>(roStr);
}
    
bool CommandLineParser::isMiSet() {
    return isOptionSet<int>(miStr);
}
    
bool CommandLineParser::isMaxAlarmLengthSet() {
    return isOptionSet<int>(maxAlarmLengthStr);
}
    
bool CommandLineParser::isLambdaSet() {
    return isOptionSet<double>(lambdaStr);
}
    
bool CommandLineParser::isLambdaZeroSet() {
    return isOptionSet<double>(lambdaZeroStr);
}
    
bool CommandLineParser::isSignalLengthSet() {
    return isOptionSet<int>(signalLengthStr);
}
    
bool CommandLineParser::isOffsetSet() {
    return isOptionSet<int>(offsetStr);
}
    
bool CommandLineParser::isFileNameToProcessSet() {
    return isOptionSet<std::string>(filenameStr);
}
    
bool CommandLineParser::isOutputFileSet() {
    return isOptionSet<std::string>(outputFileStr);
}


}
#endif