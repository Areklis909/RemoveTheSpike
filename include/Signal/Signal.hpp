#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <memory>

namespace NsSignal {

template<typename T>
class Signal {

    std::shared_ptr<T[]> signal;
    int64_t length;

public:

    Signal(T * ptr, size_t l) {
        signal = std::shared_ptr<T[]>(ptr, [](T * p){ delete [] p; });
        length = l;
    }

    std::shared_ptr<T[]> getSignal() {
        return signal;
    }

    int64_t getLength() {
        return length;
    }

};

}

#endif