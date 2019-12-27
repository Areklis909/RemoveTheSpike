#ifndef SHIFT_BUFFER
#define SHIFT_BUFFER

#include <vector>

namespace NsShiftBuffer {

template<typename BasicType>
class ShiftBuffer {
    
    const int period;
    std::vector<BasicType> buffer;

public:

    template<typename... Args>
    ShiftBuffer(const int p, Args&&... args) : period(p) {
        for(int i = 0; i < period; ++i) {
            buffer.push_back(BasicType(std::forward<Args>(args)...));
        }
    }

    void shift() {
        for(int i = 1; i < period; ++i) {
            buffer[i - 1] = buffer[i];
        }
    }

    BasicType & get(const int momentInThePast) {
        return buffer.at(period - momentInThePast - 1);
    }
};

}
#endif