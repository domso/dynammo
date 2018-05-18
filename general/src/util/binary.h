#ifndef general_util_binary
#define general_util_binary

#include <cstdint>
#include <type_traits>

namespace util {
    namespace binary {
        uint64_t getMSB64(const uint64_t in);
        uint32_t getMSB32(const uint32_t in);

        /* may not work
        template <typename srcT, typename destT>
        destT upper_half(const srcT c) {
            static_assert(sizeof(destT) * 2 == sizeof(srcT));
            static_assert(std::is_integral<srcT>::value);
            static_assert(std::is_integral<destT>::value);

            return (c >> (sizeof(destT) * 8));
        }
        
        template <typename srcT, typename destT>
        destT lower_half(const srcT c) {
            static_assert(sizeof(destT) * 2 == sizeof(srcT));
            static_assert(std::is_integral<srcT>::value);
            static_assert(std::is_integral<destT>::value);

            return (c << (sizeof(destT) * 8)) >> (sizeof(destT) * 8);
        }
        
        template <typename srcT, typename destT>
        destT concat_half(const srcT lower, const srcT upper) {
            static_assert(sizeof(destT) == sizeof(srcT) * 2);
            static_assert(std::is_integral<srcT>::value);
            static_assert(std::is_integral<destT>::value);

            destT upperDest = upper;
            destT lowerDest = lower;
            
            return (upperDest << (sizeof(srcT) * 8)) | (lowerDest & (8 * sizeof(srcT) - 1));
        }
        */
    }
}

#endif
