#ifndef util_h
#define util_h


#include <time.h>
#include <stdlib.h>

// pseudo-random unique-key generator
// each key(i) is calculated by A XOR (B + i) (starting with i = 1)
// where A and B are fix pseudo-random numbers
template<typename T>
class unique_m_keygenerator {
public:

    // initialises A and B
    unique_m_keygenerator() {
        srand(time(NULL));
        startKey = rand();
        maskKey = rand();
    }

    // returns next unique key
    T next() {
        maskKey++;
        return startKey ^ maskKey;
    }


    T startKey;
    T maskKey;
};





#endif
