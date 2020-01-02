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
    const std::string sampleRateStr = "sampleRate";
    const std::string numberOfChannelsStr = "numberOfChannels";
    const std::string signalLengthStr = "signalLength";
    const std::string soundFilePathStr = "soundFileName";
    const std::string outputFilePathStr = "outputFilePath";
    const std::string startPointOfProcessingStr = "startPointOfProcessing";

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
    int readSampleRate();
    int readNumberOfChannels();
    int readSignalLength();
    int readStartPointOfProcessing();
    double readLambda();
    double readLambdaZero();
    std::string readSoundFilePath();
    std::string readOutputFilePath();

    void writeModelOrder(const int r);
    void writeRo(const int ro);
    void writeMaxAlarmLength(const int length);
    void writeMi(const int mi);
    void writeSampleRate(const int rate);
    void writeNumberOfChannels(const int numOfChannels);
    void writeSignalLength(const int signalLength);
    void writeLambda(const double & lambda);
    void writeLambdaZero(const double & lambdaZero);

    void commit(const std::string & configName);

};

}

#endif