#ifndef CONFIG_PROC_HPP
#define CONFIG_PROC_HPP

#include <libconfig.h++>

namespace NsConfigProcessor {

class ConfigProcessor {

    libconfig::Config config;

    template<typename T>
    T readSetting(const std::string & settingName);

    template<typename T>
    void writeSetting(const std::string & settingName, const T & settingValue);

public:
    ConfigProcessor(const std::string & configName);

    int readModelOrder();
    int readRo();
    int readMaxAlarmLength();
    int readMi();
    int readLambda();
    int readLambdaZero();

    void writeModelOrder();
    void writeRo();
    void writeMaxAlarmLength();
    void writeMi();
    void writeLambda();
    void writeLambdaZero();

};

}

#endif