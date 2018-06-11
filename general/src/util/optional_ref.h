#pragma once

#include <optional>

namespace util {
    template <typename T>
    using optional_ref = std::optional<std::reference_wrapper<T>>;
}
