#ifndef gameClient_region_context_h
#define gameClient_region_context_h

#include <vector>
#include <stdint.h>
#include "src/region/layer.h"

namespace region {    
    class context {
    public:
        context();
        context(const context& copy) = delete;
        ~context();
        
        bool update();
        
        const region::layer<uint32_t>& operator[](const int i) const;
        region::layer<uint32_t>& operator[](const int i);
        
        int size() const;
    private:         
        
        std::vector<region::layer<uint32_t>> m_layers;   
    };    
}

#endif
