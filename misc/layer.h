#ifndef gameServer_data_map_layer_h
#define gameServer_data_map_layer_h

#include <vector>

namespace data {
    namespace map {

        template <typename T>
        class layer {
        public:
            layer() : m_layers(128 * 128) {

            }

            T& operator[](const int index) {
                return m_tiles[index];
            }

            const T& operator[](const int index) const {
                return m_tiles[index];
            }

        private:
            std::vector<T> m_tiles;
        };

    }
}

#endif
