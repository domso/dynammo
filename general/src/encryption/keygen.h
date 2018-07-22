#pragma once

#include <optional>

#include "src/encryption/private_key.h"
#include "src/encryption/public_key.h"

namespace encryption {
    namespace keygen {
        std::optional<std::pair<encryption::public_key, encryption::private_key>> generate();
    }
}
