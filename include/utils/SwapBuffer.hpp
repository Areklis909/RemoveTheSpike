#ifndef SWAP_BUFFER_HPP
#define SWAP_BUFFER_HPP

#include "BufferMoment.hpp"

namespace NsSwapBuffer {

template<typename T>
class SwapBuffer {

    std::vector<T> buffer;
    uint8_t currentOffset;
    int size;

    int getPreviousFromCurrent() {
        return currentOffset == 1 ?  0 : 1;
    }
    
public:
    
    template<typename ...Args>
    SwapBuffer(int bufferSize, Args &&...args) : currentOffset(0), size(bufferSize) {
        for(int i = 0; i < bufferSize; ++i) {
            T tmp(std::forward<Args>(args)...);
            buffer.push_back(std::move(tmp));
        }

    }

    T & get(const NsBufferMoment::BufferMoment moment) {
        using NsBufferMoment::BufferMoment;

        uint8_t retIndex = currentOffset;
        if(moment == BufferMoment::PREVIOUS) {
            retIndex = getPreviousFromCurrent();
        }

        return buffer.at(retIndex);
    }


    void swap() {
        ++currentOffset;
        currentOffset = currentOffset % size;
    }
};


}


#endif