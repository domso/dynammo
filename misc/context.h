#ifndef gameServer_data_map_context_h
#define gameServer_data_map_context_h

#include <stdint.h>
#include <vector>
#include "layer.h"

namespace data {
    namespace map {

        class context {
        public:
            context();

            void update();





        private:
            layer<char> m_baseLayer;
            std::vector<layer<char>> m_dynamicLayers;
        };

    }
}


#endif
