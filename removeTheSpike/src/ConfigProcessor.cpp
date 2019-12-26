#ifndef CONFIG_PROC_CPP
#define CONFIG_PROC_CPP

#include "../include/ConfigProcessor.hpp"

using namespace libconfig;

namespace NsConfigProcessor {

ConfigProcessor::ConfigProcessor(const std::string & configName) {
    config.setOptions(Config::OptionSemicolonSeparators);
    config.readFile(configName.c_str());
}

int ConfigProcessor::readModelOrder() {
    return readSetting<int>(modelOrderStr);
}

int ConfigProcessor::readRo() {
    return readSetting<int>(roStr);
}

int ConfigProcessor::readMaxAlarmLength() {
    return readSetting<int>(maxAlarmLengthStr);
}

int ConfigProcessor::readMi() {
    return readSetting<int>(miStr);
}

double ConfigProcessor::readLambda() {
    return readSetting<double>(lambdaStr);
}

double ConfigProcessor::readLambdaZero() {
    return readSetting<double>(lambdaZeroStr);
}

void ConfigProcessor::writeModelOrder(const int r) {
    writeSetting(modelOrderStr, r);
}

void ConfigProcessor::writeRo(const int ro) {
    writeSetting(roStr, ro);
}

void ConfigProcessor::writeMaxAlarmLength(const int length) {
    writeSetting(maxAlarmLengthStr, length);
}

void ConfigProcessor::writeMi(const int mi) {
    writeSetting(miStr, mi);
}

void ConfigProcessor::writeLambda(const double & lambda) {
    writeSetting(lambdaStr, lambda);
}

void ConfigProcessor::writeLambdaZero(const double & lambdaZero) {
    writeSetting(lambdaZeroStr, lambdaZero);
}

void ConfigProcessor::commit(const std::string & configName) {
    config.writeFile(configName.c_str());
}


}

#endif