#ifndef general_region_layer_h
#define general_region_layer_h

#include <cassert>

namespace region {
    template <typename tileT>
    class layer {
    public:        
        const tileT& operator[](const int n) const {
            assert(n < size);
            return m_tiles[n];
        }
        
        tileT& operator[](const int n) {
            assert(n < size);
            return m_tiles[n];
        }
        
        const tileT& get(const int x, const int y) const {
            assert(x < width && y < height && x >= 0 && y >= 0);
            return m_tiles[y * width + x];
        }
        
        tileT& get(const int x, const int y) {
            assert(x < width && y < height && x >= 0 && y >= 0);
            return m_tiles[y * width + x];
        }
        
        tileT* data() {
            return m_tiles;
        }
        
        const tileT* data() const {
            return m_tiles;
        }
        
        const tileT& get_nearest(int x, int y) const {
            x = std::max(x, 0);
            y = std::max(y, 0);
            x = std::min(x, width - 1);
            y = std::min(y, height - 1);
            return m_tiles[y * width + x];
        }
        
        tileT& get_nearest(int x, int y) {
            x = std::max(x, 0);
            y = std::max(y, 0);
            x = std::min(x, width - 1);
            y = std::min(y, height - 1);
            return m_tiles[y * width + x];
        }
        
        constexpr static const int width = 512; //TODO add into general config file
        constexpr static const int height = 512; //TODO add into general config file
        constexpr static const int size = width * height; //TODO add into general config file
    private:
        tileT m_tiles[size];
    };
}

#endif
