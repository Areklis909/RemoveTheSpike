#include <gtest/gtest.h>
#include <iostream>
#include <ConfigProcessor/ConfigProcessor.hpp>

using namespace NsConfigProcessor;

template<typename T>
void print(const T & v) {
    std::cout << v << '\n';
}

const int modelOrder = 123;
const int roResult = 4;
const int maxAlarmLen = 50;
const int miResult = 4;
const int rateResult = 44100;
const int channelsResult = 2;
const int lengthResult = 1000;
const double lambdaResult = 0.999;
const double lambdaZeroResult = 0.001;
const std::string pathResult = "blabla";
const std::string configName = "TestConfig.cfg";

TEST(ConfigProcessorTest, modelOrderTest) {
    ConfigProcessor proc(configName);

    auto r = proc.readModelOrder();
    ASSERT_EQ(r, modelOrder);

}

TEST(ConfigProcessorTest, roTest) {
    ConfigProcessor proc(configName);

    auto ro = proc.readRo(); 
    ASSERT_EQ(ro, roResult);

}

TEST(ConfigProcessorTest, lenTest) {
    ConfigProcessor proc(configName);

    auto len = proc.readMaxAlarmLength();
    ASSERT_EQ(maxAlarmLen, len);

}

TEST(ConfigProcessorTest, miTest) {
    ConfigProcessor proc(configName);

    auto mi = proc.readMi();
    ASSERT_EQ(mi, miResult);

}

TEST(ConfigProcessorTest, lambdaTest) {
    ConfigProcessor proc(configName);

    auto lambda = proc.readLambda();
    ASSERT_EQ(lambda, lambdaResult);
}

TEST(ConfigProcessorTest, lambdaZeroTest) {
    ConfigProcessor proc(configName);

    auto lambdaZero = proc.readLambdaZero();
    ASSERT_EQ(lambdaZero, lambdaZeroResult);
}

TEST(ConfigProcessorTest, sampleRateTest) {
    ConfigProcessor proc(configName);

    auto rate = proc.readSampleRate();
    ASSERT_EQ(rate, rateResult);
}

TEST(ConfigProcessorTest, numberOfChannelsTest) {
    ConfigProcessor proc(configName);

    auto channels = proc.readNumberOfChannels();
    ASSERT_EQ(channels, channelsResult);
}

TEST(ConfigProcessorTest, signalLengthTest) {
    ConfigProcessor proc(configName);

    auto length = proc.readSignalLength();
    ASSERT_EQ(length, lengthResult);
}

TEST(ConfigProcessorTest, pathTest) {
    ConfigProcessor proc(configName);

    auto path = proc.readSoundFilePath();
    ASSERT_EQ(path, pathResult);
}



