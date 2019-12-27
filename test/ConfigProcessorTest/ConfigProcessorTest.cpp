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
const double lambdaResult = 0.999;
const double lambdaZeroResult = 0.001;
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
