#ifndef OBJECT_GUARD_HPP
#define OBJECT_GUARD_HPP

namespace NsObjectGuard {

template<typename Callable>
class ObjectGuard {

    Callable callable;
public:
    ObjectGuard(Callable && c) : callable(std::forward<Callable>(c)) {};
    ~ObjectGuard() {
        callable();
    };
};

}

#endif