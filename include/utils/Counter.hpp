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
    Counter(const uint32_t p, T action) : action(action), period(p), ref_period(p) , enabled(false) {};

    void enable() noexcept {
        enabled = true;
    }

    void disable() noexcept {
        enabled = false;
    }

    bool isEnabled() const {
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