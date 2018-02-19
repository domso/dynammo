#include "src/util/binary.h"

namespace util {
    namespace binary {

        uint64_t getMSB64(uint64_t in) {
            uint64_t current = 1UL << (31);
            current = current >> (16 * (current > in)) << (16 * (current < in));
            current = current >> (8 * (current > in)) << (8 * (current < in));
            current = current >> (4 * (current > in)) << (4 * (current < in));
            current = current >> (2 * (current > in)) << (2 * (current < in));
            current = current >> (1 * (current > in)) << (1 * (current < in));
            current = current >> (current > in);

            return current;
        }

        uint32_t getMSB32(uint32_t in) {
            uint32_t current = 1UL << (15);
            current = current >> (8 * (current > in)) << (8 * (current < in));
            current = current >> (4 * (current > in)) << (4 * (current < in));
            current = current >> (2 * (current > in)) << (2 * (current < in));
            current = current >> (1 * (current > in)) << (1 * (current < in));
            current = current >> (current > in);

            return current;
        }

    }
}
