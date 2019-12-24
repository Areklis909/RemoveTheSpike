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

int ConfigProcessor::readLambda() {
    return readSetting<int>(lambdaStr);
}

int ConfigProcessor::readLambdaZero() {
    return readSetting<int>(lambdaZeroStr);
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

void ConfigProcessor::writeLambda(const int lambda) {
    writeSetting(lambdaStr, lambda);
}

void ConfigProcessor::writeLambdaZero(const int lambdaZero) {
    writeSetting(lambdaZeroStr, lambdaZero);
}

void ConfigProcessor::commit(const std::string & configName) {
    config.writeFile(configName);
}


}

#endif