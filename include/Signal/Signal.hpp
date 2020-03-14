#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <memory>

namespace NsSignal {

template<typename T>
class Signal {

    std::shared_ptr<T[]> signal;
    int64_t length;
    int32_t channels;

public:

    Signal(T * ptr, const size_t l, const int64_t ch) : length(l), channels(ch) {
        signal = std::shared_ptr<T[]>(ptr, [](T * p){ delete [] p; });
    }

    std::shared_ptr<T[]> getSignal() {
        return signal;
    }

    int64_t getLength() const {
        return length;
    }

    int32_t getNumberOfChannels() const {
        return channels;
    }

};

}

#endif