#include <vector>

struct AlarmDescriptor {

    int startSample;
    int length;

    AlarmDescriptor(int start, int len) : startSample(start), length(len) {}
};