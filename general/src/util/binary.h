#ifndef general_util_binary
#define general_util_binary

#include <cstdint>

namespace util {
    namespace binary {
        //______________________________________________________________________________________________________
        //
        // Description:
        // - Calculates the most significant bit of a given 64 Bit word
        // Parameter:
        // - 'in': input variable
        // Return:
        // - a word containing only the MSB of 'in'
        //______________________________________________________________________________________________________
        uint64_t getMSB64(uint64_t in);
        //______________________________________________________________________________________________________
        //
        // Description:
        // - Calculates the most significant bit of a given 32 Bit word
        // Parameter:
        // - 'in': input variable
        // Return:
        // - a word containing only the MSB of 'in'
        //______________________________________________________________________________________________________
        uint32_t getMSB32(uint32_t in);
    }
}

#endif
