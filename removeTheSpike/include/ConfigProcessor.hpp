#ifndef CONFIG_PROC_HPP
#define CONFIG_PROC_HPP

#include <libconfig.h++>

namespace NsConfigProcessor {

class ConfigProcessor {

    libconfig::Config config;
    const std::string modelOrderStr = "modelOrder";
    const std::string roStr = "ro";
    const std::string maxAlarmLengthStr = "maxAlarmLength";
    const std::string miStr = "mi";
    const std::string lambdaStr = "lambda";
    const std::string lambdaZeroStr = "lambdaZero";

    template<typename T>
    T readSetting(const std::string & settingName) {
        return config.lookup(settingName);
    }

    template<typename T>
    void writeSetting(const std::string & settingName, const T & settingValue) {
        libconfig::Setting & root = config.getRoot();
        libconfig::Setting::Type type = libconfig::Setting::TypeString;
        if constexpr (std::is_same_v<T, std::string>) {
            type = libconfig::Setting::TypeString;
        } else if constexpr (std::is_same_v<T, int>) {
            type = libconfig::Setting::TypeInt;
        } else if constexpr (std::is_same_v<T, double>) {
            type = libconfig::Setting::TypeFloat;
        }

        root.add(settingName, type) = settingValue;
    }

public:
    ConfigProcessor(const std::string & configName);

    int readModelOrder();
    int readRo();
    int readMaxAlarmLength();
    int readMi();
    int readLambda();
    int readLambdaZero();

    void writeModelOrder(const int r);
    void writeRo(const int ro);
    void writeMaxAlarmLength(const int length);
    void writeMi(const int mi);
    void writeLambda(const int lambda);
    void writeLambdaZero(const int lambdaZero);

    void commit(const std::string & configName);

};

}

#endif