#ifndef gameClient_region_context_h
#define gameClient_region_context_h

#include <vector>
#include <stdint.h>
#include <unordered_map>

#include "src/util/lock_ref.h"
#include "src/region/static_obj.h"
#include "src/region/dynamic_obj.h"
#include "src/region/layer.h"
#include "src/graphic/controller.h"

namespace region {    
    class context : public util::locked_ref_item {
    public:
        context(graphic::controller& graphicCtrl);
        context(const context& copy) = delete;
        ~context();
        
        void load_layers(std::vector<region::layer<uint32_t>>&& move);
        void load_static_objects(std::vector<region::static_obj>& objs);
        void load_dynamic_objects(std::vector<region::dynamic_obj>& objs);
        
        bool update();
        
        const region::layer<uint32_t>& operator[](const int i) const;
        region::layer<uint32_t>& operator[](const int i);
                
        int size() const;
    private:  
        graphic::controller& m_graphicCtrl;
        std::vector<region::layer<uint32_t>> m_layers;  
        
        std::unordered_map<uint32_t, region::static_obj> m_staticObjects;
        std::unordered_map<uint32_t, region::dynamic_obj> m_dynamicObjects;
    };    
}

#endif
