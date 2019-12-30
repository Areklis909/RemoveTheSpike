#ifndef CONFIG_PROC_CPP
#define CONFIG_PROC_CPP

#include <ConfigProcessor/ConfigProcessor.hpp>

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

int ConfigProcessor::readSignalLength() {
    return readSetting<int>(signalLengthStr);
}

int ConfigProcessor::readSampleRate() {
    return readSetting<int>(sampleRateStr);
}

int ConfigProcessor::readNumberOfChannels() {
    return readSetting<int>(numberOfChannelsStr);
}


double ConfigProcessor::readLambda() {
    return readSetting<double>(lambdaStr);
}

double ConfigProcessor::readLambdaZero() {
    return readSetting<double>(lambdaZeroStr);
}

std::string ConfigProcessor::readSoundFilePath() {
    return readSetting<std::string>(soundFilePathStr);
}

std::string ConfigProcessor::readOutputFilePath() {
    return readSetting<std::string>(outputFilePathStr);
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

void ConfigProcessor::writeSampleRate(const int sampleRate) {
    writeSetting(sampleRateStr, sampleRate);
}

void ConfigProcessor::writeNumberOfChannels(const int numOfChannels) {
    writeSetting(numberOfChannelsStr, numOfChannels);
}

void ConfigProcessor::writeSignalLength(const int length) {
    writeSetting(signalLengthStr, length);
}

void ConfigProcessor::commit(const std::string & configName) {
    config.writeFile(configName.c_str());
}


}

#endif