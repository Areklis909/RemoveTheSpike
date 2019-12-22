#ifndef COUNTER_HPP
#define COUNTER_HPP

namespace NsCounter {

template<typename T>
class Counter {

    T action;
    uint32_t period;
    const uint32_t ref_period;
    bool enabled;

public:
    Counter(const uint32_t p, T action) : period(p), ref_period(p), action(action), enabled(false) {};

    void enable() {
        enabled = true;
    }

    void disable() {
        enabled = false;
    }

    bool isEnabled() {
        return enabled;
    }

    void reset() {
        period = ref_period;
    }

    void tick() {
        if(enabled == false) return;

        --period;
        if(period == 0) {
            action();
            reset();
            disable();
        }
    }

};

}


#endif